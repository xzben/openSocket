#include "message.h"
#include "atom.h"

BEGIN_NAMESPACE

Message::Message()
{

}

Message::~Message()
{

}

//////////////////////////////////////////////////////////////////////////
MessageQueue::MessageQueue()
{

}

MessageQueue::~MessageQueue()
{

}

bool MessageQueue::is_free_status()
{
	uint32_t status = AtomGet(&m_manager_flag);

	return (status == 0);
}

void MessageQueue::set_to_mamanger()
{
	AtomSet(&m_manager_flag, 1);
}

void MessageQueue::set_free()
{
	AtomSet(&m_manager_flag, 0);
}

Message* MessageQueue::pop_msg()
{
	Message* ret = nullptr;
	if (!m_msgs.empty())
	{
		ret = m_msgs.front();
		m_msgs.pop();
	}

	return ret;
}

int32_t MessageQueue::push_msg(Message* msg)
{
	if (msg == nullptr)
		return 1;

	m_msgs.push(msg);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

MessageManager::MessageManager()
{

}

MessageManager::~MessageManager()
{

}

MessageQueue* MessageManager::pop_queue()
{
	MessageQueue *ret = nullptr;
	m_lock.lock();
	if (!m_msg_queues.empty())
	{
		ret = m_msg_queues.front();
		m_msg_queues.pop();
	}
	m_lock.unlock();

	if (ret != nullptr)
		ret->set_free();

	return ret;
}

int32_t MessageManager::push_queue(MessageQueue* queue)
{
	if (nullptr == queue)
		return 1;

	m_lock.lock();
	m_msg_queues.push(queue);
	m_lock.unlock();

	queue->set_to_mamanger();
	return 0;
}


END_NAMESPACE