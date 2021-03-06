/********************************************************************************
*	文件名称:	socket_addr.h													*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	本文件主要实现一个和平台无关的Socket 地址类						*
*********************************************************************************/

#include "object.h"
#include "socket_handle.h"

#ifndef __2014_03_17_SOCKET_ADDRESS_H__
#define __2014_03_17_SOCKET_ADDRESS_H__

BEGIN_NAMESPACE

/*
*		平台无关的 socket internet address 基类，目前实现为 ipv4 版本
*		以后将以此为基类扩展 ipv4 和 ipv6 版本，目前系统当使用地址都使用此地址
*/
class EXP_IMP_DLL InterAddress : public Object
{
public:
	static InterAddress addr_any;
public:
	InterAddress();
	InterAddress(int32 Port, const char* szIp = nullptr);
	InterAddress(sockaddr*	pSockaddr);
	virtual ~InterAddress();

	virtual	void		open(int32 Port, const char* szIp = nullptr);
	virtual void		open(sockaddr*	pSockaddr);
	virtual int32		getAddrLen() const;
	virtual sockaddr*	getAddress() const;
	virtual bool		getAddress(char* szIp, int32& port) const;
protected:
	sockaddr_in		m_addr;
};

END_NAMESPACE
#endif//__2014_03_17_SOCKET_ADDRESS_H__
