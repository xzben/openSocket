/********************************************************************************
*	文件名称:	thread.h														*
*	创建时间：	2014/06/05														*
*	作   者 :	xzben															*
*	文件功能:	系统中使用的线程对象												*
*********************************************************************************/

#include "object.h"
#include <thread>

BEGIN_NAMESPACE

class EXP_IMP_DLL Thread : public Object, public NoCopyAble
{
public:
	Thread();
	virtual ~Thread();

	int32		getID();
	bool		isJoinAble();
	void		join();
	void		detatch();
	void		swap();
protected:
	std::thread		m_thread;
};

END_NAMESPACE