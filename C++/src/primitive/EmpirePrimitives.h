#pragma once

#include <stdint.h>
#include "EmpirePrimitivesConfig.h"


#if EMPIRE_SOFTWARE_UINT_128
	union uint128_t;
	union int128_t;
#else
	typedef unsigned __int128 uint128_t;
	typedef signed __int128 int128_t;
#endif

#if EMPIRE_SOFTWARE_FLOAT_16
	struct float16;
#else
	typedef __fp16 float16;
#endif


typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

typedef uint128_t u128;
typedef int128_t s128;




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


