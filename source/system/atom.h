#ifndef __2014_10_21_ATOM_H__
#define __2014_10_21_ATOM_H__

#include "object.h"

BEGIN_NAMESPACE

/*
*	function : 原子量自加 1
*	parameter: var 要被增加的原子量地址
*	return:
*		WIN32: 返回自增后的值
*/
 uint32 AtomSelfAdd(void * var);

/*
*	function : 原子量自减 1
*	parameter: var 要被操作的原子量地址
*	return:
*		WIN32: 返回自减后的值
*/
 uint32 AtomSelfDec(void * var);

/*
*	function : 原子量增加指定值
*	parameter: var 要被操作的原子量地址， value 要增加的值
*	return:
*		WIN32: 返回增加前的值
*/
 uint32 AtomAdd(void * var, const uint32 value);

/*
*	function : 原子量减小指定值
*	parameter: var 要被操作的原子量地址， value 要减小的值
*	return:
*		WIN32: 返回减小前的值
*/
 uint32 AtomDec(void * var, int32 value);

/*
*	function : 原子量设置成指定的值
*	parameter: var 要被操作的原子量地址， value 要被设置的值
*	return:
*		WIN32: 返回设置前的值
*/
 uint32 AtomSet(void * var, const uint32 value);

/*
*	function : 获取原子量的值
*	parameter: var 要被操作的原子量地址
*	return:
*		WIN32: 原子量的当前值
*/
 uint32 AtomGet(void * var);

END_NAMESPACE
#endif//__2014_10_21_ATOM_H__