#include "thread.h"

BEGIN_NAMESPACE

Thread::Thread()
{
	m_pThread = nullptr;
}

Thread::Thread(ThreadFunc threadfunc, void* param /*= nullptr*/)
{
	this->open(threadfunc, param);
}

Thread::~Thread()
{
	if ( this->isJoinAble() )
	{
		this->join();
	}
	SAFE_DELETE(m_pThread);
}

void Thread::open(ThreadFunc threadfunc, void* param /* = nullptr */)
{
	m_pThread = new std::thread(threadfunc, param);
}

Thread::ThreadID	Thread::getID()
{
	if (nullptr == m_pThread)
		return Thread::ThreadID();

	return m_pThread->get_id();
}

bool	Thread::isJoinAble()
{
	if (nullptr == m_pThread)
		return true;

	return m_pThread->joinable();
}

void	Thread::join()
{
	if (nullptr == m_pThread)
		return;

	return m_pThread->join();
}

void	Thread::detatch()
{
	if (nullptr == m_pThread)
		return;

	return m_pThread->detach();
}

END_NAMESPACE

