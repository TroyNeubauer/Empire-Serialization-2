#pragma once

template <typename MEMBER_TYPE>
class AbstractBuffer {

private:
	MEMBER_TYPE m_Offset, m_Capacity;
	uint8_t* m_Buffer;

public:
	AbstractBuffer(MEMBER_TYPE capacity = 1024) : m_Offset(0), m_Capacity(capacity), m_Buffer(new uint8_t[capacity]) {}
	AbstractBuffer(MEMBER_TYPE size, MEMBER_TYPE capacity, uint8_t* buffer) : m_Offset(size), m_Capacity(capacity), m_Buffer(buffer) {}

	void resize(MEMBER_TYPE newCapacity) {
		uint8_t* newBuffer = new uint8_t[newCapacity];
		std::memcpy(newBuffer, m_Buffer, m_Offset);
		m_Capacity = newCapacity;
		delete[] m_Buffer;
		m_Buffer = newBuffer;
	}

	void ensureCapacity(MEMBER_TYPE bytes) {
		MEMBER_TYPE newCapacity = m_Capacity;
		MEMBER_TYPE required = bytes + m_Offset;
		if (required > m_Capacity) {
			while (newCapacity < required) {
				newCapacity <<= 1;
			}
			resize(newCapacity);
		}
	}
	
	template<typename T>
	inline void write(T* value, size_t bytes) {
		ensureCapacity(bytes);
		writeImpl(value, bytes);
	}

	template<typename T>
	inline void writeRaw(T* value, size_t bytes) {
		writeImpl(value, bytes);
	}

	template<typename T>
	inline void write(T value) {
		ensureCapacity(sizeof(T));
		writeImpl(&value, sizeof(T));
	}

	template<typename T>
	inline void writeRaw(T value) {
		writeImpl(&value, sizeof(T));
	}

	uint8_t* getPointer() {
		return m_Buffer + m_Offset;
	}

	uint8_t* getStart() {
		return m_Buffer;
	}

	MEMBER_TYPE offset() {
		return m_Offset;
	}

	~AbstractBuffer() {
		delete[] m_Buffer;
	}

private:
	template<typename T>
	void writeImpl(T* value, size_t bytes) {
		std::memcpy(getPointer(), value, bytes);
		m_Offset += bytes;
	}

};

typedef AbstractBuffer<size_t> Buffer;

