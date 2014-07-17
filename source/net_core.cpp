#include "net_core.h"
#include <iostream>
using namespace std;

BEGIN_NAMESPACE

NetCore::NetCore()
{
#if defined(_WIN32)
	m_pImpl = new NetCoreIocp;
#elif defined(_LINUX)
	m_pImpl = new NetCoreEpoll;
#endif
}

NetCore::~NetCore()
{
	SAFE_DELETE(m_pImpl);
}

//////////////////////////////////////////////////////////////////////////
// class NetCoreIocp
#if defined(_WIN32)

NetCoreIocp::NetCoreIocp()
{
	m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
}

NetCoreIocp::~NetCoreIocp()
{
	CloseHandle(m_hCompletionPort);
}

bool NetCoreIocp::isValid()
{
	return m_hCompletionPort != NULL;
}

SOCKET_HANDLE	NetCoreIocp::listen(char *host, int32 port, int32 nBacklog/* = 32*/)
{
	Socket sockListen(Socket::SOCK_TCP);
	if (!sockListen.bind(InterAddress(port, host)) || sockListen.listen(nBacklog))
	{
		fprintf(stderr, "bind host[%s] port[%d] failed!", host, port);
		sockListen.close();
		return INVALID_SOCKET_HANDLE;
	}
	
	SOCKET_HANDLE sockHandle = sockListen.dettach();

	if (addMonitor(sockHandle) != 0 || addAccept(sockHandle))
	{
		Socket sock(sockHandle);
		sock.close();

		return INVALID_SOCKET_HANDLE;
	}

	return sockHandle;
}

int NetCoreIocp::send(SOCKET_HANDLE sock, void* data, int len)
{
	Guard sGuard(&m_sMutex);
	MapSendBuffer::iterator it = m_sendbufs.find(sock);
	
	if (it == m_sendbufs.end())
	{
		SendBuffer buffer;
		buffer.pushData(data, len);
		m_sendbufs.insert(MapSendBuffer::value_type(sock, buffer));
	}
	else
	{
		it->second.pushData(data, len);
	}
}

int NetCoreIocp::addMonitor(SOCKET_HANDLE sock)
{
	HANDLE h = CreateIoCompletionPort((HANDLE)sock, m_hCompletionPort, (ULONG_PTR)sock, 0);
	
	if (h != m_hCompletionPort)
		return -1;
	
	return 0;
}

int NetCoreIocp::wait(NetCoreData *datas, int max, int32 timeout /*=-1*/)
{
	DWORD dwNumRead = 0;
	SOCKET_HANDLE eventSockHandle = 0;
	IOCP_OVERLAPPED* lpOverlapped;

	if (!GetQueuedCompletionStatus(m_hCompletionPort, &dwNumRead, (LPDWORD)&eventSockHandle, (LPOVERLAPPED*)&lpOverlapped, timeout))
	{
		DWORD dwErrorCode = ::GetLastError();

		//////////////////////////////////////////////////////////////////////////
		//�Ϸ��Ĵ���
		if (WAIT_TIMEOUT == dwErrorCode ||  // ��ʱ
			ERROR_INVALID_HANDLE == dwErrorCode) //����ʱ��δ֪ԭ��Ƿ�����������ǲ���ǰһ�γ���رպ�δ���������¼�
		{
			return 0;
		}
		//////////////////////////////////////////////////////////////////////////
		//��������error
		else if (ERROR_OPERATION_ABORTED == dwErrorCode) //����socket�ر�
		{
			datas->_type = NetCoreData::OP_DISCONNECT;
			datas->_sock = eventSockHandle;
			return 1;
		}
		else if (ERROR_NETNAME_DELETED == dwErrorCode) // �Զ�socket �ر�
		{
			datas->_type = NetCoreData::OP_DISCONNECT;
			datas->_sock = eventSockHandle;
			return 1;
		}
		else if (NetCoreData::OP_ACCEPT == lpOverlapped->_data._type) //Accept�ϵ�socket�رգ�����Ͷ�ݼ���
		{
			addAccept(eventSockHandle);
			return 0;
		}
		else//�ͻ����쳣�Ͽ��������ߣ��ϵ磬��ֹ����
		{
			datas->_type = NetCoreData::OP_DISCONNECT;
			datas->_sock = eventSockHandle;
		}
	}
	else if (0 == dwNumRead && NetCoreData::OP_RECV == lpOverlapped->_data._type) //���ӶϿ�
	{
		datas->_type = NetCoreData::OP_DISCONNECT;
		datas->_sock = eventSockHandle;
	}
	else
	{
		if (NetCoreData::OP_ACCEPT == lpOverlapped->_data._type)
		{
			*datas = lpOverlapped->_data;
		}
		else if (NetCoreData::OP_RECV == lpOverlapped->_data._type)
		{
			*datas = lpOverlapped->_data;
		}
		else if (NetCoreData::OP_SEND == lpOverlapped->_data._type)
		{
			*datas = lpOverlapped->_data;
		}
	}

	return 0;
}

