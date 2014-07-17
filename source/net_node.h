/********************************************************************************
*	文件名称:	net_node.h														*
*	创建时间：	2014/07/07														*
*	作   者 :	xzben															*
*	文件功能:	集群节点															*
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
	uint32			m_nodeHandle;		//节点的句柄
	InterAddress	m_nodeAddr;			//节点的网络地址
	InterAddress	m_masterAddr;		//master节点的地址
};
END_NAMESPACE