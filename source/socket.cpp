/********************************************************************************
*	文件名称:	Socket.cpp														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	本文件主要实现一个和平台无关的Socket类								*
*********************************************************************************/

#include "socket.h"
#include "socket_option.h"
#include <iostream>

BEGIN_NAMESPACE

Socket::Socket()
{

}

Socket::Socket(SOCKET_HANDLE hSocket)
{
	attach(hSocket);
}

Socket::Socket(const SockType type, const int32 nAf /*= AF_INET*/, const int32 nProtocl /*= 0*/)
{
	open(type, nAf, nProtocl);
}

Socket::~Socket()
{
	close();
}
 
void	Socket::attach(SOCKET_HANDLE hSocket)
{
	close();
	m_hSocket = hSocket;
}
  
SOCKET_HANDLE	Socket::dettach()
{
	SOCKET_HANDLE retHandle = m_hSocket;
	m_hSocket = INVALID_SOCKET_HANDLE;
	return retHandle;
}
  
SOCKET_HANDLE	Socket::getHandle()
{
	return m_hSocket;
}
  
bool	Socket::open(const SockType type, const int32 nAf /*= AF_INET*/, const int32 nProtocl /*= 0*/)
{
	ASSERT(INVALID_SOCKET_HANDLE == m_hSocket);
  
	m_hSocket = ::socket(nAf, type, nProtocl);
  
	ASSERT(INVALID_SOCKET_HANDLE != m_hSocket);
  
	return (INVALID_SOCKET_HANDLE != m_hSocket);
  
}
  
bool	Socket::shutdown(int32 how /*= SD_BOTH*/)
{
	if (INVALID_SOCKET_HANDLE == m_hSocket)	
  		return true;
  
	return (0 == ::shutdown(m_hSocket, how));
}
  
bool	Socket::close()
{
	if (INVALID_SOCKET_HANDLE == m_hSocket) 
  		return true;

	bool bRet = false;
#if	defined(__PLATFORM_WIN32__)
	bRet = (0 == ::closesocket(m_hSocket));
#elif defined(__PLATFORM_LINUX__)
	bRet = (0 == ::close(m_hSocket));
#endif//__PLATFORM_WIN32__
	m_hSocket = INVALID_SOCKET_HANDLE;

	return bRet;
}
  
bool	Socket::bind(const InterAddress& addrBind)
{
	if (SOCKET_ERROR == ::bind(m_hSocket, addrBind.getAddress(), addrBind.getAddrLen()))
	{
  		int nErr = ::GetLastError();
  		//常见的err 为 WSAEADDRINUSE  10048
  		return false;
	}
	return true;
}
  
bool	Socket::listen(int32 nBacklog /*= 10*/)
{
	if (SOCKET_ERROR == ::listen(m_hSocket, nBacklog))
	{
  		int nErr = ::GetLastError();
  		return false;
	}
	return true;
}

bool Socket::accept(Socket& sockCon, InterAddress* remoteAddr /*= nullptr*/)
{
	InterAddress addrCon;
	int nAddrLen = addrCon.getAddrLen();
  
	SOCKET_HANDLE hConnSocket = ::accept(m_hSocket, addrCon.getAddress(), &nAddrLen);
	if ( INVALID_SOCKET_HANDLE == hConnSocket)
	{
  		return false;
	}
  
	sockCon.attach(hConnSocket);
	if (nullptr != remoteAddr)
	{
		remoteAddr->open(addrCon.getAddress());
	}
	return true;
}

bool Socket::accept(const TimeValue& tmVal, Socket& sockCon, InterAddress* remoteAddr /* = nullptr */)
{
	if (!isReadReady(tmVal))
		return false;

	return accept(sockCon, remoteAddr);
}

bool	Socket::connect(const InterAddress& addrCon)
{
	if (SOCKET_ERROR == ::connect(m_hSocket, addrCon.getAddress(), addrCon.getAddrLen()))
	{
		int nErr = ::GetLastError();
		return false;
	}

	return true;
}
  
