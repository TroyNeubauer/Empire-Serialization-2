#pragma once

#include "Buffer.h"

namespace Empire {

class Output : public Buffer {
public:
	Output(u64 capacity = 1024) : Buffer(capacity) {}
	
	Output(u64 capacity, u8* buffer) : Buffer(buffer, capacity) {}
	
	void Resize(u64 newCapacity);

	void EnsureCapacity(u64 bytes);

	template<typename T>
	void WriteVLE(T valueIn) {
		EnsureCapacity(sizeof(T) * 8 / 7 + 1);
		typedef std::make_unsigned<T>::type E;
		E value = static_cast<E>(valueIn);
		do {
			u8 byte = static_cast<u8>(value & VLE_DATA_MASK);// Bottom 7 bits

			if (value >> 7) byte |= VLE_HAS_NEXT_BYTE;
			else			byte |= VLE_DOESNT_HAVE_NEXT_BYTE;

			m_Buffer[m_Offset++] = byte;
			value >>= 7;// Get next 7 bits
		} while (value);
	}

	template<typename T>
	inline void Write(T* value, size_t bytes) {
		EnsureCapacity(bytes);
		WriteImpl(value, bytes);
	}

	template<typename T>
	inline void WriteRaw(T* value, size_t bytes) {
		WriteImpl(value, bytes);
	}

	template<typename T>
	inline void Write(T value) {
		EnsureCapacity(sizeof(T));
		WriteImpl(&value, sizeof(T));
	}

	template<typename T>
	inline void WriteRaw(T value) {
		WriteImpl(&value, sizeof(T));
	}

private:
	template<typename T>
	inline void WriteImpl(T* value, size_t bytes) {
		std::memcpy(GetPointer(), value, bytes);
		m_Offset += bytes;
	}
	friend class Input;
};


}//namespace
