/********************************************************************************
*	�ļ�����:	message.h														*
*	����ʱ�䣺	2014/07/07														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��Ϣ�õ�ʵ��													*
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