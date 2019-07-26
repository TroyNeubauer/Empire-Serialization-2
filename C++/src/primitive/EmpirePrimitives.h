#pragma once

#include <stdint.h>

#include "Integers.h"
#include "IEEE754.hpp"
#include "double_integer.hpp"

#include "EmpirePrimitivesConfig.h"

#ifdef EMPIRE_HAVE_COMPILER_F16
	typedef __fp16 float16;
#else
	//typedef IEEE754<10, 5> float16;
#endif

#ifdef EMPIRE_HAVE_COMPILER_F128
	typedef __float128 float128;
#else
	//typedef IEEE754<112, 15> float128;
#endif

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

//typedef float16 f16;
//typedef float16 half;

typedef float f32;
typedef double f64;

//typedef float128 f128;

namespace Empire {
	struct EmpireError;
	typedef u32 ErrorCode;
	typedef u32 EmpireType;
}


