/********************************************************************************
*	文件名称:	acceptor.h														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	用于接收连接请求的接收器，设计理念来自ACE							*
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
	*	localAddr		acceptor 监听的本地地址
	*	bReuseAddr		绑定的本地地址是否可重复绑定
	*/
	Acceptor(const InterAddress& localAddr, bool bReuseAddr = false);
	virtual ~Acceptor();
	/*
	*	localAddr		acceptor 监听的本地地址
	*	bReuseAddr		绑定的本地地址是否可重复绑定
	*/
	bool open(const InterAddress& localAddr, bool bReuseAddr = false);

	bool accept(Stream& streamCon);
	bool accept(const TimeValue& tmVal, Stream& streamCon);
private:
	Socket	m_sock;
};
END_NAMESPACE
#endif//__2014_03_22_ACCEPTOR_H__
