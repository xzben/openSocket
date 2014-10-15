#include <iostream>
#include "config_file.h"
#include "logger.h"
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

	// 尝试获得指定的配置参数，如果不存在则会设置缺省参数
	LuaConfigFile::getInstance()->getValue("thread", 8);
	LuaConfigFile::getInstance()->getValue("logger", "");
	LuaConfigFile::getInstance()->getValue("logger_level", "all");
	
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
	if (init_config(argc, argv))
		return 1;

	if (init_logger())
		return 1;

	getchar();
	return 0;
}