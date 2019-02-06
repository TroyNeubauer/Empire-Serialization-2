#pragma once

#include "EmpireErrorCodes.h"
#include "EmpireException.h"

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <ios>
#include <string>
#include <sstream>

#include <assert.h>
#include <intrin.h>

const char DIGITS[36] = { 
						  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 
						  'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
						  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' 
						};

const int MAX_BASE = sizeof(DIGITS);


#define EMPIRE_ENABLE_INTEL_INTRINSICS 1
#define EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING 1

#if !EMPIRE_ENABLE_INTEL_INTRINSICS
	#error Cannot compile without intrinsics! intrinsics needed for addition and subtraction
#endif

#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)
	#define EMPIRE_SOFTWARE_U128 0
	#define EMPIRE_SOFTWARE_S128 0
#else
	#define EMPIRE_SOFTWARE_U128 1
	#define EMPIRE_SOFTWARE_S128 1
#endif

#ifdef __AVX2__
	#define EMPIRE_AVX2
#elif defined ( __AVX__ )
	#define EMPIRE_AVX
#elif (defined(_M_AMD64) || defined(_M_X64))
	#define EMPIRE_AVX2_SSE2_X64
#elif _M_IX86_FP == 2
	#define EMPIRE_AVX2_SSE2_X32
#elif _M_IX86_FP == 1
	#define EMPIRE_AVX2_SSE_X32
#else
	#define EMPIRE_NO_FP_EXTENSIONS
#endif


typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

