/**********************************************************************************
**  
**
***********************************************************************************/

#ifndef __2014_10_20_LOGIC_SERVER_H__
#define __2014_10_20_LOGIC_SERVER_H__

#include "object.h"
#include "message.h"
#include "mutex.h"
#include <string>

BEGIN_NAMESPACE

#define LOGIC_SERVER_CALLBACK_PARAMS	uint32_t session, uint32_t source, Message* msg
typedef void* (*SERVER_CALLBACK)(LOGIC_SERVER_CALLBACK_PARAMS);

class LogicServer : public Mutex, public Ref
{
public:
	LogicServer();
	virtual ~LogicServer();

	uint32_t		get_server_id() { return m_serer_id;  }
	int32_t			set_server_name(std::string server_name) { m_server_name = server_name; }
	std::string		get_server_name() { return m_server_name; }

	virtual int32_t	init_server(uint32_t server_id, SERVER_CALLBACK callback);
	virtual int32_t destroy_server();
	virtual int32_t server_callback(LOGIC_SERVER_CALLBACK_PARAMS);

protected:
	uint32_t			m_serer_id;			//服务ID，
	SERVER_CALLBACK		m_pfnCallback;		//服务的回调函数
	std::string			m_server_name;		//服务的名字
	MessageQueue		m_msg_queue;
};

END_NAMESPACE
#endif//__2014_10_20_LOGIC_SERVER_H__