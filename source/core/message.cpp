#include "message.h"
#include "atom.h"
#include "local_server_manager.h"

BEGIN_NAMESPACE

Message::Message(uint32_t session, uint32_t source, MsgType type, void* pData)
{
	m_session = session;
	m_source = source;
	m_msg_type = type;
	m_pData = pData;
}

Message::~Message()
{
	SAFE_DELETE(m_pData);
}
//////////////////////////////////////////////////////////////////////////
MessageFactory::MessageFactory()
{

}

MessageFactory::~MessageFactory()
{

}

Message* MessageFactory::create_callback_ret(uint32_t session, uint32_t source, void* data)
{
	Message* pRet = new Message(session, source, MT_CB_RT, data);

	return pRet;
}

void  MessageFactory::destory_message(Message* msg)
{
	SAFE_DELETE(msg);
}
//////////////////////////////////////////////////////////////////////////
MessageQueue::MessageQueue(uint32_t server_id)
{
	m_server_id = server_id;
}

MessageQueue::~MessageQueue()
{
	MessageManager::get_instance()->delete_message_queue(m_server_id);
	clear();
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

void	MessageQueue::clear()
{
	this->dump_w2r_queue();
	while (!m_r_queue.empty())
	{
		Message *tmp = m_r_queue.front();
		m_r_queue.pop();
		MessageFactory::get_instance()->destory_message(tmp);
	}

}

uint32_t MessageQueue::dump_w2r_queue()
{
	uint32_t len = 0;

	m_wlock.lock();
	while (!m_w_queue.empty())
	{
		len++;
		Message* tmp = m_w_queue.front();
		m_w_queue.pop();
		m_r_queue.push(tmp);
	}
	m_wlock.unlock();

	return len;
}

Message* MessageQueue::pop_msg()
{
	Message* ret = nullptr;
	if (!m_r_queue.empty())
	{
		ret = m_r_queue.front();
		m_r_queue.pop();
	}

	return ret;
}

int32_t MessageQueue::push_msg(Message* msg)
{
	if (msg == nullptr)
		return 1;

	m_wlock.lock();
	m_w_queue.push(msg);
	m_wlock.unlock();
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
	m_msg_queues_lock.lock();
	if (!m_msg_queues.empty())
	{
		ret = m_msg_queues.front();
		m_msg_queues.pop();
	}
	m_msg_queues_lock.unlock();

	if (ret != nullptr)
		ret->set_free();

	return ret;
}

int32_t MessageManager::push_queue(MessageQueue* queue)
{
	if (nullptr == queue)
		return 1;

	m_msg_queues_lock.lock();
	m_msg_queues.push(queue);
	m_msg_queues_lock.unlock();

	queue->set_to_mamanger();
	return 0;
}

int32_t MessageManager::push_msg(uint32_t server_id, Message* msg)
{
	MessageQueue *server_queue = nullptr;

	m_server_map_queue_lock.lock();
	_ContainerServer2Queue::iterator it = m_server_map_queue.find(server_id);
	if (it != m_server_map_queue.end())
		server_queue = it->second;
	m_server_map_queue_lock.unlock();

	if (nullptr == server_queue)
		return 1;

	server_queue->push_msg(msg);
	if (server_queue->is_free_status())
	{
		this->push_queue(server_queue);
	}
	return 0;
}

MessageQueue* MessageManager::create_message_queue(uint32_t server_id)
{
	MessageQueue* queue = new MessageQueue(server_id);
	if (nullptr != queue)
	{
		m_server_map_queue_lock.lock();
		m_server_map_queue.insert(_ContainerServer2Queue::value_type(server_id, queue));
		m_server_map_queue_lock.unlock();
	}
	return queue;
}

void MessageManager::delete_message_queue(uint32_t server_id)
{
	m_server_map_queue_lock.lock();
	_ContainerServer2Queue::iterator it = m_server_map_queue.find(server_id);
	if (it != m_server_map_queue.end())
	{
		MessageQueue *queue = it->second;
		m_server_map_queue.erase(it);
		SAFE_DELETE(queue);
	}
	m_server_map_queue_lock.unlock();
}

int32_t MessageManager::execute_messages(int weight)
{
	MessageQueue *queue = this->pop_queue();
	if (nullptr == queue)
		return 1;
	
	uint32_t server_id = queue->get_server_id();
	LogicServer* server = LocalServerManager::get_instance()->get_server(server_id);
	if (nullptr == server)
	{
		MessageManager::get_instance()->delete_message_queue(server_id);
	}
	uint32_t queue_size = queue->dump_w2r_queue();
	
	uint32_t n = 1;
	for (int i = 0; i < n; i++)
	{
		if (weight >= 0)
			n = queue_size >> weight;
		else
			n = queue_size;

		Message* msg = queue->pop_msg();
		if (msg == nullptr)
			continue;

		server->server_callback(msg->m_session, msg->m_source, msg);
		MessageFactory::get_instance()->destory_message(msg);
	}


}
END_NAMESPACE