bool	Socket::connect(const InterAddress& addrCon, const TimeValue& tmVal)
{
	SocketOption option;
	option.setBlockMode(*this, false);

	int nRet = ::connect(m_hSocket, addrCon.getAddress(), addrCon.getAddrLen());

	if (nRet < 0) 
	{
		if (EINPROGRESS == ::GetLastError())
		{
			FD_SET wset;
			FD_ZERO(&wset);
			FD_SET(m_hSocket, &wset);
#if defined(__PLATFORM_WIN32__)
			int nWith = 0;
#elif defined(__PLATFORM_LINUX__)
			int nWidth = m_hSocket + 1;
#endif
			if (select(nWith, NULL, &wset, NULL, tmVal) > 0 && FD_ISSET(m_hSocket, &wset))
			{
				int valopt;
				int nLen = sizeof(valopt);
				
				getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &nLen);
				if (valopt) 
				{
					//fprintf(stderr, "Error in connection() %d - %s/n", valopt, strerror(valopt));
					//exit(0);
					return false;
				}
			}
			else
			{
				//fprintf(stderr, "Timeout or error() %d - %s/n", valopt, strerror(valopt));
				//exit(0);
				return false;
			}
		}
		else
		{
			//fprintf(stderr, "Error connecting %d - %s/n", errno, strerror(errno));
			//exit(0);
			return false;
		}
	}
	
	option.setBlockMode(*this, true);

	return true;
}

int32	Socket::getReadyStatus(const TimeValue& tmVal, bool *pReadReady /*= nullptr*/,
	bool* pWriteReady /*= nullptr*/, bool* pExceptReady/*= nullptr*/)
{
	if (INVALID_SOCKET_HANDLE == m_hSocket)
		return -1;

	fd_set setRead, setWrite, setExcept;
	FD_ZERO(&setRead); FD_ZERO(&setWrite); FD_ZERO(&setExcept);
	FD_SET(m_hSocket, &setRead); FD_SET(m_hSocket, &setWrite); FD_SET(m_hSocket, &setExcept);

	SET_PTR_VALUE_SAFE(pReadReady, false);
	SET_PTR_VALUE_SAFE(pWriteReady, false);
	SET_PTR_VALUE_SAFE(pExceptReady, false);

#if defined( __PLATFORM_WIN32__ )
	int32 selectWith = 0;
#elif define( __PLATFORM_LINUX__ )
	int32 selectWith = hSocket + 1;
#endif

	int32 nRet = ::select(selectWith, 
		pReadReady	 ? &setRead  : nullptr,
		pWriteReady	 ? &setWrite : nullptr,
		pExceptReady ? &setExcept: nullptr, tmVal);


	if (nRet > 0)
	{
		if (FD_ISSET(m_hSocket, &setRead))	 SET_PTR_VALUE_SAFE(pReadReady, true);
		if (FD_ISSET(m_hSocket, &pWriteReady)) SET_PTR_VALUE_SAFE(pWriteReady, true);
		if (FD_ISSET(m_hSocket, &pExceptReady))SET_PTR_VALUE_SAFE(pExceptReady, true);
	}

	return nRet;
}

bool Socket::isReadReady(const TimeValue& tmVal)
{
	bool bReady = false;
	VERIFY(getReadyStatus(tmVal, &bReady, nullptr, nullptr) >= 0);

	return bReady;
}


bool Socket::isWriteReady(const TimeValue& tmVal)
{
	bool bReady = false;
	VERIFY(getReadyStatus(tmVal, nullptr, &bReady, nullptr) >= 0);

	return bReady;
}

int32	Socket::recv(void* pBuf, int32 nLen, int32 nFlag /*= 0*/)
{
	int nRecvSize = ::recv(m_hSocket, (char*)pBuf, nLen, nFlag);
	if (SOCKET_ERROR == nRecvSize)
	{
		int nErr = ::GetLastError();
		std::cerr << "Socket::recv error : " << nErr << std::endl;
	}

	return nRecvSize;
}

int32	Socket::recv(void* pBuf, int32 nLen, const TimeValue& tmVal, int32 nFlag /* = 0 */)
{
	if (!isReadReady(tmVal))
		return 0;

	return recv(pBuf, nLen, nFlag);
}

