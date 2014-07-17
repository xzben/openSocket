/********************************************************************************
*	文件名称:	message.h														*
*	创建时间：	2014/07/07														*
*	作   者 :	xzben															*
*	文件功能:	系统消息泵的实现													*
*********************************************************************************/

#include "object.h"

BEGIN_NAMESPACE

class Message;

class EXP_IMP_DLL MessagePump : public Object
{
public:
	MessagePump();
	virtual ~MessagePump();

};

class EXP_IMP_DLL Message : public Object
{
public:
	Message();
	virtual ~Message();


};

END_NAMESPACE