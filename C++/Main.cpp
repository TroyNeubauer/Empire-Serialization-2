#include <iostream>

#include "Buffer.h"
#include "EmpireException.h"
#include "EmpireErrorCodes.h"
#include "EmpirePrimitives.h"

using namespace Empire;



int main() {
	u128 test(0x0011223344556677, 0x8899AABBCCDDEEFF);
	std::cout << "test: " << test << std::endl << std::endl;
	u128 value = 0xFFFFFFFFFFFFFF;
	u128 value2 = 0xFFFFFFFFFFFFFF;
	std::cout << value << std::endl;
	std::cout << value2 << std::endl;
	std::cout << "+ ________________" << std::endl;
	u128 result = value + value2;
	std::cout << result << std::endl;

	system("PAUSE");
	Buffer buffer;
	buffer.EnsureCapacity(12);


	FILE* file = fopen("data.dat", "wb");
	if (file) {
		fwrite(buffer.GetStart(), 1, buffer.Offset(), file);
		fclose(file);
	}
}
