/********************************************************************
*	文件名称:	time_value.h										*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	平台无关时间											*
*********************************************************************/

#include"object.h"

#ifdef __PLATFORM_WIN32__
	#include <winsock2.h> 
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

	//秒
	long sec();
	//毫秒
	long msec();
	//微妙
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