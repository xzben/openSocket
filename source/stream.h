/********************************************************************************
*	�ļ�����:	Stream.h														*
*	����ʱ�䣺	2014/03/18														*
*	��   �� :	xzben															*
*	�ļ�����:																	*
*********************************************************************************/

#ifndef __2014_03_22_STREAM_H__
#define __2014_03_22_STREAM_H__

#include "socket.h"

BEGIN_NAMESPACE

class EXP_IMP_DLL Stream
{
public:
	Stream();
	virtual ~Stream();

	Socket	getSocket();
private:
	Socket	m_sock;
};

END_NAMESPACE
#endif//__2014_03_22_STREAM_H__