#include <iostream>
#include "Buffer.h"

int main() {
	BasicMemoryBuffer<size_t> buffer(4);
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
	buffer.flush();

	FILE* file = fopen("data.dat", "wb");
	if (file != NULL) {
		fwrite(buffer.getStart(), 1, buffer.offset(), file);
	}
	fclose(file);
	buffer.~BasicMemoryBuffer();
}