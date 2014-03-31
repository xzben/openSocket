#include "Connector.h"

BEGIN_NAMESPACE

Connector::Connector()
{

}

Connector::~Connector()
{

}

bool Connector::connect(Stream& stream, const InterAddress& addrCon)
{
	return stream.getSocket().connect(addrCon);
}
END_NAMESPACE