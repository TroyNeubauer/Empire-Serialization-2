#pragma once
#include <stdint.h>
#include <stdlib.h>

template <class T> class BasicBuffer {

public:	
	/* Ensures that the buffer can hold count more bytes */
	void ensureCapacity(T count) = delete;
	/* Advances the position by x bytes (will affect calls to getPointer) */
	void advance(T count);

	uint8_t* getPointer() = delete;
	void flush() = delete;
	void free() = delete;

};

template <class T> class BasicMemoryBuffer : BasicBuffer<T> {
public:
	T size, capacity;
	uint8_t* buffer;
	bool ownsMemory;

public:
	BasicMemoryBuffer(T capacity) : size(0), capacity(capacity), buffer((uint8_t*) malloc(capacity)), ownsMemory(true) {}
	BasicMemoryBuffer(T size, T capacity, uint8_t* buffer) : size(size), capacity(capacity), buffer(buffer), ownsMemory(false) {}

	void resize(T newCap);
	
	void ensureCapacity(T newCap);
	void advance(T count);

	uint8_t* getPointer();
	void flush();
	void free();

	~BasicMemoryBuffer();

};


