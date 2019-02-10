#include "FloatUtil.h"

#include <intrin.h>


void FloatsToDoubles(double* dest, float* src, size_t elements) {
	//D33 instructions R6 - 2 floats  33D  13R- N floats	
	__m128 temp;
	while (elements >= 2) {
		temp.m128_u64[0] = *((u64*)src);//Copy 2 floats
		__m128d result = _mm_cvtps_pd(temp);
		*((__m128d*) dest) = result; //Copy 2 doubles to dest

		elements -= 2;
		dest += 2;
		src += 2;
	}
	if (elements == 1) {
		temp.m128_f32[0] = *src;
		__m128d result = _mm_cvtps_pd(temp);
		*dest = result.m128d_f64[0];
	}
	//Basic casts are slow
}