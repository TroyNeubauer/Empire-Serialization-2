#pragma once
#include "../EmpireException.h"
#include "EmpirePrimitives.h"

#include <string>

using namespace Empire;

#if EMPIRE_SOFTWARE_INT_128
	union int128_t;
#else
	typedef signed __int128 int_128_t;
#endif

std::string to_string(int128_t value, u8 base = 10);
void FromString(const char* string, uint64_t length, u8 base, int128_t* result EMPIRE_ERROR_PARAMETER);

uint64_t log10(int128_t value);
uint64_t log2(int128_t value);

union int128_t {
	std::string ToString(u8 base = 10) const;
};
