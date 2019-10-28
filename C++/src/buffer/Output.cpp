#include "Output.h"

namespace Empire {

void BufferedOutput::Resize(u64 newCapacity)
{
	u8* newBuffer = new u8[newCapacity];
	memcpy(newBuffer, m_Buffer, m_Offset);
	m_Capacity = newCapacity;
	delete[] m_Buffer;
	m_Buffer = newBuffer;
}

void BufferedOutput::EnsureCapacity(u64 bytes)
{
	u64 newCapacity = m_Capacity;
	u64 required = bytes + m_Offset;
	if (required > m_Capacity) {
		while (newCapacity < required) {
			newCapacity <<= 1;
		}
		Resize(newCapacity);
	}
}


}//namespace
