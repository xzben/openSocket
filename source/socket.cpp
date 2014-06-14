/********************************************************************************
*	�ļ�����:	Socket.cpp														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	���ļ���Ҫʵ��һ����ƽ̨�޹ص�Socket��								*
*********************************************************************************/

#include "socket.h"
#include "socket_option.h"
#include "socket_handle.h"

BEGIN_NAMESPACE

Socket::Socket()
{
	m_hSocket = INVALID_SOCKET_HANDLE;
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
	ASSERT(INVALID_SOCKET_HANDLE != hSocket);
	close();
	m_hSocket = hSocket;
}
  
SOCKET_HANDLE	Socket::dettach()
{
	SOCKET_HANDLE retHandle = m_hSocket;
	m_hSocket = INVALID_SOCKET_HANDLE;
	return retHandle;
}
  
bool	Socket::isValidTcp()
{
	if (INVALID_SOCKET_HANDLE == m_hSocket)
		return false;

	SocketOption option;
	return SOCK_TCP == option.getSocketType(*this);
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
#if	defined(_WIN32)
	bRet = (0 == ::closesocket(m_hSocket));
#elif defined(_LINUX)
	bRet = (0 == ::close(m_hSocket));
#endif//_WIN32
	m_hSocket = INVALID_SOCKET_HANDLE;

	return bRet;
}
  
bool	Socket::bind(const InterAddress& addrBind)
{
	if (SOCKET_ERROR == ::bind(m_hSocket, addrBind.getAddress(), addrBind.getAddrLen()))
	{
  		int32 nErr = ::GetLastError();
  		//������err Ϊ WSAEADDRINUSE  10048
  		return false;
	}
	return true;
}
  
bool	Socket::listen(int32 nBacklog /*= 10*/)
{
	if (SOCKET_ERROR == ::listen(m_hSocket, nBacklog))
	{
  		int32 nErr = ::GetLastError();
  		return false;
	}
	return true;
}

bool Socket::accept(Socket& sockCon, InterAddress* remoteAddr /*= nullptr*/)
{
	InterAddress addrCon;
	socklen_t nAddrLen = addrCon.getAddrLen();
  
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
		int32 nErr = ::GetLastError();
		return false;
	}

	return true;
}

