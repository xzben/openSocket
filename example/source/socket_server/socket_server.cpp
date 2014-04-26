#include <socket.h>
#include <iostream>
USING_NAMESPACE

int main()
{
	Socket sock(Socket::SOCK_TCP);
	VERIFY( sock.bind(InterAddress(6000)) );
	VERIFY( sock.listen() );

	while (1)
	{
		Socket sockCon;
		InterAddress addrCon;
		char   buf[100];
		if (sock.accept(sockCon, &addrCon))
		{
			char szIp[20]; int16 nPort;
			int nDataSize = 0;
			addrCon.getAddress(szIp, nPort);

			printf("connect client ip[%s] port[%d]\n", szIp, nPort);
			if ((nDataSize = sockCon.recv(buf, 100)) > 0)
			{
				buf[nDataSize] = '\0';
				printf("recv from [%s:%d] content : %s\n", szIp, nPort, buf);

				sockCon.send(buf, nDataSize);
			}
		}
		sockCon.close();
	}

	return 0;
}