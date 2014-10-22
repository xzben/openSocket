#include <iostream>
#include "config_file.h"
#include "logger.h"
#include "local_server_manager.h"
#include "openSocket.h"
using namespace std;
USING_NAMESPACE

int init_config(int argc, char*argv[])
{
	const char* config_file = nullptr;
	if (argc > 1){
		config_file = argv[1];
	}
	else {
		fprintf(stderr, "Need a config file.\n");
		return 1;
	}

	if (LuaConfigFile::getInstance()->init(config_file))
	{
		return 1;
	}

	// ���Ի��ָ�������ò���������������������ȱʡ����
	LuaConfigFile::getInstance()->getValue("thread", 8);
	LuaConfigFile::getInstance()->getValue("logger", "");
	LuaConfigFile::getInstance()->getValue("logger_level", "all");
	LuaConfigFile::getInstance()->getValue("node_id", 1);
	LuaConfigFile::getInstance()->getValue("node_name", "main");
	
	return 0;
}

int init_logger()
{
	std::string logger_file = LuaConfigFile::getInstance()->getString("logger");
	std::string logger_level = LuaConfigFile::getInstance()->getString("logger_level");

	Logger::getInstace()->setLevel(logger_level);
	if (!logger_file.empty())
	{
		Logger::getInstace()->addLocalFileLog(logger_file);
		Logger::getInstace()->debug("set local logger file[%s] level [%s]", logger_file.c_str(), logger_level.c_str());
	}

	return 0;
}

int main(int argc, char* argv[])
{
	//��ʼ�����ñ�
	if (init_config(argc, argv))
		return 1;

	//��ʼ����־
	if (init_logger())
		return 1;

	//��ʼ�����ط��������
	uint8_t node_id = LuaConfigFile::getInstance()->getInt("node_id");
	std::string node_name = LuaConfigFile::getInstance()->getString("node_name");
	if (LocalServerManager::get_instance()->init(node_id, node_name))
	{
		return 1;
	}

	if (openSocket::get_instance()->start())
	{
		return 1;
	}

	if (LocalServerManager::get_instance()->destroy())
	{
		return 1;
	}
	return 0;
}