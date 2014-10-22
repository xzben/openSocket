/**********************************************************************************
**
**
***********************************************************************************/

#ifndef __2014_10_20_OPENSOCKET_START_H__
#define __2014_10_20_OPENSOCKET_START_H__

#include "object.h"
#include "condition.h"

BEGIN_NAMESPACE

class openSocket
{
public:
	static openSocket* get_instance()
	{
		static openSocket s_instance;

		return &s_instance;
	}

	int start();
protected:
	static void _minitor(void* param);
	static void _timer(void* param);
	static void _socket(void* param);
	static void _worker(void* param);
	static inline bool check_abort();
protected:
	static Condition	s_thread_start_condition;
	openSocket();
	~openSocket();
};


END_NAMESPACE
#endif//__2014_10_20_OPENSOCKET_START_H__