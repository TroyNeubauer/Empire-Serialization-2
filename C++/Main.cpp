#include <iostream>
#include "Buffer.h"
#include "EmpireException.h"
#include "EmpireErrorCodes.h"

using namespace Empire;

int test(int x ERROR_CODE_PARAMETER) {
	if (x == 5) {
		ERROR(EMPIRE_INVALID_ARGUMENT,0)
	}
	return 0;
}


int main() {
	ErrorCode code = 0;
	try {
		test(15, code);
	} catch (std::exception e) {
		std::cout << "Error! Code " << code << " : " << e.what() << std::endl;
	}
	try {
		test(5, code);
	} catch (std::exception e) {
		std::cout << "Error! Code " << code << " : " << e.what() << std::endl;
	}
	system("PAUSE");
	Buffer buffer;
	buffer.ensureCapacity(12);


	FILE* file = fopen("data.dat", "wb");
	if (file != NULL) {
		fwrite(buffer.getStart(), 1, buffer.offset(), file);
	}
	fclose(file);
}
