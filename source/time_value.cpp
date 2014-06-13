#include "time_value.h"

BEGIN_NAMESPACE

TimeValue::TimeValue()
{

}
TimeValue::TimeValue(int32 sencond, int32 msec /*= 0*/, int32 usec /*= 0*/)
{
	set(sencond, msec, usec);
}

TimeValue::TimeValue(const timeval& tmval)
{
	m_tmVal = tmval;
}

TimeValue::~TimeValue()
{

}

void TimeValue::normalize()
{
	m_tmVal.tv_sec += m_tmVal.tv_usec / ONE_SECOND_TO_USECOND;
	m_tmVal.tv_usec %= ONE_SECOND_TO_USECOND;
	
	if (m_tmVal.tv_sec > 0 && m_tmVal.tv_usec < 0)
	{
		m_tmVal.tv_sec--;
		m_tmVal.tv_usec += ONE_SECOND_TO_USECOND;
	}
}

int32 TimeValue::sec()
{
	return m_tmVal.tv_sec + m_tmVal.tv_usec / ONE_SECOND_TO_USECOND;
}

int32 TimeValue::msec()
{
	return m_tmVal.tv_sec * ONE_SECOND_TO_MSECOND + m_tmVal.tv_usec / ONE_MSECOND_TO_USECOND;
}

int32 TimeValue::usec()
{
	return m_tmVal.tv_sec * ONE_SECOND_TO_USECOND + m_tmVal.tv_usec;
}

void TimeValue::set(int32 sec, int32 msec /*= 0*/, int32 usec /*= 0*/)
{
	m_tmVal.tv_sec = sec;
	m_tmVal.tv_usec = usec + msec * ONE_MSECOND_TO_USECOND;

	normalize();
}

TimeValue::operator timeval()const
{
	return m_tmVal;
}

TimeValue::operator const timeval*() const
{
	return &m_tmVal;
}

TimeValue::operator timeval*()
{
	return &m_tmVal;
}
//TimeValue::operator timeval*() const
//{
//	TimeValue *pThis = const_cast<TimeValue*>(this);
//	return &(pThis->m_tmVal);
//}

TimeValue& TimeValue::operator+(const TimeValue& rTime)
{
	this->set(m_tmVal.tv_sec + rTime.m_tmVal.tv_sec,
			0,
			m_tmVal.tv_usec + rTime.m_tmVal.tv_usec);

	return *this;
}

TimeValue& TimeValue::operator-(const TimeValue& rTime)
{
	this->set(m_tmVal.tv_sec - rTime.m_tmVal.tv_sec,
		0,
		m_tmVal.tv_usec - rTime.m_tmVal.tv_usec);

	return *this;
}

END_NAMESPACE
