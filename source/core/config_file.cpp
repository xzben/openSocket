#include "config_file.h"
#include "logger.h"
BEGIN_NAMESPACE


LuaConfigFile* LuaConfigFile::getInstance()
{
	static LuaConfigFile s_instnace;

	return &s_instnace;
}

LuaConfigFile::LuaConfigFile()
{
	
}

LuaConfigFile::~LuaConfigFile()
{
	
}

int32 LuaConfigFile::init(const char* filename)
{	
	m_mutex.lock();
	m_values.clear();
	m_mutex.unlock();

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//lua 加载配置文件脚本
	//此脚本，会将 配置文件中 $开头的字符串替换成系统环境变量对应的值
	static const char * load_config = "\
	local config_name = ...\
	local f = assert(io.open(config_name))\
	local code = assert(f:read \'*a\')\
	local function getenv(name) return assert(os.getenv(name), name) end\
	code = string.gsub(code, \'%$([%w_%d]+)\', getenv)\
	f:close()\
	local result = {}\
	assert(load(code,\'=(load)\',\'t\',result))()\
	return result\
	";
	int err = luaL_loadstring(L, load_config);
	assert(err == LUA_OK);
	lua_pushstring(L, filename);
	err = lua_pcall(L, 1, 1, 0);
	if (err) {
		fprintf(stderr, "%s\n", lua_tostring(L, -1));
		lua_close(L);
		return 1;
	}

	// 遍历返回的result table中的值
	lua_pushnil(L);
	while (lua_next(L, -2) != 0)
	{
		int keyt = lua_type(L, -2);
		if (keyt != LUA_TSTRING) 
		{
			fprintf(stderr, "Invalid config table [ %d ]\n", keyt);
			lua_close(L);
			exit(1);
		}

		const char * key = lua_tostring(L, -2);
		if (lua_type(L, -1) == LUA_TBOOLEAN)
		{
			int b = lua_toboolean(L, -1);
			setValue(key, b ? "true" : "false");
		}
		else 
		{
			const char * value = lua_tostring(L, -1);
			if (value == NULL)
			{
				fprintf(stderr, "Invalid config table key = %s\n", key);
				lua_close(L);
				exit(1);
			}
			setValue(key, value);
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);

	lua_close(L);
	printf("config loading done!!!!!!!!!!!!\n");

	return 0;
}

int32 LuaConfigFile::getValue(const char* key, int32 default_value)
{
	Guard guard(&m_mutex);
	MapValue::iterator it = m_values.find(key);
	if (it == m_values.end())
	{
		return setValue(key, default_value);
	}
	return atoi(it->second.c_str());
}

std::string LuaConfigFile::getValue(const char* key, const char* default_value)
{
	Guard guard(&m_mutex);
	MapValue::iterator it = m_values.find(key);
	if (it == m_values.end())
	{
		return setValue(key, default_value);
	}
	return it->second;
}

int32	LuaConfigFile::getInt(const char* key)
{
	Guard guard(&m_mutex);
	MapValue::iterator it = m_values.find(key);
	if (it == m_values.end())
	{
		Logger::getInstace()->error("try get config key [%s] not exist", key);
		return 0;
	}

	return atoi(it->second.c_str());
}
std::string		LuaConfigFile::getString(const char* key)
{
	Guard guard(&m_mutex);
	MapValue::iterator it = m_values.find(key);
	if (it == m_values.end())
	{
		Logger::getInstace()->error("try get config key [%s] not exist", key);
		return "";
	}

	return it->second;
}

std::string LuaConfigFile::setValue(const char* key, const char* Value)
{
	if (Value == nullptr) 
		Value = "";

	Guard guard(&m_mutex);
	m_values[key] = Value;
	printf("setValue[%s] = %s\n", key, Value);

	return Value;
}

int32 LuaConfigFile::setValue(const char* key, int32 Value)
{
	char szValue[20];
	_itoa(Value, szValue, 10);

	Guard guard(&m_mutex);
	m_values[key] = szValue;

	printf("setValue[%s] = %s\n", key, szValue);

	return Value;
}

END_NAMESPACE