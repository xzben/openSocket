/********************************************************************************
*	文件名称:	Socket.h														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	本文件主要实现一个和平台无关的Socket类								*		
*	error list: http://msdn.microsoft.com/en-us/library/ms681391(v=vs.85).aspx	*
*********************************************************************************/

#include "socket_addr.h"
#include "time_value.h"

#ifndef __2014_03_18_SOCKET_H__
#define __2014_03_18_SOCKET_H__

BEGIN_NAMESPACE

class TimeValue;
class InterAddress;
class Socket;
/*
*		平台无关的Socket类
*/
class EXP_IMP_DLL Socket : public Object
{
public:
	enum SockType : DWORD
	{
		SOCK_TCP	=	 SOCK_STREAM,
		SOCK_UDP	=	 SOCK_DGRAM,
	};
	
	Socket();
	Socket(SOCKET_HANDLE hSocket);
	Socket(const SockType type, const int32 nAf = AF_INET, const int32 nProtocl = 0);
	virtual ~Socket();

 	void			attach(SOCKET_HANDLE hSocket);
 	SOCKET_HANDLE	dettach();
 	SOCKET_HANDLE	getHandle();
	bool			isValidTcp();

	bool			open(const SockType type, const int32 nAf = AF_INET, const int32 nProtocl = 0);
 	bool			shutdown(int32 how = SD_BOTH);
 	bool			close();
	bool			bind(const InterAddress& addrBind);
 	bool			listen(int32 nBacklog = 10);
 	
	bool			accept(Socket& sockCon, InterAddress* remoteAddr = nullptr);
	bool			accept(const TimeValue& tmVal, Socket& sockCon, InterAddress* remoteAddr = nullptr);

	bool			connect(const InterAddress& addrCon);
	bool			connect(const InterAddress& addrCon, const TimeValue& tmVal);

	int32			getReadyStatus(const TimeValue& tmVal, bool *pReadReady = nullptr,
								  bool* pWriteReady = nullptr, bool* pExceptReady = nullptr);
	bool			isReadReady(const TimeValue& tmVal);
	bool			isWriteReady(const TimeValue& tmVal);

	int32			recv(void* pBuf, int32 nLen, int32 nFlag = 0);
	int32			recv(void* pBuf, int32 nLen, const TimeValue& tmVal, int32 nFlag = 0);

	int32			send(const void *pBuf, const int32 nLen, int32 nFlag = 0);
	int32			send(const void *pBuf, const int32 nLen, const TimeValue& tmVal, int32 nFlag = 0);

	int32			recvfrom(void* pBuf, int32 nLen, InterAddress& addFrom, int32 nFlag = 0);
	int32			recvfrom(void* pBuf, int32 nLen, InterAddress& addFrom, const TimeValue& tmVal, int32 nFlag = 0);

	int32			sendto(const void *pBuf, const int32 nLen, const InterAddress& addrTo, int32 nFlag = 0);
	int32			sendto(const void *pBuf, const int32 nLen, const InterAddress& addrTo, const TimeValue& tmVal, int32 nFlag = 0);
protected:
	SOCKET_HANDLE	m_hSocket = INVALID_SOCKET_HANDLE;	//套接字句柄
#ifdef _WIN32
public:	
	bool			isBlocked()					{ return m_bIsBlock; }
	void			setBlocked(bool bIsBlock)	{ m_bIsBlock = bIsBlock; }
protected:
	// 由于window下未找到合适的判断套接字是否阻塞的方式，目前先用这个标记来自行实现一下
	// window 下默认为阻塞套接字
	bool			m_bIsBlock = true;	
#endif // _WIN32

};
END_NAMESPACE
#endif//__2014_03_18_SOCKET_H__
