#include "socket.h"
#include "Stream.h"

#ifndef __2014_03_22_CONNECTOR_H__
#define __2014_03_22_CONNECTOR_H__

BEGIN_NAMESPACE

class EXP_IMP_DLL Connector
{
public:
	Connector();
	virtual ~Connector();

	bool connect(Stream& stream, const InterAddress& addrCon);
private:

};
END_NAMESPACE
#endif//__2014_03_22_CONNECTOR_H__