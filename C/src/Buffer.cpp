#include "Buffer.h"


template<class T> void BasicMemoryBuffer<T>::resize(T newCap) {
	if (ownsMemory) {
		free(buffer);
	}
	ownsMemory = true;
	buffer = (uint8_t*) malloc(newCap);
	capacity = newCap;
}

template<class T> void BasicMemoryBuffer<T>::ensureCapacity(T bytes) {
	T newCap = bytes + size;
	if (newCap > capacity) {
		while (capacity < newCap) {
			capacity <<= 1;
		}
		buffer = realloc(buffer, capacity);
	}
}
template<class T> void BasicMemoryBuffer<T>::advance(T count) {
	size += count;
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

template<class T> BasicMemoryBuffer<T>::~BasicMemoryBuffer() {
	free();
}


