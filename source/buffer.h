/********************************************************************************
*	文件名称:	buffer.h														*
*	创建时间：	2014/07/15														*
*	作   者 :	xzben															*
*	文件功能:	系统中使用的buffer类型											*
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