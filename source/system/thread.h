/********************************************************************************
*	文件名称:	thread.h														*
*	创建时间：	2014/06/05														*
*	作   者 :	xzben															*
*	文件功能:	系统中使用的线程对象												*
*********************************************************************************/

#ifndef __2014_10_12_THREAD_H__
#define __2014_10_12_THREAD_H__

#include "object.h"
#include <thread>

BEGIN_NAMESPACE

class EXP_IMP_DLL std::thread;

typedef void(*ThreadFunc)(void* );

class EXP_IMP_DLL Thread : public Object, public NoCopyAble
{
	typedef std::thread::id		ThreadID;
public:
	Thread();
	Thread(ThreadFunc threadfunc, void* param = nullptr);
	virtual ~Thread();

	void		open(ThreadFunc threadfunc, void* param = nullptr);
	ThreadID	getID();
	bool		isJoinAble();
	void		join();
	void		detatch();
protected:
	std::thread		*m_pThread;
};

END_NAMESPACE
#endif//!__2014_10_12_THREAD_H__