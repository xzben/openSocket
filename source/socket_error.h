#include "socket.h"

#ifndef __2014_03_31_SOCKET_ERROR_H__
#define __2014_03_31_SOCKET_ERROR_H__

BEGIN_NAMESPACE

class /*EXP_IMP_DLL*/ SocketError
{
public:
	static SocketError* instance();

	char* getErrorDescript(int nErrId);
private:
	SocketError(){};
	~SocketError(){};
};

END_NAMESPACE
#endif//__2014_03_31_SOCKET_ERROR_H__