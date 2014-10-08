/********************************************************************
*	�ļ�����:	time_value.h										*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	ƽ̨�޹�ʱ��											*
*********************************************************************/

#include"object.h"
#ifndef __2014_03_23_TIME_VALUE_H__
#define __2014_03_23_TIME_VALUE_H__

BEGIN_NAMESPACE

#include <time.h>
#if defined(_WIN32)
	#include <winsock2.h>
#elif defined(_LINUX)
	#include<sys/time.h>
#endif//_WIN32


const int32 ONE_SECOND_TO_MSECOND = 1000;
const int32 ONE_SECOND_TO_USECOND = 1000 * 1000;
const int32 ONE_MSECOND_TO_USECOND = 1000;

class EXP_IMP_DLL TimeValue : public Object
{
public:
	TimeValue();
	explicit TimeValue(int32 sencond, int32 msec = 0, int32 usec = 0);
	explicit TimeValue(const struct timeval& tmval);
	virtual ~TimeValue();

	//��
	int32 sec();
	//����
	int32 msec();
	//΢��
	int32 usec();


	void set(int32 sec, int32 msec = 0, int32 usec = 0);

#if	  defined(_WIN32)
	struct timeval const* getTimeval() const { return &m_tmVal; }
#elif defined(_LINUX)
	struct timeval* getTimeval() const;
#endif
// 	operator struct timeval const  () const;
// 	operator struct timeval const *() const;

	TimeValue& operator+(const TimeValue& rTime);
	TimeValue& operator-(const TimeValue& rTime);

private:
	void normalize();

private:
	struct timeval	m_tmVal;
};
END_NAMESPACE

#endif//__2014_03_23_TIME_VALUE_H__
