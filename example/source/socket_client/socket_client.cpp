#include <iostream>
#include <stream.h>
#include <connector.h>
#include <memory.h>
#include <socket_option.h>

using namespace std;
USING_NAMESPACE


int main()
{
	Stream stream;
	Connector connector;
	
	if( connector.connect(stream, InterAddress(6000, "127.0.0.1"), TimeValue(1)) )
	{
		char ip[20]; int16 port;
		stream.getLocalAddress(ip, port);
		printf("local addr:[%s][%d]\n", ip, port);			
		char buf[100] = "hello world!";
		if( SOCKET_ERROR != stream.send(buf, strlen(buf)) )
			printf("send successed!\n");

		char recvbuf[100];
		memset(recvbuf, 0, sizeof(recvbuf));
		if (SOCKET_ERROR != stream.recv(recvbuf, 100))
			printf("recv: %s", recvbuf);
	}
	else
	{
		printf("Connector failed!!");
	}

	getchar();
	return 0;
}
