/********************************************************************
*	文件名称:	time_value.h										*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	平台无关时间											*
*********************************************************************/

#include"object.h"

#if defined(_WIN32)
	#include <winsock2.h>
#elif defined(_LINUX)
	#include<sys/time.h>
#endif//_WIN32

#ifndef __2014_03_23_TIME_VALUE_H__
#define __2014_03_23_TIME_VALUE_H__

BEGIN_NAMESPACE

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

	//秒
	int32 sec();
	//毫秒
	int32 msec();
	//微妙
	int32 usec();


	void set(int32 sec, int32 msec = 0, int32 usec = 0);
	
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