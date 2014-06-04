/********************************************************************************
*	�ļ�����:	Stream.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	һ��TCP����ͨ����,����TCP���Ӷ˵�����ͨ�Ŷ���ͨ�������������		*
*				�����������ACE													*
*********************************************************************************/

#ifndef __2014_03_22_STREAM_H__
#define __2014_03_22_STREAM_H__

#include "socket.h"

BEGIN_NAMESPACE
class TimeValue;
class Acceptor;
class Connector;

class EXP_IMP_DLL Stream
{
	friend class Acceptor;
	friend class Connector;
public:
	Stream();
	virtual ~Stream();


	int32	recv(void* pBuf, int32 nLen);
	int32	recv(void* pBuf, int32 nLen, const TimeValue& tmVal);

	int32	send(const void *pBuf, const int32 nLen);
	int32	send(const void *pBuf, const int32 nLen, const TimeValue& tmVal);

	bool	getRemoteAddress(char* szIp, int16& port);
protected:
	Socket&			getSocket();
	InterAddress&   getRemoteAddress();
	Socket			m_sock;
	InterAddress	m_remoteAddress;
};

END_NAMESPACE
#endif//__2014_03_22_STREAM_H__