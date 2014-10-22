/********************************************************************************
*	�ļ�����:	Object.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	ʵ��ϵͳ�Ĺ�������Object�࣬��һЩ���Ӹ�����						*
*********************************************************************************/

#include "object.h"
#include "atom.h"

BEGIN_NAMESPACE

uint32_t Ref::retain()
{
	return AtomSelfAdd(&m_ref);
}

uint32_t Ref::release()
{
	uint32_t ref = AtomSelfDec(&m_ref);
	if (ref <= 0)
	{
		delete this;
	}

	return ref;
}

Ref::Ref()
{
	AtomSet(&m_ref, 1);
}

Ref::~Ref()
{

}
//////////////////////////////////////////////////////////////////////////
Object::Object()
{

}

Object::~Object()
{

}

END_NAMESPACE