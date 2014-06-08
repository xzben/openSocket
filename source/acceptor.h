/********************************************************************************
*	�ļ�����:	acceptor.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	���ڽ�����������Ľ������������������ACE							*
*********************************************************************************/

#include "socket.h"

#ifndef __2014_03_22_ACCEPTOR_H__
#define __2014_03_22_ACCEPTOR_H__

BEGIN_NAMESPACE

class Stream;
class InterAddress;
class TimeValue;
/*
*
*/
class EXP_IMP_DLL Acceptor : public Object
{
public:
	Acceptor();
	/*
	*	localAddr		acceptor �����ı��ص�ַ
	*	bReuseAddr		�󶨵ı��ص�ַ�Ƿ���ظ���
	*/
	Acceptor(InterAddress& localAddr, bool bReuseAddr = false);
	virtual ~Acceptor();
	/*
	*	localAddr		acceptor �����ı��ص�ַ
	*	bReuseAddr		�󶨵ı��ص�ַ�Ƿ���ظ���
	*/
	bool open(InterAddress& localAddr, bool bReuseAddr = false);

	bool accept(Stream& streamCon);
	bool accept(const TimeValue& tmVal, Stream& streamCon);
private:
	Socket	m_sock;
};
END_NAMESPACE
#endif//__2014_03_22_ACCEPTOR_H__