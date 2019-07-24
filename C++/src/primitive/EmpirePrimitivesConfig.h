#pragma once

#define EMPIRE_ENABLE_INTEL_INTRINSICS

#ifdef __SIZEOF_UINT128__
	#define EMPIRE_HAVE_COMPILER_I128
#endif

#ifdef __SIZEOF_FLOAT16__
	#define EMPIRE_HAVE_COMPILER_F16
#endif


#if defined(DISABLE_SMID)
	#define EMPIRE_NO_FP_EXTENSIONS
#elif defined(__AVX2__)
	#define EMPIRE_AVX2
	#define EMPIRE_AVX_GENERAL
#elif defined ( __AVX__ )
	#define EMPIRE_AVX
	#define EMPIRE_AVX_GENERAL
#elif defined(_M_AMD64) || defined(_M_X64)
	#define EMPIRE_AVX2_SSE2_X64
	#define EMPIRE_AVX_GENERAL
#elif _M_IX86_FP == 2
	#define EMPIRE_AVX2_SSE2_X32
	#define EMPIRE_AVX_GENERAL
#elif _M_IX86_FP == 1
	#define EMPIRE_AVX2_SSE_X32
	#define EMPIRE_AVX_GENERAL
#endif
