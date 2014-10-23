/**********************************************************************************
**
**
***********************************************************************************/

#ifndef __2014_10_20_LOCAL_SERVER_MANAGER_H__
#define __2014_10_20_LOCAL_SERVER_MANAGER_H__

#include "logic_server.h"
#include <map>
#include "mutex.h"

BEGIN_NAMESPACE

class LocalServerManager
{
public:
	static LocalServerManager* get_instance()
	{
		static LocalServerManager s_instance;

		return &s_instance;
	}

	int32_t init(uint8_t node_id, std::string node_name);
	int32_t destroy();
	/*
	* 创建一个服务，并返回这个服务的ID
	*/
	LogicServer* create_server(SERVER_CALLBACK callback);
	/*
	* 销毁指定ID的服务
	*/
	int32_t destroy_server(uint32_t server_id);

	LogicServer* get_server(uint32_t server_id);
	bool		is_local_server(uint32_t server_id);
protected:
	friend class LogicServer;
	int32_t push_server_to_map(uint32_t server_id, LogicServer* server);
	void	delete_server(uint32_t server_id);
protected:
	LocalServerManager();
	~LocalServerManager();

private:
	Mutex		m_lock;
	uint32_t	m_global_server_count;
	uint32_t	m_local_server_count;
	uint8_t		m_node_id;
	std::string m_node_name;
	typedef std::map<uint32_t, LogicServer*>	_ContainorId2Server;
	_ContainorId2Server		m_id_map_servers;
};

END_NAMESPACE
#endif//__2014_10_20_LOCAL_SERVER_MANAGER_H__