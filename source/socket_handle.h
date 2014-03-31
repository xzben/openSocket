/********************************************************************
*	文件名称:	socket_handle.h										*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	包含最基本socket封装									*
*********************************************************************/

#include"config.h"

#ifndef __2014_03_17_SOCKET_HANDLE_H__
#define __2014_03_17_SOCKET_HANDLE_H__

//SOCKET 句柄类型
#if defined(__PLATFORM_WIN32__)
	#include"winsock2.h"
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment ( lib, "mswsock.lib")
	typedef SOCKET					SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	INVALID_SOCKET
#elif define(__PLATFORM_LINUX__)
	typedef int				SOCKET_HANDLE;
	#define	INVALID_SOCKET_HANDLE	-1
#endif//平台相关

#endif//__2014_03_17_SOCKET_HANDLE_H__