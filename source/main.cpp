#include <iostream>
#include "socket.h"
using namespace std;
USING_NAMESPACE

int32 main()
{
	Socket sock;
	sock.open(Socket::SOCK_TCP);

	if (sock.connect(InterAddress(6000, "127.0.0.1")))
	{
		char buf[100] = "hello world!";
		if( SOCKET_ERROR != sock.send(buf, strlen(buf) + 1) )
			printf("send successed!\n");

		char recvbuf[100];
		memset(recvbuf, 0, sizeof(recvbuf));
		if (SOCKET_ERROR != sock.recv(recvbuf, 100))
			printf("recv: %s", recvbuf);
	}
	


	return 0;
}