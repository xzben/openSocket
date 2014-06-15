/********************************************************************************
*	文件名称:	Stream.h														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	一个TCP连接通道流,两个TCP连接端的数据通信都是通过这个对象来做		*
*				设计理念来自ACE													*
*********************************************************************************/

#ifndef __2014_03_22_STREAM_H__
#define __2014_03_22_STREAM_H__

#include "socket.h"

BEGIN_NAMESPACE
class TimeValue;
class Acceptor;
class Connector;

class EXP_IMP_DLL Stream : public Object
{
	friend class Acceptor;
	friend class Connector;
	friend class SocketOption;
public:
	Stream();
	virtual ~Stream();


	int32	recv(void* pBuf, int32 nLen);
	int32	recv(void* pBuf, int32 nLen, const TimeValue& tmVal);

	int32	send(const void *pBuf, const int32 nLen);
	int32	send(const void *pBuf, const int32 nLen, const TimeValue& tmVal);
	
	bool 	getLocalAddress(char* szIp, int16& port);
	bool	getRemoteAddress(char* szIp, int16& port);
protected:
	Socket&			getSocket();
	InterAddress&   getRemoteAddress();
	Socket			m_sock;
	InterAddress	m_remoteAddress;
};

END_NAMESPACE
#endif//__2014_03_22_STREAM_H__
