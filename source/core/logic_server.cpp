#include "logic_server.h"
#include "local_server_manager.h"
BEGIN_NAMESPACE

LogicServer::LogicServer()
{
	m_session_count = 0;
}

LogicServer::~LogicServer()
{
	destroy_server();
	LocalServerManager::get_instance()->delete_server(m_serer_id);
}

int32_t	LogicServer::init_server(uint32_t server_id, SERVER_CALLBACK callback)
{
	m_serer_id = server_id;
	m_pfnCallback = callback;
	m_pMsg_queue = MessageManager::get_instance()->create_message_queue(server_id);
	return 0;
}

int32_t LogicServer::destroy_server()
{
	return 0;
}

int32_t LogicServer::server_callback(LOGIC_SERVER_CALLBACK_PARAMS)
{
	if (m_pfnCallback)
	{
		void* retData = m_pfnCallback(session, source, msg);
		Message* retMsg = MessageFactory::get_instance()->create_callback_ret(m_session_count++, m_serer_id, retData);
		MessageManager::get_instance()->push_msg(m_serer_id, retMsg);
	}

	return 0;
}

END_NAMESPACE