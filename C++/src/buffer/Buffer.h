#pragma once

#include <cstring>

#include "../primitive/EmpirePrimitives.h"
#include "../EmpireException.h"

#define VLE_DATA_MASK				0b01111111
#define VLE_STATUS_MASK				0b10000000
#define VLE_HAS_NEXT_BYTE			VLE_STATUS_MASK
#define VLE_DOESNT_HAVE_NEXT_BYTE   0

namespace Empire {

class Buffer {

protected:
	u8* m_Buffer;
	u64 m_Offset, m_Capacity;
	bool m_Free;

public:
	Buffer(u64 capacity = 1024) :
		m_Buffer(new u8[capacity]), m_Offset(0), m_Capacity(capacity), m_Free(true) {}

	Buffer(u8* buffer, u64 capacity) :
		m_Buffer(buffer), m_Offset(0), m_Capacity(capacity), m_Free(false) {}

	inline u8* GetPointer() const { return m_Buffer + m_Offset; }

	inline u8* GetStart() const { return m_Buffer; }

	inline u64 Offset() const { return m_Offset; }

	inline void Offset(u64 newOffset) { this->m_Offset = newOffset; }

	inline Buffer() {
		if (m_Free) {
			delete[] m_Buffer;
		}
	}
};

}//namespace