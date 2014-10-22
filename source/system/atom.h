#ifndef __2014_10_21_ATOM_H__
#define __2014_10_21_ATOM_H__

#include "object.h"

BEGIN_NAMESPACE

/*
*	function : ԭ�����Լ� 1
*	parameter: var Ҫ�����ӵ�ԭ������ַ
*	return:
*		WIN32: �����������ֵ
*/
 uint32 AtomSelfAdd(void * var);

/*
*	function : ԭ�����Լ� 1
*	parameter: var Ҫ��������ԭ������ַ
*	return:
*		WIN32: �����Լ����ֵ
*/
 uint32 AtomSelfDec(void * var);

/*
*	function : ԭ��������ָ��ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ���ӵ�ֵ
*	return:
*		WIN32: ��������ǰ��ֵ
*/
 uint32 AtomAdd(void * var, const uint32 value);

/*
*	function : ԭ������Сָ��ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ��С��ֵ
*	return:
*		WIN32: ���ؼ�Сǰ��ֵ
*/
 uint32 AtomDec(void * var, int32 value);

/*
*	function : ԭ�������ó�ָ����ֵ
*	parameter: var Ҫ��������ԭ������ַ�� value Ҫ�����õ�ֵ
*	return:
*		WIN32: ��������ǰ��ֵ
*/
 uint32 AtomSet(void * var, const uint32 value);

/*
*	function : ��ȡԭ������ֵ
*	parameter: var Ҫ��������ԭ������ַ
*	return:
*		WIN32: ԭ�����ĵ�ǰֵ
*/
 uint32 AtomGet(void * var);

END_NAMESPACE
#endif//__2014_10_21_ATOM_H__