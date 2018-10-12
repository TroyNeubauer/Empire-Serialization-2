#include "Buffer.h"
#include <iostream>

int main() {

	BasicMemoryBuffer<size_t> buffer(4);
	buffer.ensureCapacity(12);
	int* buf = (int*) buffer.getPointer();
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
	buffer.flush();

	FILE* file = fopen("data.dat", "wb");
	if (file != NULL) {
		fwrite(buffer.getStart(), 1, buffer.offset(), file);
	}
	fclose(file);

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
		buffer = (uint8_t*) realloc(buffer, capacity);
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
/*
template<class T> BasicMemoryBuffer<T>::~BasicMemoryBuffer() {
	free();
}*/


