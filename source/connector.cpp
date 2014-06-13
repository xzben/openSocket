#include "connector.h"

BEGIN_NAMESPACE

Connector::Connector()
{

}

Connector::~Connector()
{

}

bool Connector::connect(Stream& stream, const InterAddress& addrCon)
{
	checkStream(stream);
	stream.getRemoteAddress().open(addrCon.getAddress());
	return stream.getSocket().connect(addrCon);
}

bool Connector::connect(Stream& stream, const InterAddress& addrCon, TimeValue& tmVal)
{
	checkStream(stream);
	stream.getRemoteAddress().open(addrCon.getAddress());
	return stream.getSocket().connect(addrCon, tmVal);
}

void Connector::checkStream(Stream& stream)
{
	if (!stream.getSocket().isValidTcp())
	{
		stream.getSocket().close();
		stream.getSocket().open(Socket::SOCK_TCP);
	}
}

END_NAMESPACE
