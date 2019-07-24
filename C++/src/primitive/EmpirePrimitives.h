#pragma once

#include <stdint.h>

#include "IEEE754.hpp"
#include "double_integer.hpp"

#include "EmpirePrimitivesConfig.h"

#ifdef EMPIRE_HAVE_COMPILER_F16
	typedef __fp16 float16;
#else
	typedef IEEE754<10, 5> float16;
#endif



typedef IEEE754<112, 15> float128;

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;



typedef float16 f16;
typedef float16 half;

typedef float f32;
typedef double f64;


//typedef f128 float128;
//typedef f256 float256;
namespace Empire {
	struct EmpireError;
	typedef u32 ErrorCode;
	typedef u32 EmpireType;
}


