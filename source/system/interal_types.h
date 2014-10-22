/********************************************************************
*	�ļ�����:	interal_types.h										*
*	����ʱ�䣺	2014/03/17											*
*	��   �� :	xzben												*
*	�ļ�����:	���ļ���ҪΪ��ƽ̨��չʱ�ܹ����ݱ�֤����һ����			*
*				����������ͳһ�ض���									*
*********************************************************************/

#ifndef __2014_03_17_INTERAL_TYPES_H__
#define __2014_03_17_INTERAL_TYPES_H__

#include <cstdint>

//�ض��������������
typedef	char						int8;
typedef unsigned char				uint8;
typedef short						int16;
typedef unsigned short				uint16;
typedef int							int32;
typedef unsigned int				uint32;
typedef long long					int64;
typedef	unsigned long long			uint64;
typedef uint8						BYTE;

#if defined(_LINUX) //window���ж��嵫��linux��û�е�����
	typedef uint32					DWORD;
#elif defined(_WIN32) //linux���ж��嵫��window��û�е�����
	typedef int32					socklen_t;
#endif


#endif //__2014_03_17_INTERAL_TYPES_H__

