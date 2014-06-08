#include "socket.h"
#include "stream.h"

#ifndef __2014_03_22_CONNECTOR_H__
#define __2014_03_22_CONNECTOR_H__

BEGIN_NAMESPACE

class EXP_IMP_DLL Connector : public Object
{
public:
	Connector();
	virtual ~Connector();
	
	bool connect(Stream& stream, const InterAddress& addrCon);
	bool connect(Stream& stream, const InterAddress& addrCon, const TimeValue& tmVal);

protected:
	// ��� ��ǰ�������Ƿ�Ϊ��Ч��TCP�׽��֡����������ǿ�ƽ����ʼ��Ϊ��Ч��TCP
	void checkStream(Stream& stream);
};
END_NAMESPACE
#endif//__2014_03_22_CONNECTOR_H__