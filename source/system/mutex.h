/********************************************************************************
*	文件名称:	mutex.h															*
*	创建时间：	2014/06/05														*
*	作   者 :	xzben															*
*	文件功能:	系统中使用的线程安全锁											*
*********************************************************************************/

#ifndef __2014_10_12_MUTEX_H__
#define __2014_10_12_MUTEX_H__

#include "object.h"
#include <mutex>

BEGIN_NAMESPACE

class EXP_IMP_DLL std::timed_mutex;

/*
*	互斥锁，使用 C++11 提供的定时锁对象实现
*	本锁使用的是定时锁
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