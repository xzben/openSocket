/********************************************************************************
*	文件名称:	rw_queue.h														*
*	创建时间：	2014/07/15														*
*	作   者 :	xzben															*
*	文件功能:	线程安全的读写队列												*
*********************************************************************************/

#include "object.h"
#include "mutex.h"
#include <queue>

BEGIN_NAMESPACE

template<class type>
class RWQueue
{
public:
	RWQueue()
	{
		m_pRQueue = new std::queue<type>;
		m_pWQueue = new std::queue<type>;
	}

	virtual ~RWQueue()
	{
		SAFE_DELETE(m_pRQueue);
		SAFE_DELETE(m_pWQueue);
	}

	bool   push(type& item)
	{
		Guard wGuard(&m_wMutex);
		m_pWQueue->push(item);

		return true;
	}

	bool   pop(type& item)
	{
		{
			Guard rGuard(&m_rMutex);
			if ( !m_pRQueue->empty() )
			{
				item = m_pRQueue->front();
				m_pRQueue->pop();
				return true;
			}
		}
		swapQueue();
		{
			Guard rGuard(&m_rMutex);
			if (!m_pRQueue->empty())
			{
				item = m_pRQueue->front();
				m_pRQueue->pop();
				return true;
			}
		}

		return false;
	}

	void clear()
	{
		Guard rGuard(&m_rMutex);
		Guard wGuard(&m_wMutex);
		while (!m_pRQueue->empty())
		{
			type& item = m_pRQueue->front();
			deleteItem(item);
			m_pRQueue->pop();
		}
		while (!m_pWQueue->empty())
		{
			type& item = m_pWQueue->front();
			deleteItem(item);
			m_pWQueue->pop();
		}
	}
protected:
	virtual void	deleteItem(type& item) = 0;
	void	swapQueue()
	{
		Guard rGuard(&m_rMutex);
		Guard wGuard(&m_wMutex);

		PContainer tmp = m_pRQueue;
		m_pRQueue = m_pWQueue;
		m_pWQueue = tmp;
	}
protected:
	typedef std::queue<type>*	PContainer;
	PContainer			m_pRQueue;
	PContainer			m_pWQueue;
	Mutex				m_rMutex;
	Mutex				m_wMutex;
	
};
END_NAMESPACE