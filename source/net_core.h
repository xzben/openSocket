/********************************************************************************
*	�ļ�����:	net_core.h														*
*	����ʱ�䣺	2014/07/07														*
*	��   �� :	xzben															*
*	�ļ�����:	�������ģ�飬��Ҫͨ�� IOCP��window����epoll��linux��ʵ��			*
*********************************************************************************/

#include "object.h"
#include "socket.h"
#include "buffer.h"
#include <map>
BEGIN_NAMESPACE

class NetCoreImpl;
class NetCoreData;

int const ERROR_MSG_SIZE = 256;

class EXP_IMP_DLL NetCore : public Object
{
public:
	NetCore();
	virtual ~NetCore();

private:
	NetCoreImpl* m_pImpl; //ָ����ľ���ʵ�ֶ���
};

struct NetCoreData
{
	enum DataType
	{
		OP_ERROR = -1,
		OP_ACCEPT,
		OP_CONNECTED,
		OP_DISCONNECT,
		OP_RECV,
		OP_SEND,
		OP_DATA,
	};

	DataType		_type;
	SOCKET_HANDLE	_sock;
	union
	{
		struct{
			SOCKET_HANDLE _clientSock;
		}_acceptData;

		struct {
			
		}_connectedData;

		struct{
			
		}_disconnectData;

		struct{

		}_sockData;

		struct{
			char _msg[ERROR_BUF_SIZE];
		}_errorData;
	};
};

// �����࣬����ʵ���� NetCoreIocp �� NetCoreEpoll ʵ��
class NetCoreImpl
{
public:
	NetCoreImpl();
	virtual ~NetCoreImpl();
	
	virtual	bool			isValid() = 0;
	virtual SOCKET_HANDLE	listen(char *host, int32 port, int32 nBacklog = 32) = 0;
	virtual int				send(SOCKET_HANDLE sock, void* buffer, int len) = 0;
	virtual int				addMonitor(SOCKET_HANDLE sock) = 0;
	virtual int				wait(NetCoreData *datas, int max, int32 timeout = -1) = 0;
protected:
	typedef std::map<SOCKET_HANDLE, SendBuffer> MapSendBuffer;
	MapSendBuffer			m_sendbufs;
	Mutex					m_sMutex;
};

#if defined(_WIN32)
class NetCoreIocp : public NetCoreImpl
{
	typedef struct _IOCP_OVERLAPPED
	{
		/**
		* OVERLAPPED����ָ��
		* ָ����ɲ�������
		* ���ݸ�AcceptEx()�����һ������
		* ���ݸ�WSARecv()�ĵ�6������
		* GetQueuedCompletionStatus()���صĵ�4������
		*/
		OVERLAPPED _overlapped;
		/**
		* ָ��������ӽ����Ŀͻ��˾�������������ַ���ڴ�
		* ����ʹ�ö�̬������ڴ��
		* ���ݸ�AcceptEx()�ĵ�3������
		*
		*/
		char _outPutBuf[sizeof(SOCKADDR_IN)* 2 + 32];
		/**
		* �ͻ��˾�����IP��Ϣ����
		* ���ݸ�AcceptEx()�ĵ�5������
		*/
		unsigned long _dwLocalAddressLength;
		/**
		* �ͻ�������IP��Ϣ����
		* ���ݸ�AcceptEx()�ĵ�6������
		*/
		unsigned long _dwRemoteAddressLength;
		WSABUF _wsaBuffer;	//WSARecv���ջ�������,���ݸ�WSARecv()�ĵ�2������
		NetCoreData	_data;
	}IOCP_OVERLAPPED;
public:
	NetCoreIocp();
	virtual ~NetCoreIocp();
	bool			  isValid() override;
	SOCKET_HANDLE	  listen(char *host, int32 port, int32 nBacklog = 32) override;
	int				  send(SOCKET_HANDLE sock, void* buffer, int len) override;
	int				  addMonitor(SOCKET_HANDLE sock) override;
	int				  wait(NetCoreData *datas, int max, int32 timeout = -1) override;
	int				  close(SOCKET_HANDLE sock);
protected:
	bool			  addAccept(SOCKET_HANDLE listenSock);
	bool			  addSend(SOCKET_HANDLE sock, void* buffer, int len);
	bool			  addRecv(SOCKET_HANDLE sock, void* buffer, int len);
	IOCP_OVERLAPPED*  getOverlapped();
	void			  freeOverlapped(IOCP_OVERLAPPED *& pOverlapped);
	HANDLE			  m_hCompletionPort;
};
#endif//_WIN32

#if defined(_LINUX)
class NetCoreEpoll : public NetCoreImpl
{
	enum
	{
		MAX_EVENT = 64,
	};
public:
	NetCoreEpoll();
	virtual ~NetCoreEpoll();
	bool			isValid() override;
	SOCKET_HANDLE	listen(char *host, int32 port, int32 nBacklog = 32) override;
	int				send(SOCKET_HANDLE sock, void* buffer, int len) override;
	int				addMonitor(SOCKET_HANDLE sock) override;
	int				wait(NetCoreData *datas, int max, int32 timeout = -1) override;
protected:
	int				add(int sock, void* ud);
	int				del(int sock);
	int				write(int sock, void* ud, bool enable);
	int				epoll_wait(struct Event *e, int max, int timeout = -1);
	void			nonblocking(int fd);
protected:
	SOCKET_HANDLE	m_epfd;
};
#endif//_LINUX

END_NAMESPACE