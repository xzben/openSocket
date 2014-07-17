#include <thread.h>
#include <mutex.h>
#include <iostream>
#include <condition.h>
using namespace std;
USING_NAMESPACE;

int gCount = 0;
Mutex mut;
Condition cond;

void func(void *param)
{
	cond.wait();
	int id = *((int*)param);
	while (1)
	{
		Guard guard(&mut);
		gCount = gCount + 1;
		if (gCount > 100) break;
		printf("thread function ---%d---[%d]\n", id, gCount);
	}
}


int main()
{
	Thread thread1, thread2;
	int32 id1 = 1;
	int32 id2 = 2;

	thread1.open(func, &id1);
	thread2.open(func, &id2);

	
	cond.notify_all();  //开启线程的运行 // cond.notify_one(); cond.notify_one();
	thread2.join();
	thread1.join();

	
	printf("game over!!");
}