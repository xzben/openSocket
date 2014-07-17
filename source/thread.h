/********************************************************************************
*	�ļ�����:	thread.h														*
*	����ʱ�䣺	2014/06/05														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ��̶߳���												*
*********************************************************************************/

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