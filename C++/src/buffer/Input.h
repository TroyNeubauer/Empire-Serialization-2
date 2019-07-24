#pragma once

#include "Buffer.h"
#include "Output.h"

namespace Empire {

class Input : public Buffer {
public:

	Input(u64 capacity) : Buffer(capacity), m_Limit(capacity) {}

	Input(u8* buffer, u64 capacity) : 
		Buffer(buffer, capacity), m_Limit(capacity) {}

	Input(Output& out) : 
		Buffer(out.m_Buffer, out.m_Capacity), m_Limit(out.m_Offset) {}

	/*
	* Returns true if there is enough space in the buffer to read x bytes, false if a buffer overflow will occur by reading x bytes
	*/
	inline bool EnsureCapacity(u64 bytes EMPIRE_ERROR_PARAMETER) {
		if (bytes + m_Offset > m_Limit) {
			EMPIRE_ERROR(BUFFER_OVERFLOW, new BufferOverflowErrorData(bytes, m_Offset, m_Limit), false);
		}
		return true;
	}

	template<typename T>
	T ReadVLE(EMPIRE_ERROR_PARAMETER1) {
		T result = 0;
		u32 shift = 0;
		u8 byte;
		do {
			if (!EnsureCapacity(1 EMPIRE_ERROR_VAR)) return 0;
			byte = m_Buffer[m_Offset++];
			u8 data = byte & VLE_DATA_MASK;
			T next = static_cast<T>(data) << shift;
			result |= next;
			shift += 7;
		} while (byte & VLE_STATUS_MASK);


		return result;
	}
	template<typename T>
	T Read(EMPIRE_ERROR_PARAMETER1) {
		EnsureCapacity(sizeof(T) EMPIRE_ERROR_VAR);
		T result;
		ReadRaw(&result, sizeof(T));
		return result;
	}

	template<typename T>
	void Read(T* dest, u64 bytes EMPIRE_ERROR_PARAMETER) {
		EnsureCapacity(bytes EMPIRE_ERROR_VAR);
		ReadRaw(dest, bytes);
	}

private:
	template<typename T>
	inline void ReadRaw(T* dest, u64 bytes) {
		std::memcpy(dest, GetPointer(), bytes);
		m_Offset += bytes;
	}


protected:
	u64 m_Limit;

	friend class Output;
};


}//namespace
