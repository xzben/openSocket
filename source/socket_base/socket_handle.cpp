#include "socket_handle.h"


#if defined(_LINUX)
int32 	GetLastError()
{
	return errno;
}

#endif//_LINUX
