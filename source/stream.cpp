#include "stream.h"

BEGIN_NAMESPACE


Stream::Stream()
{

}

Stream::~Stream()
{

}

int32	Stream::recv(void* pBuf, int32 nLen)
{
	return getSocket().recv(pBuf, nLen);
}
int32	Stream::recv(void* pBuf, int32 nLen, TimeValue& tmVal)
{
	return getSocket().recv(pBuf, nLen, tmVal);
}

int32	Stream::send(const void *pBuf, const int32 nLen)
{
	return getSocket().send(pBuf, nLen);
}

int32	Stream::send(const void *pBuf, const int32 nLen, TimeValue& tmVal)
{
	return getSocket().send(pBuf, nLen, tmVal);
}

bool	Stream::getRemoteAddress(char* szIp, int16& port)
{
	return m_remoteAddress.getAddress(szIp, port);
}

Socket&	Stream::getSocket()
{
	return m_sock;
}

InterAddress&   Stream::getRemoteAddress()
{
	return m_remoteAddress;
}
END_NAMESPACE
