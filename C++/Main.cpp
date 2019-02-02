#include <iostream>
#include <time.h>

#include "little-big-int.hpp"

#include "Empire.h"

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

//Full length string - fills 128 bits
//ABCDEFABCDEFABCD1122334455667788
int main() {
	ParseOverFlowData* data = new ParseOverFlowData { std::string("Input"), std::string("Error") };


	std::string str;
	std::cin >> str;
	EmpireError code;
	u128 a(str, code, 16);
	std::cout << "Error Code" << ErrorCodeToString(code.code) << std::endl;
	std::cout << a.ToString() << std::endl;

	system("PAUSE");
}
