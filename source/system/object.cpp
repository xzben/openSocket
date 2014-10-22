/********************************************************************************
*	文件名称:	Object.h														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	实现系统的公共基类Object类，和一些附加辅助类						*
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