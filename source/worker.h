/********************************************************************************
*	文件名称:	worker.h														*
*	创建时间：	2014/07/07														*
*	作   者 :	xzben															*
*	文件功能:	系统工作器，不断完成消息队列中的任务								*
*********************************************************************************/

#include "object.h"

BEGIN_NAMESPACE

class EXP_IMP_DLL Worker : public Object
{
public:
	Worker();
	virtual ~Worker();

};
END_NAMESPACE