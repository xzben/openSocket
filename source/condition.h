/********************************************************************************
*	文件名称:	condition.h														*
*	创建时间：	2014/06/24														*
*	作   者 :	xzben															*
*	文件功能:	系统中使用的线程同步条件变量										*
*********************************************************************************/

#include "object.h"
#include <condition_variable>
#include <mutex>

BEGIN_NAMESPACE

class EXP_IMP_DLL std::condition_variable;
class EXP_IMP_DLL std::mutex;
/*
*
*/
class EXP_IMP_DLL Condition : public NoCopyAble, public Object
{
public:
	Condition();
	virtual ~Condition();

	void notify_one();
	void notify_all();

	void wait();
private:
	std::condition_variable  m_condition;
	std::mutex				 m_mutex;
	int						 m_NotifyCount;  //为了防止伪装的唤醒
	int						 m_WaitCount;
};
END_NAMESPACE