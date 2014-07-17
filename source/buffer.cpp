#include "buffer.h"

BEGIN_NAMESPACE

SendBuffer::SendBuffer()
{

}

SendBuffer::~SendBuffer()
{

}

bool SendBuffer::pushData(void* data, int32 len)
{
	this->push(BufferData(data, len));
}

bool SendBuffer::popData(void*& rpData, int32& len)
{
	BufferData bufData;
	if (!this->pop(bufData))
		return false;

	rpData = bufData._data;
	len = bufData._len;
	return true;
}

void SendBuffer::deleteItem(BufferData& item)
{
	SAFE_DELETE(item._data);
}

END_NAMESPACE