bool NetCoreIocp::addAccept(SOCKET_HANDLE listenSock)
{
	IOCP_OVERLAPPED *pOverlapped = getOverlapped();
	if (nullptr == pOverlapped) 
		return false;

	memset(&pOverlapped->_overlapped, 0, sizeof(OVERLAPPED));
	pOverlapped->_dwLocalAddressLength = sizeof(SOCKADDR_IN)+16;//�ͻ��˾�����IP
	pOverlapped->_dwRemoteAddressLength = sizeof(SOCKADDR_IN)+16;//�ͻ�������IP
	memset( pOverlapped->_outPutBuf, 0, sizeof(SOCKADDR_IN)*2+32 );

	Socket client(Socket::SOCK_TCP);
	SOCKET sockClient = client.dettach();
	pOverlapped->_data._sock = listenSock;
	pOverlapped->_data._type = NetCoreData::OP_ACCEPT;
	pOverlapped->_data._acceptData._clientSock = sockClient;

	//Ͷ�ݽ������Ӳ���
	if (!::AcceptEx(listenSock,
		sockClient,
		pOverlapped->_outPutBuf, 0,
		pOverlapped->_dwLocalAddressLength,
		pOverlapped->_dwRemoteAddressLength,
		NULL, &pOverlapped->_overlapped))
	{
		int nErrCode = WSAGetLastError();
		if (ERROR_IO_PENDING != nErrCode)
		{
			freeOverlapped(pOverlapped);
			return false;
		}
	}

	return true;
}

bool NetCoreIocp::addSend(SOCKET_HANDLE sock, void* buffer, int len)
{
	IOCP_OVERLAPPED *pOverlapped = getOverlapped();
	if (NULL == pOverlapped)
		return false;

	memset(&pOverlapped->_overlapped, 0, sizeof(OVERLAPPED));
	pOverlapped->_wsaBuffer.buf = (CHAR*)buffer;
	pOverlapped->_wsaBuffer.len = len;
	pOverlapped->_overlapped.Internal = 0;
	pOverlapped->_data._sock = sock;
	pOverlapped->_data._type = NetCoreData::OP_SEND;

	DWORD dwSend = 0;
	DWORD dwFlags = 0;
	//Ͷ�����ݽ��ղ���
	if (SOCKET_ERROR == ::WSASend(sock,
		&pOverlapped->_wsaBuffer,
		1, &dwSend, dwFlags,
		&pOverlapped->_overlapped, NULL))
	{
		int nErrCode = WSAGetLastError();
		if (ERROR_IO_PENDING != nErrCode)
		{
			freeOverlapped(pOverlapped);
			pOverlapped = NULL;
			return false;
		}
	}
	return true;
}

