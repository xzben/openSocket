/********************************************************************************
*	�ļ�����:	mutex.h															*
*	����ʱ�䣺	2014/06/05														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ��̰߳�ȫ��											*
*********************************************************************************/

#ifndef __2014_10_12_MUTEX_H__
#define __2014_10_12_MUTEX_H__

#include "object.h"
#include <mutex>

BEGIN_NAMESPACE

class EXP_IMP_DLL std::timed_mutex;

/*
*	��������ʹ�� C++11 �ṩ�Ķ�ʱ������ʵ��
*	����ʹ�õ��Ƕ�ʱ��
*/
class EXP_IMP_DLL Mutex : public Object, public NoCopyAble
{
public:
	Mutex();
	virtual ~Mutex();
	void lock();
	bool try_lock(uint32 milliseconds = 200);
	void unlock();
protected:
	std::timed_mutex	m_lock;
};

class EXP_IMP_DLL Guard : public Object
{
public:
	Guard(Mutex* pMutex);
	~Guard();
private:
	Mutex	*m_pGuardMuext;
};
END_NAMESPACE

#endif//!__2014_10_12_MUTEX_H__