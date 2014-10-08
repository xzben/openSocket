/********************************************************************************
*	�ļ�����:	condition.h														*
*	����ʱ�䣺	2014/06/24														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ��߳�ͬ����������										*
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
	int						 m_NotifyCount;  //Ϊ�˷�ֹαװ�Ļ���
	int						 m_WaitCount;
};
END_NAMESPACE