bool	Socket::connect(const InterAddress& addrCon, const TimeValue& tmVal)
{
	SocketOption option;
	//Ϊ��ʵ�����ӳ�ʱ�Ĺ��ܣ��Ƚ�socket����Ϊ������ģʽ��Ȼ���ٻָ���ԭ�ȵ�ģʽ
	bool bIsBlock = option.isBlockMode(*this);
	if ( bIsBlock )  option.setBlockMode(*this, false);

	int32 nRet = ::connect(m_hSocket, addrCon.getAddress(), addrCon.getAddrLen());

	if (nRet < 0) 
	{
		int32 error = ::GetLastError();
#ifdef _WIN32
		if ( WSAEWOULDBLOCK == error)
#elif  _LINUX
		if (EINPROGRESS == error)
#endif
		{
			fd_set wset;
			FD_ZERO(&wset);
			FD_SET(m_hSocket, &wset);
#if defined(_WIN32)
			int32 nWidth = 0;
#elif defined(_LINUX)
			int32 nWidth = m_hSocket + 1;
#endif
			if (::select(nWidth, NULL, &wset, NULL, tmVal.getTimeval()) > 0 && FD_ISSET(m_hSocket, &wset))
			{
				int32 valopt;
				socklen_t nLen = sizeof(valopt);
				
				getsockopt(m_hSocket, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &nLen);
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
	
	//Ϊ��ʵ�����ӳ�ʱ�Ĺ��ܣ��Ƚ�socket����Ϊ������ģʽ��Ȼ���ٻָ���ԭ�ȵ�ģʽ
	if(bIsBlock) option.setBlockMode(*this, true);

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

#if defined( _WIN32 )
	int32 selectWith = 0;
#elif defined( _LINUX )
	int32 selectWith = m_hSocket + 1;
#endif

	int32 nRet = ::select(selectWith, 
		pReadReady	 ? &setRead  : nullptr,
		pWriteReady	 ? &setWrite : nullptr,
		pExceptReady ? &setExcept: nullptr, tmVal.getTimeval());


	if (nRet > 0)
	{
		if (FD_ISSET(m_hSocket, &setRead))	 SET_PTR_VALUE_SAFE(pReadReady, true);
		if (FD_ISSET(m_hSocket, &setWrite)) SET_PTR_VALUE_SAFE(pWriteReady, true);
		if (FD_ISSET(m_hSocket, &setExcept))SET_PTR_VALUE_SAFE(pExceptReady, true);
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
	int32 nRecvSize = ::recv(m_hSocket, (char*)pBuf, nLen, nFlag);
	if (SOCKET_ERROR == nRecvSize)
	{
		int32 nErr = ::GetLastError();
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
	int32 nSendSize = ::send(m_hSocket, (char*)pBuf, nLen, nFlag);
	if (SOCKET_ERROR == nSendSize)
	{
		int32 nErr = ::GetLastError();
	}

	return nSendSize;
}

int32 Socket::send(const void *pBuf, const int32 nLen, const TimeValue& tmVal, int32 nFlag /* = 0 */)
{
	if (!isWriteReady(tmVal))
		return 0;

	return send(pBuf, nLen, nFlag);
}


int32 Socket::recvfrom(void* pBuf, int32 nLen, InterAddress& addFrom, int32 nFlag /*=0*/)
{
	socklen_t nFrom = addFrom.getAddrLen();

	int32 nRecvSize = ::recvfrom(m_hSocket, (char*)pBuf, nLen, nFlag, addFrom.getAddress(), &nFrom);
	if (SOCKET_ERROR == nRecvSize)
	{
		int32 nErr = ::GetLastError();
	}

	return nRecvSize;
	
}

int32 Socket::recvfrom(void* pBuf, int32 nLen, InterAddress& addFrom, const TimeValue& tmVal, int32 nFlag /* = 0 */)
{
	if (!isReadReady(tmVal))
		return 0;

	return recvfrom(pBuf, nLen, addFrom, nFlag);
}

int32 Socket::sendto(const void *pBuf, const int32 nLen, const InterAddress& addrTo, int32 nFlag /*= 0*/)
{
	int32 nSendSize = ::sendto(m_hSocket, (char*)pBuf, nLen, nFlag, addrTo.getAddress(), addrTo.getAddrLen());
	if (SOCKET_ERROR == nSendSize)
	{
		int32 nErr = ::GetLastError();
	}

	return nSendSize;
}

int32 Socket::sendto(const void *pBuf, const int32 nLen, const InterAddress& addrTo, const TimeValue& tmVal, int32 nFlag /* = 0 */)
{
	if (!isWriteReady(tmVal))
		return 0;

	return sendto(pBuf, nLen, addrTo, nFlag);
}
//////////////////////////////////////////////////////////////////////////
//�����Զ�װ�� SocketLib ��
#ifdef _WIN32

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
 	static bool			s_loadSockLib(int32 nHigh = 2, int32 nLow = 2);
 	static bool			s_destroySockLib();
 private:
 	static	bool		s_bLoadedSockLib;
 };
 
 bool SocketLibLoadHelper::s_bLoadedSockLib = false;
 bool SocketLibLoadHelper::s_loadSockLib(int32 nHigh /* = 2 */, int32 nLow /* = 2 */)
 {
 	if (s_bLoadedSockLib) //�Ѿ����ع���ֱ�ӷ���
 		return true;
 	s_bLoadedSockLib = true;
 
 	WORD wVersionRequested;
 	WSADATA wsaData;
 	int32 err;
 
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
 	if (!s_bLoadedSockLib) //��δ���ع������Ѿ�ж����
 		return true;
 
 	s_bLoadedSockLib = false;
 	if (0 != WSACleanup())
 		return false;
 
 	return true;
 }
 
 SocketLibLoadHelper g_socketLibLoadHelper; //����һ��ȫ�ֱ�����ʹ���л����Զ�װ�غ�ж�� SocketLib
#endif //_WIN32
//////////////////////////////////////////////////////////////////////////
END_NAMESPACE
