#include "socket_option.h"

#if defined(__PLATFORM_WIN32__)
	#include <MSWSock.h>
#endif
BEGIN_NAMESPACE

SocketOption::SocketOption()
{

}

SocketOption::~SocketOption()
{

}

bool SocketOption::setResueAddr(Socket& sock, bool bReuse)
{
	DWORD opt = bReuse ? 1 : 0;

	return setoption(sock, SO_REUSEADDR, &opt, sizeof(opt));
}

bool SocketOption::isReuseAddr(Socket& sock)
{
	DWORD opt = 0;
	int32 optlen = sizeof(opt);

	VERIFY(getoption(sock, SO_REUSEADDR, &opt, &optlen));

	return !(opt == 0);
}

bool SocketOption::isListening(Socket& sock)
{
	DWORD opt = 0;
	int32 optlen = sizeof(opt);

	VERIFY(getoption(sock, SO_ACCEPTCONN, &opt, &optlen));

	return  !(opt == 0);
}

bool SocketOption::getRemoteAddr(Socket& sock, InterAddress& remoteAddr)
{
	int nLen = remoteAddr.getAddrLen();
	bool bRet = (0 == getpeername(sock.getHandle(), remoteAddr.getAddress(), &nLen));

#ifdef _DEBUG
	int nErr = ::GetLastError();
#endif
	return bRet;
}

bool SocketOption::getLocalAddr(Socket& sock, InterAddress& localAddr)
{
	int nLen = localAddr.getAddrLen();
	bool bRet = (0 == getsockname(sock.getHandle(), localAddr.getAddress(), &nLen));

#ifdef _DEBUG
	int nErr = ::GetLastError();
#endif
	return bRet;
}

#ifdef __PLATFORM_WIN32__
int32 SocketOption::getConnectTime(Socket& sock)
{
	int32 nSecondConnect = -1;
	int32 nOptLen = sizeof(nSecondConnect);

	VERIFY(getoption(sock, SO_CONNECT_TIME, &nSecondConnect, &nOptLen));

	return nSecondConnect;
}
#endif//__PLATFORM_WIN32__

int32 SocketOption::getRecvBufSize(Socket& sock)
{
	int32 nOptVal = 0;
	int32 nOptLen = sizeof(nOptVal);
	
	VERIFY(getoption(sock, SO_RCVBUF, &nOptVal, &nOptLen));

	return nOptVal;
}

int32 SocketOption::getSendBufSize(Socket& sock)
{
	int32 nOptVal = 0;
	int32 nOptLen = sizeof(nOptVal);

	VERIFY(getoption(sock, SO_SNDBUF, &nOptVal, &nOptLen));

	return nOptVal;
}

Socket::SockType SocketOption::getSocketType(Socket& sock)
{
	int32 nOptVal = 0;
	int32 nOptLen = sizeof(nOptVal);

	VERIFY(getoption(sock, SO_TYPE, &nOptVal, &nOptLen));

	return (Socket::SockType)nOptVal;
}

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/ms738573(v=vs.85).aspx
*/
bool  SocketOption::setBlockMode(Socket& sock, bool bBlock)
{
	SOCKET_HANDLE hSock = sock.getHandle();
	if (INVALID_SOCKET_HANDLE == hSock)
		return false;

#if defined(__PLATFORM_WIN32__)

	u_long iMode = bBlock ? 0 : 1;
	return (SOCKET_ERROR != ioctlsocket(hSock, FIONBIO, &iMode));

#elif defined(__PLATFORM_LINUX__)

	int flag;
	if (flag = fcntl(fd, F_GETFL, 0) < 0)
		return false;
	
	SET_DEL_BIT( flag, O_NONBLOCK, !bBlock );

	if (fcntl(fd, F_SETFL, flag) < 0)
		return false;

	return true;
#endif
}

bool  SocketOption::isBlockMode(Socket& sock)
{
	SOCKET_HANDLE hSock = sock.getHandle();
	if (INVALID_SOCKET_HANDLE == hSock)
		return false;

#if defined(__PLATFORM_WIN32__)

	return (SOCKET_ERROR != ioctlsocket(hSock, FIONBIO, ));

#elif defined(__PLATFORM_LINUX__)

	int flag;
	if (flag = fcntl(fd, F_GETFL, 0) < 0)
		return false;

	return QUERY_IS_SET_BIT(flag, O_NONBLOCK);

#endif

}
bool  SocketOption::setRecvBufSize(Socket& sock, int32 nBufSize)
{
	return setoption(sock, SO_RCVBUF, &nBufSize, sizeof(nBufSize));
}

bool  SocketOption::setSendBufSize(Socket& sock, int32 nBufSize)
{
	return setoption(sock, SO_SNDBUF, &nBufSize, sizeof(nBufSize));
}

bool SocketOption::setoption(Socket& sock, uint32 optname, const void *optval, int32 optlen)
{
	SOCKET_HANDLE hSocket = sock.getHandle();
	if (INVALID_SOCKET_HANDLE == hSocket)
		return false;

	bool bRet = (SOCKET_ERROR != ::setsockopt(hSocket,
		SOL_SOCKET, optname,
		(char*)optval,
		optlen));

	int nErr = ::GetLastError();

	return bRet;
}

bool SocketOption::getoption(Socket& sock, uint32 optname, void* optval, int32* optlen)
{
	SOCKET_HANDLE hSocket = sock.getHandle();
	if (INVALID_SOCKET_HANDLE == hSocket)
		return false;

	bool bRet = (SOCKET_ERROR != ::getsockopt(hSocket,
		SOL_SOCKET, optname,
		(char*)optval,
		optlen));

	int nErr = ::GetLastError();

	return bRet;
}

END_NAMESPACE