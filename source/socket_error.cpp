#include "socket_error.h"

BEGIN_NAMESPACE

char* SocketError::getErrorDescript(int nErrId)
{
	return "";
}
SocketError* SocketError::instance()
{
	static SocketError s_instance;

	return &s_instance;
}

END_NAMESPACE