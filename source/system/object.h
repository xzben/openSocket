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
class EXP_IMP_DLL NoCopyAble
{
protected:
	NoCopyAble(){}
	virtual ~NoCopyAble(){}
private:
	NoCopyAble(const NoCopyAble& rObject) = delete;
	NoCopyAble&	operator=(const NoCopyAble& rObject) = delete;
};


class EXP_IMP_DLL Ref
{
public:
	/*
	*	function : ���ü�һ
	*	return:
	*		WIN32: ���ü�һ���ֵ
	*/
	inline uint32_t retain();
	/*
	*	function : ���ü�һ
	*	return:
	*		WIN32: �������ü�һ���ֵ
	*/
	inline uint32_t release();
protected:
	Ref();
	virtual ~Ref();
private:
	uint32_t  m_ref;
};
/*
*	ϵͳ��������Ĺ�������
*/
class EXP_IMP_DLL Object
{
public:
	Object();
	virtual ~Object();
};

END_NAMESPACE
#endif//__2014_03_18_OBJECT_H__