#include <iostream>
#include <socket_addr.h>
#include <acceptor.h>
#include <stream.h>
#include <time_value.h>
USING_NAMESPACE

int main()
{
	Acceptor acceptor(InterAddress(6000, "127.0.0.1"));

	while (1)
	{
		Stream newStream;
		char   buf[100];
		int nDataSize = 0;

		if (acceptor.accept(TimeValue(1), newStream))
		{
			char szIp[20]; int16 nPort;
			newStream.getRemoteAddress(szIp, nPort);

			printf("connected from [%s:%d]\n", szIp, nPort);

			if ((nDataSize = newStream.recv(buf, 100)) > 0)
			{
				buf[nDataSize] = '\0';
				printf("recv from [%s:%d] content : %s\n", szIp, nPort, buf);

				newStream.send(buf, nDataSize);
			}
		}
		else
		{
			printf("acceptor time out!!!!\n");
		}
	}

	return 0;
}
