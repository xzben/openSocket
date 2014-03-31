#include <socket.h>
#include <socket_option.h>
#include <iostream>
USING_NAMESPACE


int main()
{
	SocketOption option;

	Socket tcpSock(Socket::SOCK_TCP);
	Socket udpSock(Socket::SOCK_UDP);

	
	Socket::SockType tcpType = option.getSocketType(tcpSock);
	Socket::SockType udpType = option.getSocketType(udpSock);
	
	ASSERT(tcpType == Socket::SockType::SOCK_TCP);
	ASSERT(udpType == Socket::SockType::SOCK_UDP);

	printf("recv buf size :%d\n", option.getRecvBufSize(tcpSock));
	printf("send buf size : %d\n", option.getSendBufSize(tcpSock));

	option.setRecvBufSize(tcpSock, 1024);
	option.setSendBufSize(tcpSock, 2048);

	printf("recv buf size :%d\n", option.getRecvBufSize(tcpSock));
	printf("send buf size : %d\n", option.getSendBufSize(tcpSock));

	printf("is listening [%s]\n", option.isListening(tcpSock) ? "On" : "Off");

	Socket::SockType type;
	InterAddress local, remote;
	//tcpSock.bind(InterAddress(6001, "192.168.137.1"));
	ASSERT(tcpSock.connect(InterAddress(6000, "127.0.0.1")));

	while (1)
	{
		std::cout << option.getConnectTime(tcpSock) << std::endl;
	}
	option.getSocketInfo(tcpSock, &type, &local, &remote);
	char szIp[20]; int16 nPort;
	ASSERT(type == Socket::SockType::SOCK_TCP);

	local.getAddress(szIp, nPort);
	printf("ip[ %s ] port[ %d ]\n", szIp, nPort);

	remote.getAddress(szIp, nPort);
	printf("ip[ %s ] port[ %d ]\n", szIp, nPort);


	getchar();
	return 0;
}