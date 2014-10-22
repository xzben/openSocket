/**********************************************************************************
**
**
***********************************************************************************/

#ifndef __2014_10_20_MESSAGE_H__
#define __2014_10_20_MESSAGE_H__

#include "object.h"
#include "queue"
#include "mutex.h"

BEGIN_NAMESPACE
enum MsgType : uint8
{

};

class Message
{
	
public:
	Message();
	~Message();

private:
	MsgType	  m_msg_type;
};


class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

	Message* pop_msg();
	//判断当前队列状态是否是自由状态
	bool	 is_free_status();
	//将队列状态改为放入MessageManager中的状态
	void	 set_to_mamanger();
	//将队列状态改为自由状态
	void	 set_free();
protected:
	int32_t push_msg(Message* msg);
private:
	typedef std::queue<Message*> _Container;
	_Container		m_msgs;
	uint32_t		m_manager_flag;
};

class MessageManager
{
public:
	MessageManager* get_instance()
	{
		static MessageManager s_instance;
		return &s_instance;
	}

	MessageQueue* pop_queue();
	int32_t		  push_queue(MessageQueue* queue);
protected:
	MessageManager();
	~MessageManager();
private:
	typedef std::queue<MessageQueue*> _Container;
	_Container		m_msg_queues;
	Mutex			m_lock;
};
END_NAMESPACE

#endif//__2014_10_20_MESSAGE_H__