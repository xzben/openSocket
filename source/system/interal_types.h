/********************************************************************
*	文件名称:	interal_types.h										*
*	创建时间：	2014/03/17											*
*	作   者 :	xzben												*
*	文件功能:	本文件主要为了平台扩展时能够兼容保证类型一致性			*
*				将内置类型统一重定义									*
*********************************************************************/

#ifndef __2014_03_17_INTERAL_TYPES_H__
#define __2014_03_17_INTERAL_TYPES_H__

#include <cstdint>

//重定义基本数据类型
typedef	char						int8;
typedef unsigned char				uint8;
typedef short						int16;
typedef unsigned short				uint16;
typedef int							int32;
typedef unsigned int				uint32;
typedef long long					int64;
typedef	unsigned long long			uint64;
typedef uint8						BYTE;

#if defined(_LINUX) //window下有定义但是linux下没有的类型
	typedef uint32					DWORD;
#elif defined(_WIN32) //linux下有定义但是window下没有的类型
	typedef int32					socklen_t;
#endif


#endif //__2014_03_17_INTERAL_TYPES_H__

