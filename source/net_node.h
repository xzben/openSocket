/********************************************************************************
*	�ļ�����:	net_node.h														*
*	����ʱ�䣺	2014/07/07														*
*	��   �� :	xzben															*
*	�ļ�����:	��Ⱥ�ڵ�															*
*********************************************************************************/

#include "object.h"
#include "socket_addr.h"
#include "logic_server.h"

BEGIN_NAMESPACE

class EXP_IMP_DLL NetNode : public Object
{
public:
	NetNode* getInstance();
	bool     registerLogicServer(const LogicServer& server);

protected:
	NetNode();
	virtual ~NetNode();
	
protected:
	uint32			m_nodeHandle;		//�ڵ�ľ��
	InterAddress	m_nodeAddr;			//�ڵ�������ַ
	InterAddress	m_masterAddr;		//master�ڵ�ĵ�ַ
};
END_NAMESPACE