/********************************************************************************
*	�ļ�����:	buffer.h														*
*	����ʱ�䣺	2014/07/15														*
*	��   �� :	xzben															*
*	�ļ�����:	ϵͳ��ʹ�õ�buffer����											*
*********************************************************************************/

#include "object.h"
#include "rw_queue.h"

BEGIN_NAMESPACE

struct BufferData
{
	BufferData(void* data, int32 len)
	{
		_data = data;
		_len  = len;
	}
	BufferData()
	{
		_data = nullptr;
		_len = 0;
	}
	void*		_data;
	int32		_len;
};

class SendBuffer : public RWQueue<BufferData>
{
public:
	SendBuffer();
	~SendBuffer();

	bool pushData(void* data, int32 len);
	bool popData(void*& rpData, int32& len);
protected:
	void deleteItem(BufferData& item) override;
};
END_NAMESPACE