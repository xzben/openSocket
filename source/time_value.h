/********************************************************************
*	�ļ�����:	time_value.h										*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	ƽ̨�޹�ʱ��											*
*********************************************************************/

#include"object.h"

#if defined(__PLATFORM_WIN32__)
	#include <winsock2.h>
#elif defined(__PLATFORM_LINUX__)
	#include<sys/time.h>
#endif//__PLATFORM_WIN32__

#ifndef __2014_03_23_TIME_VALUE_H__
#define __2014_03_23_TIME_VALUE_H__

BEGIN_NAMESPACE

const long ONE_SECOND_TO_MSECOND = 1000;
const long ONE_SECOND_TO_USECOND = 1000 * 1000;
const long ONE_MSECOND_TO_USECOND = 1000;

class TimeValue
{
public:
	TimeValue();
	explicit TimeValue(long sencond, long msec = 0, long usec = 0);
	explicit TimeValue(const struct timeval& tmval);
	virtual ~TimeValue();

	//��
	long sec();
	//����
	long msec();
	//΢��
	long usec();


	void set(long sec, long msec = 0, long usec = 0);
	
	operator timeval()const;
	operator const timeval*() const;

	TimeValue operator+(const TimeValue& rTime);
	TimeValue operator-(const TimeValue& rTime);

private:
	void normalize();

private:
	struct timeval	m_tmVal;
};
END_NAMESPACE

#endif//__2014_03_23_TIME_VALUE_H__