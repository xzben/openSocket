/********************************************************************************
*	�ļ�����:	socket_addr.cpp													*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	���ļ���Ҫʵ��һ����ƽ̨�޹ص�Socket��ַ��							*
*********************************************************************************/

#include "socket_addr.h"
#include <memory.h>

BEGIN_NAMESPACE

InterAddress InterAddress::addr_any;

InterAddress::InterAddress()
{
	open(0, nullptr);
}

InterAddress::InterAddress(int32 Port, const char*	szIp /*= nullptr*/)
{
	open(Port, szIp);
}

InterAddress::InterAddress(sockaddr*	pSockaddr)
{
	open(pSockaddr);
}

InterAddress::~InterAddress()
{

}

void InterAddress::open(int32 Port, const char* szIp /*=nullptr*/)
{
	memset(&m_addr, 0, sizeof(m_addr));

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(Port);
	if (nullptr == szIp)
	{
#if defined(_WIN32)
		m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#elif defined(_LINUX)
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	}
	else
	{
#if defined(_WIN32)
		m_addr.sin_addr.S_un.S_addr = inet_addr(szIp);
#elif defined(_LINUX)
		m_addr.sin_addr.s_addr = inet_addr(szIp);
#endif
	}
}

void InterAddress::open(sockaddr* pSockaddr)
{
	VERIFY(sizeof(sockaddr) == sizeof(sockaddr_in));

	memcpy(&m_addr, pSockaddr, sizeof(sockaddr_in));
	//m_addr = *(sockaddr_in*)pSockaddr;
}

bool InterAddress::getAddress(char* szIp, int32& port)const
{
	port = ntohs(m_addr.sin_port);
	strcpy_s(szIp, 20, inet_ntoa(m_addr.sin_addr));


	return true;
}

int32  InterAddress::getAddrLen() const
{
	return sizeof(m_addr);
}

sockaddr*	InterAddress::getAddress() const
{
	return (sockaddr*)&m_addr;
}

END_NAMESPACE
