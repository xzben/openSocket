/********************************************************************
*	�ļ�����:	socket_handle.h										*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	���������socket��װ									*
*********************************************************************/

#include"config.h"

#ifndef __2014_03_17_SOCKET_HANDLE_H__
#define __2014_03_17_SOCKET_HANDLE_H__

//SOCKET �������
#if defined(__PLATFORM_WIN32__)
	#include"winsock2.h"
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment ( lib, "mswsock.lib")
	typedef SOCKET					SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	INVALID_SOCKET
#elif define(__PLATFORM_LINUX__)
	typedef int				SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	-1
#endif//ƽ̨���

#endif//__2014_03_17_SOCKET_HANDLE_H__