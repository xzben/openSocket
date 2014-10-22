#include "atom.h"

#ifdef _WIN32 
	#include <windows.h>
#endif

BEGIN_NAMESPACE

uint32 AtomSelfAdd(void * var)
{
#ifdef _WIN32
	return InterlockedIncrement((long *)(var)); // NOLINT
#else
	return __sync_add_and_fetch((uint32 *)(var), 1); // NOLINT
#endif
}

/*
*	function : 原子量自减 1
*	parameter: var 要被操作的原子量地址
*	return:
*		WIN32: 返回自减后的值
*/
uint32 AtomSelfDec(void * var)
{
#ifdef _WIN32
	return InterlockedDecrement((long *)(var)); // NOLINT
#else
	return __sync_add_and_fetch((uint32 *)(var), -1); // NOLINT
#endif
}

/*
*	function : 原子量增加指定值
*	parameter: var 要被操作的原子量地址， value 要增加的值
*	return:
*		WIN32: 返回增加前的值
*/
uint32 AtomAdd(void * var, const uint32 value)
{
#ifdef _WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

/*
*	function : 原子量减小指定值
*	parameter: var 要被操作的原子量地址， value 要减小的值
*	return:
*		WIN32: 返回减小前的值
*/
uint32 AtomDec(void * var, int32 value)
{
	value = value * -1;
#ifdef _WIN32
	return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
	return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}

/*
*	function : 原子量设置成指定的值
*	parameter: var 要被操作的原子量地址， value 要被设置的值
*	return:
*		WIN32: 返回设置前的值
*/
uint32 AtomSet(void * var, const uint32 value)
{
#ifdef _WIN32
	::InterlockedExchange((long *)(var), (long)(value)); // NOLINT
#else
	__sync_lock_test_and_set((uint32 *)(var), value);  // NOLINT
#endif
	return value;
}

/*
*	function : 获取原子量的值
*	parameter: var 要被操作的原子量地址
*	return:
*		WIN32: 原子量的当前值
*/
uint32 AtomGet(void * var)
{
#ifdef _WIN32
	return InterlockedExchangeAdd((long *)(var), 0); // NOLINT
#else
	return __sync_fetch_and_add((uint32 *)(var), 0);  // NOLINT
#endif
}

END_NAMESPACE