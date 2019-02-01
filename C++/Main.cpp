#include <iostream>
#include <time.h>

#include "Buffer.h"
#include "EmpireException.h"
#include "EmpireErrorCodes.h"
#include "EmpirePrimitives.h"
#include "little-big-int.hpp"

using namespace Empire;

template<typename T> 
T ParsePtimitive(std::string string);


template<>
int ParsePtimitive(std::string string) {
	return std::stoi(string, 0, 16);
}

template<>
long ParsePtimitive(std::string string) {
	return std::stol(string, 0, 16);
}

template<>
long long ParsePtimitive(std::string string) {
	return std::stoll(string, 0, 16);
}

template <typename T> std::string ToString(T value) {
	size_t hex_len = sizeof(T) << 1;
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = DIGITS[(value >> j) & 0x0F];
	return rc;
}

template<typename T, std::string(*ToString)(T), typename Op>
bool CompartOperator(T a, T b, BigUint aCmp, BigUint bCmp) {
	Op op;
	T result = op(a, b);
	BigUint compare = op(a, b);
	

	std::cout << ToString(a) << " + " << ToString(b) << std::endl;
	std::cout << "= " << ToString(result) << std::endl << std::endl;

	return true;
}


template<typename T, int testCount, std::string(*ToString)(T), T(*FromString)(std::string) >
void RunAllTests() {
	constexpr int chars = sizeof(T) * 2;// Were using hex
	char aBuf[chars];
	char bBuf[chars];
	std::memset(aBuf, '0', chars);
	std::memset(bBuf, '0', chars);
	for (int test = 0; test < testCount; test++) {
		// Generate new random numbers for this time
		for (int i = chars - chars / 4; i < chars; i++) {
			aBuf[i] = DIGITS[std::rand() % 16];
			bBuf[i] = DIGITS[std::rand() % 16];
		}
		std::string aStr(aBuf, chars);
		std::string bStr(bBuf, chars);
		T a = FromString(aStr);
		T b = FromString(bStr);
		BigUint aCmp(aStr);
		BigUint bCmp(bStr);
		CompartOperator<T, ToString, std::plus<T>>(a, b, aCmp, bCmp);

	}
}

int main() {
	u128 a(0b0010101110111010000010100010101010001010001010101010100101000000L, 0b1010111010111111100111110011111111001010011010101010100101000100L);
	u32 b = 2;
	std::cout << std::oct << a << std::endl;
	std::cout << std::hex << b << std::endl << "==========================" << std::endl;
	a *= b;
	std::cout << std::oct << a << std::endl;

	system("PAUSE");
	return 0;
	std::srand(time(nullptr));
	RunAllTests<int, 1, ToString, ParsePtimitive>();


	system("PAUSE");
	Buffer buffer;
	buffer.EnsureCapacity(12);


	FILE* file = fopen("data.dat", "wb");
	if (file) {
		fwrite(buffer.GetStart(), 1, buffer.Offset(), file);
		fclose(file);
	}
}
