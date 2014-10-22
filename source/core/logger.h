/********************************************************************************
*	�ļ�����:	logger.h														*
*	����ʱ�䣺	2014/06/05														*
*	��   �� :	xzben															*
*	�ļ�����:	һ��TCP����ͨ����,����TCP���Ӷ˵�����ͨ�Ŷ���ͨ�������������		*
*				�����������ACE													*
*********************************************************************************/

#ifndef __2014_03_02_CLOGGER_H__
#define __2014_03_02_CLOGGER_H__

#include "object.h"
#include "mutex.h"
#include <string>
BEGIN_NAMESPACE
/*
*	ϵͳ��־������Ϊ��λ�洢��־�ļ�
*	����������־��ʾ�ȼ�, ����ȼ���Ϣ�����е� LOG_LEVEL ö��
*/
class EXP_IMP_DLL Logger : public NoCopyAble, public Object
{
public:
	/*
	*	��־�ȼ�, ͨ����־�ȼ�����������־����ʾ����Ϣ
	*/
	typedef enum
	{
		/*
		*	�ر���־���ܣ���������ʾ�κ���Ϣ
		*/
		LEVEL_OFF = INT_MAX,
		/*
		*	����ΪLEVEL_FATALʱ����ʾ���ڴ˵ȼ�����Ϣ
		*	��������: ��������ǽ�����ϵͳ������
		*/
		LEVEL_FATAL = 500000,
		/*
		*	����ΪLEVEL_ERRORʱ����ʾ�ȼ����ڵ��ڴ˵ȼ�����Ϣ
		*	����:	����Ϊϵͳ��һЩ���󣬵���ϵͳ������������
		*/
		LEVEL_ERROR = 400000,		   //	����ȼ�����ʾ
		/*
		*	����ΪLEVEL_WARNʱ����ʾ�ȼ����ڵ��ڴ˵ȼ�����Ϣ
		*	����: ϵͳ��һЩ��Ҫע��ĵط�, ���ǻ������ڴ��󣬵��ǿ����Ǻ����������ĸ�Դ
		*/
		LEVEL_WARN  = 300000,
		/*
		*	����ΪLEVEL_INFOʱ����ʾ�ȼ����ڵ��ڴ˵ȼ�����Ϣ
		*	��Ϣ: ϵͳ��һЩ��һЩ������Ϣ�����ڸ��ٷ�������״̬
		*/
		LEVEL_INFO  = 20000,
		/*
		*	����ΪLEVEL_DEBUGʱ����ʾȫ������Ϣ
		*/
		LEVEL_DEBUG = 10000,
		/*
		*	����ΪLEVEL_ALLʱ����ʾȫ������Ϣ
		*/
		LEVEL_ALL = INT_MIN, 
	}LOG_LEVEL;
	
	~Logger(); 
	static Logger* getInstace()
	{
		static Logger instance;
		return &instance;
	}
	void removeConsoleLog();
	void addLocalFileLog(const std::string &file);

	void setLevel(const LOG_LEVEL level);
	void setLevel(const std::string &level);

	void logtext(const LOG_LEVEL level,const char * text);
	void logva(const LOG_LEVEL level,const char * pattern,va_list vp);
	void log(const LOG_LEVEL level,const char * pattern,...);

	void debug(const char * pattern,...);
	void debug16(const char* info, const BYTE* pData, int Datasize);
	void error(const char * pattern,...);
	void info(const char * pattern,...);
	void fatal(const char * pattern,...);
	void warn(const char * pattern,...);
	void setLoggerName(std::string strLogNmae){ m_strLogName = strLogNmae; }
protected:
	Logger(std::string strLogName = "LOGGER", LOG_LEVEL emLevel= LEVEL_ERROR);
private:
	Mutex			m_mutex;
	LOG_LEVEL		m_emLevel;
	std::string		m_strLogName;
	std::string		m_file;
	FILE			*m_fp_console;
	FILE			*m_fp_file;		
	int				m_day;			//��ǰ����־��������ģ�ÿ��һ����־�ı�
};

END_NAMESPACE
#endif//__2014_03_02_CLOGGER_H__