#pragma once

#include "EmpirePrimitivesConfig.h"

#ifdef EMPIRE_HAVE_COMPILER_I128
	typedef __uint128 u128;
	typedef __int128 s128;
#else
	#include "double_integer.hpp"
	
	typedef double_integer<uint64_t, uint64_t> u128;
	typedef double_integer<uint64_t, int64_t> s128;

#endif

typedef double_integer<u128, u128> u256;
//typedef double_integer<u128, s128> s256;
