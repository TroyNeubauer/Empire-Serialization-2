#pragma once

#define EMPIRE_ENABLE_INTEL_INTRINSICS 1
#define EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING 1

#if !EMPIRE_ENABLE_INTEL_INTRINSICS
	#error Cannot compile without intrinsics! intrinsics needed for addition and subtraction
#endif

#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)
	#define EMPIRE_SOFTWARE_UINT_128 0
	#define EMPIRE_SOFTWARE_INT_128 0
#else
	#define EMPIRE_SOFTWARE_UINT_128 1
	#define EMPIRE_SOFTWARE_INT_128 1
#endif

#if defined(__SIZEOF_FLOAT16__) && (__SIZEOF_FLOAT16__ == 2)
	#define EMPIRE_SOFTWARE_FLOAT_16 0
#else
	#define EMPIRE_SOFTWARE_FLOAT_16 1
#endif

#if defined(__SIZEOF_FLOAT128__) && (__SIZEOF_FLOAT128__ == 16)
	#define EMPIRE_SOFTWARE_FLOAT_128 0
#else
	#define EMPIRE_SOFTWARE_FLOAT_128 1
#endif

#if defined(__SIZEOF_FLOAT256__) && (__SIZEOF_FLOAT256__ == 32)
	#define EMPIRE_SOFTWARE_FLOAT_256 0
#else
	#define EMPIRE_SOFTWARE_FLOAT_256 1
#endif


#define DISABLE_SIMD 0
#if defined(__AVX2__) && !DISABLE_SIMD
	#define EMPIRE_AVX2
	#define EMPIRE_AVX_GENERAL
#elif defined ( __AVX__ ) && !DISABLE_SIMD
	#define EMPIRE_AVX
	#define EMPIRE_AVX_GENERAL
#elif (defined(_M_AMD64) || defined(_M_X64)) && !DISABLE_SIMD
	#define EMPIRE_AVX2_SSE2_X64
	#define EMPIRE_AVX_GENERAL
#elif _M_IX86_FP == 2 && !DISABLE_SIMD
	#define EMPIRE_AVX2_SSE2_X32
	#define EMPIRE_AVX_GENERAL
#elif _M_IX86_FP == 1 && !DISABLE_SIMD
	#define EMPIRE_AVX2_SSE_X32
	#define EMPIRE_AVX_GENERAL
#else
	#define EMPIRE_NO_FP_EXTENSIONS
#endif
