#pragma once
#include "EmpirePrimitives.h"


namespace Empire {
#if EMPIRE_SOFTWARE_S128
	union s128;
#else
	typedef signed __int128 s128;
#endif

std::string to_string(s128 value, u8 base = 10);
void FromString(const char* string, u64 length, u8 base, s128* result ERROR_CODE_PARAMETER);

u64 log10(s128 value);
u64 log2(s128 value);

union s128 {
	std::string ToString(u8 base = 10) const;
};

}//namespace
