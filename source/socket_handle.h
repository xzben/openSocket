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
	#include<winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment ( lib, "mswsock.lib")
////////////////////////////////////////////////////////////
//	ͳһ socket shutdown �Ĳ���
	#define	SHUT_RD					SD_RECEIVE
	#define	SHUT_WR					SD_SEND
	#define	SHUT_RDWR				SD_BOTH
////////////////////////////////////////////////////////
	typedef SOCKET					SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	INVALID_SOCKET
#elif defined(__PLATFORM_LINUX__)
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	#include <errno.h>
////////////////////////////////////////////////////////////
//	ͳһ socket shutdown �Ĳ���
	#define	SD_RECEIVE				SHUT_RD					
	#define	SD_SEND					SHUT_WR					
	#define	SD_BOTH					SHUT_RDWR	
////////////////////////////////////////////////////////////
	typedef int						SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	-1
	#define	GetLastError()			 errno
#endif//ƽ̨���

#endif//__2014_03_17_SOCKET_HANDLE_H__