#include "openSocket.h"
#include "config_file.h"
#include "thread.h"
#include "condition.h"
#include "logger.h"
#include "timer.h"
#include "message.h"
BEGIN_NAMESPACE

struct WorkerParam
{
	int ID;
	int weight;
};

bool openSocket::check_abort()
{
	return false;
}

Condition	openSocket::s_thread_start_condition;

void openSocket::_minitor(void* param)
{
	s_thread_start_condition.wait();
	Logger::getInstace()->info("this is mintor thread start...............");
	while (1)
	{
		if (check_abort()) break;
	}
}

void openSocket::_timer(void* param)
{
	s_thread_start_condition.wait();
	Logger::getInstace()->info("this is timer thread start...............");
	while (1)
	{
		Timer::getInstance()->updateTimer();
		if (check_abort()) break;
		
	}
}

void openSocket::_socket(void* param)
{
	s_thread_start_condition.wait();
	Logger::getInstace()->info("this is socket thread start...............");
	while (1)
	{
		if (check_abort()) break;
	}
}

void openSocket::_worker(void* param)
{
	WorkerParam* worker_param = (WorkerParam*)param;
	int worker_id = worker_param->ID;
	int weight = worker_param->weight;

	s_thread_start_condition.wait();
	Logger::getInstace()->info("this is worker thread[%d] start...............", worker_id);

	while (1)
	{
		int32_t idle = MessageManager::get_instance()->execute_messages(weight);
		if (idle)
		{

		}
		if (check_abort()) break;
	}
}

openSocket::openSocket()
{

}

openSocket::~openSocket()
{

}
int openSocket::start()
{
	int		worker_num = LuaConfigFile::getInstance()->getInt("thread");
	int	    thread_nums = 3 + worker_num;
	Thread *threads = new Thread[thread_nums];
	Logger::getInstace()->info("open socket start...............");
	threads[0].open(openSocket::_minitor, nullptr);
	threads[1].open(openSocket::_timer, nullptr);
	threads[2].open(openSocket::_socket, nullptr);

	WorkerParam *param = new WorkerParam[worker_num];
	static int weight[] = {
		-1, -1, -1, -1, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, };
	for (int i = 0; i < worker_num; i++)
	{
		param[i].ID = i;
		if (i < sizeof(weight) / sizeof(weight[0]))
			param[i].weight = weight[i];
		else
			param[i].weight = 0;

		
		threads[3 + i].open(openSocket::_worker, &param[i]);
	}

	s_thread_start_condition.notify_all();

	for (int i = 0; i < thread_nums; i++)
		threads[i].join();

	delete[] param;
	return 0;
}


END_NAMESPACE