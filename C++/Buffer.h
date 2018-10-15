#pragma once


#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <limits>

template <class T> class BasicBuffer {

public:
	/* Ensures that the buffer can hold count more bytes also increments size after the next call to ensure capacity */
	void ensureCapacity(T count) = delete;

	uint8_t* getPointer() = delete;
	uint8_t* getStart() = delete;
	void flush() = delete;
	void free() = delete;

	T offset() = delete;

};

template <class T> class BasicMemoryBuffer : BasicBuffer<T> {
public:
	T size, capacity, blockSize = std::numeric_limits<T>::max();
	uint8_t* buffer;
	bool ownsMemory;

public:
	BasicMemoryBuffer(T capacity) : size(0), capacity(capacity), buffer((uint8_t*)malloc(capacity)), ownsMemory(true) {}
	BasicMemoryBuffer(T size, T capacity, uint8_t* buffer) : size(size), capacity(capacity), buffer(buffer), ownsMemory(false) {}

	void resize(T newCap);

	void ensureCapacity(T newCap);

	uint8_t* getPointer();
	uint8_t* getStart();
	void flush();
	void free();

	T offset();


	//~BasicMemoryBuffer();

};



template<class T> void BasicMemoryBuffer<T>::resize(T newCap) {
	if (ownsMemory) {
		free(buffer);
	}
	ownsMemory = true;
	buffer = (uint8_t*)malloc(newCap);
	capacity = newCap;
}

template<class T> void BasicMemoryBuffer<T>::ensureCapacity(T bytes) {
	if (bytes == std::numeric_limits<T>::max()) {
		std::cout << "Invalid number of bytes " << bytes << " cannot be " << std::numeric_limits<T>::max();
	}
	if (blockSize != std::numeric_limits<T>::max()) {//If this isnt the first time
		size += blockSize;//Increment from last time
	}

	T newCap = bytes + size;
	if (newCap > capacity) {
		while (capacity < newCap) {
			capacity <<= 1;
		}
		buffer = (uint8_t*)realloc(buffer, capacity);
	}
	blockSize = bytes;
}

template<class T> uint8_t* BasicMemoryBuffer<T>::getStart() {
	return buffer;
}

template<class T> uint8_t* BasicMemoryBuffer<T>::getPointer() {
	return buffer + size;
}


template<class T> void BasicMemoryBuffer<T>::flush() {
	if (blockSize != std::numeric_limits<T>::max()) {//If this isnt the first time
		size += blockSize;//Increment from last time
	}
	blockSize = 0;
}

template<class T> void BasicMemoryBuffer<T>::free() {
	if (ownsMemory) {
		free(buffer);
		ownsMemory = false;
	}
	buffer = nullptr;
	size = 0;
	capacity = 0;
}

template<class T> T BasicMemoryBuffer<T>::offset() {
	return size;
}
