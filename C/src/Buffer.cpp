#include "Buffer.h"
#include <iostream>

int main() {
	std::cout << std::numeric_limits<int>::max() << ", " << std::numeric_limits<unsigned short>::max();
	system("PAUSE");
	return 0;
	BasicMemoryBuffer<size_t> buffer(10);
	buffer.ensureCapacity(12);
	int* buf = (int*)buffer.getPointer();
	*buf = 5;
	buf++;

	*buf = 5;
	buf++;

	*buf = 5;
	buf++;

	buffer.ensureCapacity(12);

	*buf = 5;
	buf++;

	*buf = 5;
	buf++;

	*buf = 5;
	buf++;

}


template<class T> void BasicMemoryBuffer<T>::resize(T newCap) {
	if (ownsMemory) {
		free(buffer);
	}
	ownsMemory = true;
	buffer = (uint8_t*) malloc(newCap);
	capacity = newCap;
}

template<class T> void BasicMemoryBuffer<T>::ensureCapacity(T bytes) {
	if (bytes == std::numeric_limits<T>::max()) {
		std::cout << "Invalid number of bytes " << bytes << " cannot be " << std::numeric_limits<T>::max(;
	}
	if (blockSize != -1) {
		size += blockSize;//Increment from last time
	}
	T newCap = bytes + size;
	if (newCap > capacity) {
		while (capacity < newCap) {
			capacity <<= 1;
		}
		buffer = (uint8_t*) realloc(buffer, capacity);
	}
}

template<class T> uint8_t* BasicMemoryBuffer<T>::getPointer() {
	return buffer + size;
}

template<class T> void BasicMemoryBuffer<T>::flush() {
	//nop
	//nothing to flush
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
/*
template<class T> BasicMemoryBuffer<T>::~BasicMemoryBuffer() {
	free();
}*/


