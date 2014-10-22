/********************************************************************************
*	文件名称:	Object.h														*
*	创建时间：	2014/03/18														*
*	作   者 :	xzben															*
*	文件功能:	实现系统的公共基类Object类，和一些附加辅助类						*
*********************************************************************************/

#include"config.h"

#ifndef __2014_03_18_OBJECT_H__
#define __2014_03_18_OBJECT_H__

BEGIN_NAMESPACE

/*
*	确保其继承子类不能复制
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
	*	function : 引用加一
	*	return:
	*		WIN32: 引用加一后的值
	*/
	inline uint32_t retain();
	/*
	*	function : 引用减一
	*	return:
	*		WIN32: 返回引用减一后的值
	*/
	inline uint32_t release();
protected:
	Ref();
	virtual ~Ref();
private:
	uint32_t  m_ref;
};
/*
*	系统中所有类的公共基类
*/
class EXP_IMP_DLL Object
{
public:
	Object();
	virtual ~Object();
};

END_NAMESPACE
#endif//__2014_03_18_OBJECT_H__