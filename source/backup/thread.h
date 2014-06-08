/********************************************************************************
*	�ļ�����:	thread.h														*
*	����ʱ�䣺	2014/06/05														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ��̶߳���												*
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