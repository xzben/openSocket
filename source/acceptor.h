/********************************************************************************
*	�ļ�����:	Stream.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:																	*
*********************************************************************************/

#include "socket.h"

#ifndef __2014_03_22_ACCEPTOR_H__
#define __2014_03_22_ACCEPTOR_H__

BEGIN_NAMESPACE

template<typename StreamType>
class EXP_IMP_DLL Acceptor
{
public:
	Acceptor(){}
	/*
	*	localAddr		acceptor �����ı��ص�ַ
	*	bReuseAddr		�󶨵ı��ص�ַ�Ƿ���ظ���
	*/
	Acceptor(InterAddress& localAddr, bool bReuseAddr = false)
	{
		VERIFY(open(localAddr, bReuseAddr));
	}

	virtual ~Acceptor()
	{

	}
	
	/*
	*	localAddr		acceptor �����ı��ص�ַ
	*	bReuseAddr		�󶨵ı��ص�ַ�Ƿ���ظ���
	*/
	bool open(InterAddress& localAddr, bool bReuseAddr = false)
	{
		if (!m_sock.open(Socket::SockType::SOCK_TCP))
			return false;

		if (!m_sock.bind(localAddr))
			return false;
	
		if (!m_sock.listen())
			return false;

		return true;
	}
private:
	Socket	m_sock;
};
END_NAMESPACE
#endif//__2014_03_22_ACCEPTOR_H__