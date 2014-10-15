/********************************************************************************
*	�ļ�����:	condition.h														*
*	����ʱ�䣺	2014/06/24														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ��߳�ͬ����������										*
*********************************************************************************/

#ifndef __2014_10_12_CONDITION_H__
#define __2014_10_12_CONDITION_H__


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

#endif // !__2014_10_12_CONDITION_H__