#include "Stream.h"

BEGIN_NAMESPACE


Stream::Stream()
{

}

Stream::~Stream()
{

}

Socket	Stream::getSocket()
{
	return m_sock;
}

END_NAMESPACE