#ifndef __2014_10_09_CONFIG_FILE_H__
#define __2014_10_09_CONFIG_FILE_H__

#include "config.h"
#include "mutex.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <string>
#include <map>

BEGIN_NAMESPACE

class LuaConfigFile
{
public:
	static LuaConfigFile* getInstance();
	int32			init(const char* file);
	int32			getValue(const char* key, int32 default_value);
	std::string		getValue(const char* key, const char*  default_value);
	std::string		setValue(const char* key, const char* szValue);
	int32			setValue(const char* key, int32 szValue);
	
	//此接口获得参数时是必须要获得到配置的，如果获得不到则必定报错
	int32			getInt(const char* key);
	std::string		getString(const char* key);

protected:
	LuaConfigFile();
	~LuaConfigFile();

	typedef std::map<std::string, std::string>	MapValue;
	MapValue		m_values;
	Mutex			m_mutex;
};
END_NAMESPACE
#endif//__2014_10_09_CONFIG_FILE_H__