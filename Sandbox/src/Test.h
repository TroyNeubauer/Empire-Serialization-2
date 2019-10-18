#pragma once

#include <string>
#include <assert.h>
#include <functional>

#define PRINT_EXTRA 1


template<typename T, typename OpT, typename OpB, typename String>
bool CompareOperator(std::string aStr, std::string bStr, String op) {
	OpT opt;
	OpB opb;
	BigUint ao(aStr), bo(bStr);
	T at(aStr), bt(bStr);

	T result = opt(at, bt);
	BigUint compare = opb(ao, bo);
	std::string resultStr = result.ToString();
	std::string compareStr = compare.str();
	if (resultStr.compare(compareStr) == 0) {
#if PRINT_EXTRA
		std::cout << aStr << ' ' << op << ' ' << bStr << " = " << std::endl << resultStr << std::endl;
#else
		std::cout << ".";
#endif
	} else {
		std::cout << std::endl << "Results differ for op "  << op << std::endl;
		
		assert(false);
	}
	return true;
}

template<typename T, int testCount>
void RunAllTests() {
	constexpr int chars = sizeof(T) * 2;//2 * the number of bytes so that we must use the upper half in operations
	char aBuf[chars];
	char bBuf[chars];
	for (int test = 0; test < testCount; test++) {
		// Generate new random numbers for this time
		int val = std::rand() % 100;
		if (val < 90) {
			std::memset(aBuf, '0', chars);
			std::memset(bBuf, '0', chars);
			int first = 0;
			for (int i = first + 1; i < chars; i++) {
				aBuf[i] = DIGITS[std::rand() % 10];
			}
			for (int i = 0; i < chars; i++) {
				bBuf[i] = DIGITS[std::rand() % 10];
			}
			aBuf[first] = DIGITS[std::rand() % 9 + 1];//[1,9]so that b can always be smaller if needed
			while (bBuf[first] >= aBuf[first]) {
				bBuf[first]--;
			}
		} else if (val < 95) {
			std::memset(aBuf, '0', chars);
			for (int i = 0; i < chars; i++) {
				aBuf[i] = DIGITS[std::rand() % 10];
			}
			std::memcpy(bBuf, aBuf, chars);
		} else {//Same as first case but with smaller numbers
			std::memset(aBuf, '0', chars);
			std::memset(bBuf, '0', chars);
			int first = chars * 2 / 3;
			for (int i = first + 1; i < chars; i++) {
				aBuf[i] = DIGITS[std::rand() % 10];
			}
			for (int i = first + 2; i < chars; i++) {
				bBuf[i] = DIGITS[std::rand() % 10];
			}
			aBuf[first] = DIGITS[std::rand() % 9 + 1];//[1,9]so that b can always be smaller if needed
			while (bBuf[first] >= aBuf[first]) {
				bBuf[first]--;
			}
		}
		std::string a(aBuf, chars);
		std::string b(bBuf, chars);
		CompareOperator<T, std::plus<T>, std::plus<BigUint>, const char *>(a, b, "+");
		CompareOperator<T, std::minus<T>, std::minus<BigUint>, const char *>(a, b, "-");
		CompareOperator<T, std::multiplies<T>, std::multiplies<BigUint>, const char *>(a, b, "*");
		//CompareOperator<T, std::divides<T>, std::divides<BigUint>, const char *>(a, b, "/");
	}
}


