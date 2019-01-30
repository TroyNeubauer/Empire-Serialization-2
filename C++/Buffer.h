#pragma once

template <typename MEMBER_TYPE>
class AbstractBuffer {

private:
	MEMBER_TYPE m_Offset, m_Capacity;
	uint8_t* m_Buffer;

public:
	AbstractBuffer(MEMBER_TYPE capacity = 1024) : m_Offset(0), m_Capacity(capacity), m_Buffer(new uint8_t[capacity]) {}
	AbstractBuffer(MEMBER_TYPE size, MEMBER_TYPE capacity, uint8_t* buffer) : m_Offset(size), m_Capacity(capacity), m_Buffer(buffer) {}

	void Resize(MEMBER_TYPE newCapacity) {
		uint8_t* newBuffer = new uint8_t[newCapacity];
		std::memcpy(newBuffer, m_Buffer, m_Offset);
		m_Capacity = newCapacity;
		delete[] m_Buffer;
		m_Buffer = newBuffer;
	}

	void EnsureCapacity(MEMBER_TYPE bytes) {
		MEMBER_TYPE newCapacity = m_Capacity;
		MEMBER_TYPE required = bytes + m_Offset;
		if (required > m_Capacity) {
			while (newCapacity < required) {
				newCapacity <<= 1;
			}
			Resize(newCapacity);
		}
	}
	
	template<typename T>
	inline void write(T* value, size_t bytes) {
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

	inline uint8_t* GetPointer() {
		return m_Buffer + m_Offset;
	}

	inline uint8_t* GetStart() {
		return m_Buffer;
	}

	inline MEMBER_TYPE Offset() {
		return m_Offset;
	}

	~AbstractBuffer() {
		delete[] m_Buffer;
	}

private:
	template<typename T>
	inline void WriteImpl(T* value, size_t bytes) {
		std::memcpy(GetPointer(), value, bytes);
		m_Offset += bytes;
	}

};

typedef AbstractBuffer<size_t> Buffer;

