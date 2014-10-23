/**********************************************************************************
**
**
***********************************************************************************/

#ifndef __2014_10_20_MESSAGE_H__
#define __2014_10_20_MESSAGE_H__

#include "object.h"
#include <queue>
#include <map>
#include "mutex.h"

BEGIN_NAMESPACE
enum MsgType : uint8
{
	MT_TIMEOUT = 1,  // ��ʱ���ĵ�ʱ��Ϣ
	MT_CB_RT   = 2,  // ����ص�����ֵ
};

class Message
{
public:
	Message(uint32_t session, uint32_t source, MsgType type, void* pData);
	~Message();

	MsgType	  m_msg_type;
	uint32_t  m_session;
	uint32_t  m_source;
	void*	  m_pData;
};

class MessageFactory
{
public:
	static MessageFactory* get_instance()
	{
		static MessageFactory s_instance;
		return &s_instance;
	}

	Message* create_callback_ret(uint32_t session, uint32_t source, void* data);

	void  destory_message(Message* msg);
protected:
	MessageFactory();
	~MessageFactory();
};

class MessageQueue
{
public:
	MessageQueue(uint32_t server_id);
	~MessageQueue();

	Message* pop_msg();
	//�жϵ�ǰ����״̬�Ƿ�������״̬
	bool	 is_free_status();
	uint32_t get_server_id() { return m_server_id; }

protected:
	friend class MessageManager;
	int32_t		push_msg(Message* msg);
	void		set_to_mamanger();		//������״̬��Ϊ����MessageManager�е�״̬
	void		set_free();				//������״̬��Ϊ����״̬
	uint32_t	dump_w2r_queue();		//��д�����е���Ϣ�����������
	void		clear();
private:
	typedef std::queue<Message*> _Container;
	uint32_t		m_server_id;
	_Container		m_r_queue;
	_Container		m_w_queue;
	uint32_t		m_manager_flag;
	Mutex			m_wlock;
};

class MessageManager
{
public:
	static MessageManager* get_instance()
	{
		static MessageManager s_instance;
		return &s_instance;
	}

	MessageQueue* pop_queue();
	int32_t		  push_queue(MessageQueue* queue);
	int32_t       push_msg(uint32_t server_id, Message* msg);
	MessageQueue* create_message_queue( uint32_t server_id);
	int32_t		  execute_messages(int weight);
protected:
	friend class MessageQueue;
	void		  delete_message_queue(uint32_t server_id);
	MessageManager();
	~MessageManager();
private:
	typedef std::queue<MessageQueue*> _Container;
	typedef std::map<uint32_t, MessageQueue*> _ContainerServer2Queue;
	_Container					m_msg_queues;
	Mutex						m_msg_queues_lock;
	_ContainerServer2Queue		m_server_map_queue;
	Mutex						m_server_map_queue_lock;
};

END_NAMESPACE

#endif//__2014_10_20_MESSAGE_H__