int32	Socket::send(const void *pBuf, const int32 nLen, int32 nFlag /*= 0*/)
{
	int nSendSize = ::send(m_hSocket, (char*)pBuf, nLen, nFlag);
	if (SOCKET_ERROR == nSendSize)
	{
		int nErr = ::GetLastError();
		std::cerr << "Socket::send error : " << nErr << std::endl;
	}

	return nSendSize;
}

int32 Socket::send(const void *pBuf, const int32 nLen, const TimeValue& tmVal, int32 nFlag /* = 0 */)
{
	if (!isWriteReady(tmVal))
		return 0;

	return send(pBuf, nLen, nFlag);
}


int32 Socket::recvfrom(void* pBuf, int nLen, InterAddress& addFrom, int nFlag /*=0*/)
{
	int nFrom = addFrom.getAddrLen();

	int nRecvSize = ::recvfrom(m_hSocket, (char*)pBuf, nLen, nFlag, addFrom.getAddress(), &nFrom);
	if (SOCKET_ERROR == nRecvSize)
	{
		int nErr = ::GetLastError();
		std::cerr << "Socket::recvfrom error : " << nErr << std::endl;
	}

	return nRecvSize;
	
}

int32 Socket::recvfrom(void* pBuf, int nLen, InterAddress& addFrom, const TimeValue& tmVal, int nFlag /* = 0 */)
{
	if (!isReadReady(tmVal))
		return 0;

	return recvfrom(pBuf, nLen, addFrom, nFlag);
}

int32 Socket::sendto(const void *pBuf, const int nLen, const InterAddress& addrTo, int nFlag /*= 0*/)
{
	int nSendSize = ::sendto(m_hSocket, (char*)pBuf, nLen, nFlag, addrTo.getAddress(), addrTo.getAddrLen());
	if (SOCKET_ERROR == nSendSize)
	{
		int nErr = ::GetLastError();
		std::cerr << "Socket::Sendto error : "<< nErr << std::endl;
	}

	return nSendSize;
}

int32 Socket::sendto(const void *pBuf, const int nLen, const InterAddress& addrTo, const TimeValue& tmVal, int nFlag /* = 0 */)
{
	if (!isWriteReady(tmVal))
		return 0;

	return sendto(pBuf, nLen, addrTo, nFlag);
}
//////////////////////////////////////////////////////////////////////////
//用于自动装载 SocketLib 用
#ifdef __PLATFORM_WIN32__

 class SocketLibLoadHelper
 {
 public:
 	SocketLibLoadHelper()
 	{
 		s_loadSockLib();
 	}
 	~SocketLibLoadHelper()
 	{
 		s_destroySockLib();
 	}
 private:
 	static bool			s_loadSockLib(int nHigh = 2, int nLow = 2);
 	static bool			s_destroySockLib();
 private:
 	static	bool		s_bLoadedSockLib;
 };
 
 bool SocketLibLoadHelper::s_bLoadedSockLib = false;
 bool SocketLibLoadHelper::s_loadSockLib(int nHigh /* = 2 */, int nLow /* = 2 */)
 {
 	if (s_bLoadedSockLib) //已经加载过，直接返回
 		return true;
 	s_bLoadedSockLib = true;
 
 	WORD wVersionRequested;
 	WSADATA wsaData;
 	int err;
 
 	wVersionRequested = MAKEWORD(nHigh, nLow);
 	err = WSAStartup(wVersionRequested, &wsaData);
 	if (err != 0)
 		return false;
 
 	if (LOBYTE(wsaData.wVersion) != nHigh
 		|| HIBYTE(wsaData.wVersion) != nLow)
 	{
 		WSACleanup();
 		return false;
 	}
 	return true;
 }
 bool SocketLibLoadHelper::s_destroySockLib()
 {
 	if (!s_bLoadedSockLib) //还未加载过，或已经卸载了
 		return true;
 
 	s_bLoadedSockLib = false;
 	if (0 != WSACleanup())
 		return false;
 
 	return true;
 }
 
 SocketLibLoadHelper g_socketLibLoadHelper; //定义一个全局变量，使运行环境自动装载和卸载 SocketLib
#endif //__PLATFORM_WIN32__
//////////////////////////////////////////////////////////////////////////
END_NAMESPACE