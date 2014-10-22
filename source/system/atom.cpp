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
*	function : ԭ�����Լ� 1
*	parameter: var Ҫ��������ԭ������ַ
*	return:
*		WIN32: �����Լ����ֵ
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
*	function : ԭ��������ָ��ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ���ӵ�ֵ
*	return:
*		WIN32: ��������ǰ��ֵ
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
*	function : ԭ������Сָ��ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ��С��ֵ
*	return:
*		WIN32: ���ؼ�Сǰ��ֵ
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
*	function : ԭ�������ó�ָ����ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ�����õ�ֵ
*	return:
*		WIN32: ��������ǰ��ֵ
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
*	function : ��ȡԭ������ֵ
*	parameter: var Ҫ��������ԭ������ַ
*	return:
*		WIN32: ԭ�����ĵ�ǰֵ
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