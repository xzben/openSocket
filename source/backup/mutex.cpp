#include"mutex.h"

BEGIN_NAMESPACE

Mutex::Mutex()
{

}

Mutex::~Mutex()
{
	
}

void Mutex::lock()
{
	m_lock.lock();
}

bool Mutex::try_lock(uint32 milliseconds /*= 200*/)
{
	return m_lock.try_lock_for(std::chrono::milliseconds(milliseconds));
}

void Mutex::unlock()
{
	m_lock.unlock();
}

END_NAMESPACE