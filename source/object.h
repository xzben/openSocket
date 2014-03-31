/********************************************************************************
*	�ļ�����:	Object.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:	ʵ��ϵͳ�Ĺ�������Object�࣬��һЩ���Ӹ�����						*
*********************************************************************************/

#include"config.h"

#ifndef __2014_03_18_OBJECT_H__
#define __2014_03_18_OBJECT_H__

BEGIN_NAMESPACE

/*
*	ȷ����̳����಻�ܸ���
*/
class NoCopyAble
{
protected:
	NoCopyAble(){}
	virtual ~NoCopyAble(){}
private:
	NoCopyAble(const NoCopyAble& rObject);
	NoCopyAble&	operator=(const NoCopyAble& rObject);
};

/*
*	ϵͳ��������Ĺ�������
*/
class Object
{
public:
	Object();
	virtual ~Object();
};

END_NAMESPACE
#endif//__2014_03_18_OBJECT_H__