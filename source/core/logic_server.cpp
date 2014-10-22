#include "logic_server.h"

BEGIN_NAMESPACE

LogicServer::LogicServer()
{
		
}

LogicServer::~LogicServer()
{
	destroy_server();
}


int32_t	LogicServer::init_server(uint32_t server_id, SERVER_CALLBACK callback)
{
	m_serer_id = server_id;
	m_pfnCallback = callback;

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
	}

	return 0;
}

END_NAMESPACE