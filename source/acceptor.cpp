#include "Acceptor.h"
#include "stream.h"

BEGIN_NAMESPACE

Acceptor::Acceptor()
{

}

Acceptor::Acceptor(InterAddress& localAddr, bool bReuseAddr /* = false */)
{
	VERIFY(open(localAddr, bReuseAddr));
}

Acceptor::~Acceptor()
{

}

bool Acceptor::open(InterAddress& localAddr, bool bReuseAddr /*= false*/)
{
	if (!m_sock.open(Socket::SockType::SOCK_TCP))
		return false;

	if (!m_sock.bind(localAddr))
		return false;

	if (!m_sock.listen())
		return false;

	return true;
}

bool Acceptor::accept(Stream& streamCon)
{
	return m_sock.accept(streamCon.getSocket(), &streamCon.getRemoteAddress());
}

bool Acceptor::accept(const TimeValue& tmVal, Stream& streamCon)
{
	return m_sock.accept(tmVal, streamCon.getSocket(), &streamCon.getRemoteAddress());
}

END_NAMESPACE