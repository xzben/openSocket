#include <socket.h>
#include <iostream>
USING_NAMESPACE


int main()
{
	Socket sock;
	sock.open(Socket::SOCK_TCP);

	if (sock.connect(InterAddress(6000, "127.0.0.1")))
	{
		char buf[100] = "hello world!";
		if (SOCKET_ERROR != sock.send(buf, strlen(buf)))
			printf("send successed!\n");

		char recvbuf[100];
		memset(recvbuf, 0, sizeof(recvbuf));
		if (SOCKET_ERROR != sock.recv(recvbuf, 100))
			printf("recv: %s", recvbuf);
	}
	sock.close();
	return 0;
}