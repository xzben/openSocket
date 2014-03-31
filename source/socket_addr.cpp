/********************************************************************************
*	�ļ�����:	socket_addr.cpp													*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	���ļ���Ҫʵ��һ����ƽ̨�޹ص�Socket��ַ��							*
*********************************************************************************/

#include "socket_addr.h"
#include <cstring>

BEGIN_NAMESPACE

InterAddress InterAddress::addr_any;

InterAddress::InterAddress()
	:InterAddress(0, nullptr)
{

}

InterAddress::InterAddress(int16 Port, const char*	szIp /*= nullptr*/)
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

void InterAddress::open(int16 Port, const char* szIp /*=nullptr*/)
{
	memset(&m_addr, 0, sizeof(m_addr));

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons(Port);
	if (nullptr == szIp)
		m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	else
		m_addr.sin_addr.S_un.S_addr = inet_addr(szIp);
}

void InterAddress::open(sockaddr* pSockaddr)
{
	VERIFY(sizeof(sockaddr) == sizeof(sockaddr_in));
	m_addr = *(sockaddr_in*)pSockaddr;
}

bool InterAddress::getAddress(char* szIp, int16& port)const
{
	port = ntohs(m_addr.sin_port);
	strcpy_s(szIp, 20, inet_ntoa(m_addr.sin_addr));

	return true;
}

int  InterAddress::getAddrLen() const
{
	return sizeof(m_addr);
}

sockaddr*	InterAddress::getAddress() const
{
	return (sockaddr*)&m_addr;
}

END_NAMESPACE
