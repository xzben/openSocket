#include "local_server_manager.h"
#include "atom.h"

#define NODE_ID_WIDTH			(8)
#define NODE_SHIFT				(24)
#define NODE_ID_MASK			(((1<<NODE_ID_WIDTH)-1)<<NODE_SHIFT)
#define SERVER_COUNT_MASK		(~NODE_ID_MASK)

BEGIN_NAMESPACE

LocalServerManager::LocalServerManager()
{
	m_local_server_count = 0;
	m_node_id = 0;
	m_node_name = "";
}

LocalServerManager::~LocalServerManager()
{

}

int32_t LocalServerManager::init(uint8_t node_id, std::string node_name)
{
	m_lock.lock();
	m_node_id = node_id;
	m_node_name = node_name;
	m_lock.unlock();

	return 0;
}

int32_t LocalServerManager::destroy()
{
	m_lock.lock();

	_ContainorId2Server::iterator it = m_id_map_servers.begin();
	for (; it != m_id_map_servers.end(); it++)
	{
		it->second->release();
	}
	m_id_map_servers.clear();
	m_lock.unlock();
	return 0;
}

bool	LocalServerManager::is_local_server(uint32_t server_id)
{
	uint8_t node_id = (server_id & NODE_ID_MASK) >> NODE_SHIFT;

	return node_id == m_node_id;
}
/*
* 创建一个服务，并返回这个服务的ID
*/
LogicServer* LocalServerManager::create_server(SERVER_CALLBACK callback)
{
	uint32   cur_server_count = AtomSelfAdd(&m_local_server_count);
	uint32_t server_id = ((m_node_id << NODE_SHIFT) & NODE_ID_MASK) + (cur_server_count&SERVER_COUNT_MASK);
	LogicServer* new_server = new LogicServer;
	if (nullptr == new_server || new_server->init_server(server_id, callback) || push_server_to_map(server_id, new_server))
	{
		SAFE_DELETE(new_server);
		return nullptr;
	}

	return new_server;
}
/*
* 销毁指定ID的服务
*/
int32_t LocalServerManager::destroy_server(uint32_t server_id)
{
	m_lock.lock();
	_ContainorId2Server::iterator it = m_id_map_servers.find(server_id);
	if (it != m_id_map_servers.end())
	{
		it->second->release();
	}
	m_id_map_servers.erase(it);
	m_lock.unlock();

	return 0;
}

int32_t LocalServerManager::push_server_to_map(uint32_t server_id, LogicServer* server)
{
	if (server == nullptr) return 1;

	m_lock.lock();
	m_id_map_servers.insert(_ContainorId2Server::value_type(server_id, server));
	m_lock.unlock();

	return 0;
}
void	LocalServerManager::delete_server(uint32_t server_id)
{
	m_lock.lock();
	_ContainorId2Server::iterator it = m_id_map_servers.find(server_id);
	if (it != m_id_map_servers.end())
	{
		m_id_map_servers.erase(it);
	}
	m_lock.unlock();
}

LogicServer* LocalServerManager::get_server(uint32_t server_id)
{
	LogicServer* ret = nullptr;
	
	m_lock.lock();

	_ContainorId2Server::iterator it = m_id_map_servers.find(server_id);
	if (it != m_id_map_servers.end())
		ret = it->second;

	m_lock.unlock();

	return ret;
}
END_NAMESPACE