bool NetCoreIocp::addRecv(SOCKET_HANDLE sock, void* buffer, int len)
{
	IOCP_OVERLAPPED *pOverlapped = getOverlapped();
	if (NULL == pOverlapped)
		return false;

	memset(&pOverlapped->_overlapped, 0, sizeof(OVERLAPPED));
	pOverlapped->_wsaBuffer.buf = (CHAR*)buffer;
	pOverlapped->_wsaBuffer.len = len;
	pOverlapped->_overlapped.Internal = 0;
	pOverlapped->_data._sock = sock;
	pOverlapped->_data._type = NetCoreData::OP_RECV;

	DWORD dwRecv = 0;
	DWORD dwFlags = 0;
	//Ͷ�����ݽ��ղ���
	if (SOCKET_ERROR == ::WSARecv(sock,
		&pOverlapped->_wsaBuffer,
		1, &dwRecv, &dwFlags,
		&pOverlapped->_overlapped, NULL))
	{
		int nErrCode = WSAGetLastError();
		if (ERROR_IO_PENDING != nErrCode)
		{
			freeOverlapped(pOverlapped);
			pOverlapped = NULL;
			return false;
		}
	}
	return true;
}
NetCoreIocp::IOCP_OVERLAPPED*  NetCoreIocp::getOverlapped()
{
	return new NetCoreIocp::IOCP_OVERLAPPED;
}
void NetCoreIocp::freeOverlapped(NetCoreIocp::IOCP_OVERLAPPED *& pOverlapped)
{
	SAFE_DELETE(pOverlapped)
}


#endif//_WIN32
//////////////////////////////////////////////////////////////////////////
// class NetCoreEpoll
#if defined(_LINUX)
NetCoreEpoll::NetCoreEpoll()
{
	m_epfd = epoll_create(1024);
}
NetCoreEpoll::~NetCoreEpoll()
{
	close(m_epfd);
}

bool NetCoreEpoll::isValid()
{
	return m_epfd == -1;
}

SOCKET_HANDLE	NetCoreEpoll::listen(char *host, int32 port, int32 nBacklog = 32)
{
	Socket sockListen(Socket::SOCK_TCP);
	if (!sockListen.bind(InterAddress(port, host)) || sockListen.listen(nBacklog))
	{
		fprintf(stderr, "bind host[%s] port[%d] failed!", host, port);
		sockListen.close();
		return INVALID_SOCKET_HANDLE;
	}

	SOCKET_HANDLE sockHandle = sockListen.dettach();

	if (addMonitor(sockHandle) != 0 )
	{
		Socket sock(sockHandle);
		sock.close();

		return INVALID_SOCKET_HANDLE;
	}

	return sockHandle;
}

int	NetCoreEpoll::send(SOCKET_HANDLE sock, void* buffer, int len)
{

}

int	NetCoreEpoll::addMonitor(SOCKET_HANDLE sock)
{

	return 0;
}

int	NetCoreEpoll::wait(NetCoreData *datas, int max, int32 timeout /*= -1*/)
{

}

int	NetCoreEpoll::add(int sock, void* ud)
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = ud;
	if (epoll_ctrl(m_epfd, EPOLL_CTL_ADD, sock, &ev) == -1){
		return 1
	}

	return 0;
}

int	NetCoreEpoll::del(int sock)
{
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, sock, NULL);
}

int	NetCoreEpoll::write(int sock, void* ud, bool enable)
{
	struct epoll_event ev;
	ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
	ev.data.ptr = ud;
	epoll_ctl(efd, EPOLL_CTL_MOD, sock, &ev);
}

/*
*	timeout �ȴ���ʱʱ��, ��λ����
*/
int	NetCoreEpoll::epoll_wait(struct Event *e, int max, int timeout /*= -1*/)
{
	struct epoll_event ev[MAX_EVENT];
	int wait_max = (max > MAX_EVENT) ? MAX_EVENT : max;

	int n = epoll_wait(m_epfd, ev, wait_max, timeout);

	for (int i = 0; i < n; i++)
	{
		e[i]._data = ev[i].data.ptr;
		unsigned flag = ev[i].events;
		e[i]._write = (flag & EPOLLOUT) != 0;
		e[i]._read = (flag &EPOLLIN) != 0;
	}

	return n;
}

void NetCoreEpoll::nonblocking(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if (-1 == flag) {
		return;
	}

	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
#endif//_LINUX
END_NAMESPACE
