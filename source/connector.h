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
	bool connect(Stream& stream, const InterAddress& addrCon, const TimeValue& tmVal);

protected:
	// 检查 当前连接流是否为有效的TCP套接字。如果不是则强制将其初始化为有效的TCP
	void checkStream(Stream& stream);
};
END_NAMESPACE
#endif//__2014_03_22_CONNECTOR_H__