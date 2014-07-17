/********************************************************************************
*	文件名称:	net_core.h														*
*	创建时间：	2014/07/07														*
*	作   者 :	xzben															*
*	文件功能:	网络核心模块，主要通过 IOCP（window）、epoll（linux）实现			*
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
	NetCoreImpl* m_pImpl; //指向核心具体实现对象
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

// 抽象类，具体实现由 NetCoreIocp 和 NetCoreEpoll 实现
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
		* OVERLAPPED类型指针
		* 指向完成操作参数
		* 传递给AcceptEx()的最后一个参数
		* 传递给WSARecv()的第6个参数
		* GetQueuedCompletionStatus()返回的第4个参数
		*/
		OVERLAPPED _overlapped;
		/**
		* 指向存有连接进来的客户端局域网和外网地址的内存
		* 必须使用动态分配的内存块
		* 传递给AcceptEx()的第3个参数
		*
		*/
		char _outPutBuf[sizeof(SOCKADDR_IN)* 2 + 32];
		/**
		* 客户端局域网IP信息长度
		* 传递给AcceptEx()的第5个参数
		*/
		unsigned long _dwLocalAddressLength;
		/**
		* 客户端外网IP信息长度
		* 传递给AcceptEx()的第6个参数
		*/
		unsigned long _dwRemoteAddressLength;
		WSABUF _wsaBuffer;	//WSARecv接收缓冲数据,传递给WSARecv()的第2个参数
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