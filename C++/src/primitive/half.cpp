#include "half.h"

#include <intrin.h>
#include <memory>

namespace Empire {


#ifdef EMPIRE_AVX_GENERAL

half FloatToHalf(float value) {
	__m128 src;
	src.m128_f32[0] = value;
	__m128i result = _mm_cvtps_ph(src, _MM_FROUND_TO_NEAREST_INT);
	return half(result.m128i_u16[0]);
}

float HalfToFloat(half value) {
	__m128i src;
	src.m128i_u16[0] = value.value;
	__m128 result = _mm_cvtph_ps(src);
	return result.m128_f32[0];
}


void FloatsToHalves(half* dest, float* src, size_t elements) {
	while (elements >= 4) {
		__m128 temp = *((__m128*) src);
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		((__m128i*) dest)->m128i_u64[0] = result.m128i_u64[0];//Copy 4 * 16 bit halves = 64 bits

		elements -= 4;
		dest += 4;
		src += 4;
	}

	if (elements == 3) {
		__m128 temp;
		temp.m128_u64[0] = *((u64*)src);// Copy the first 2 elements
		src += 2;
		temp.m128_f32[2] = *src; src++;
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		std::memcpy(dest, &result, 3 * sizeof(half));
	} else if (elements == 2) {
		__m128 temp;
		temp.m128_u64[0] = *((u64*)src);// Copy the first 2 elements
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		std::memcpy(dest, &result, 2 * sizeof(half));
	} else if (elements == 1) {
		__m128 temp;
		temp.m128_f32[0] = *src; src++;
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		*dest = half(result.m128i_u16[0]);
	}
}

void HalvesToFloats(float* dest, half* src, size_t elements) {
	__m128i temp;
	while (elements >= 4) {
		temp.m128i_u64[0] = *((u64*)src);// copy 4 * 16 bit half = 64 bits
		__m128 result = _mm_cvtph_ps(temp);
		*((__m128*) dest) = result;// Write 4 floats = 128 bits

		elements -= 4;
		dest += 4;
		src += 4;
	}

	if (elements == 3) {
		temp.m128i_u32[0] = *((u32*)src);  src += 2;
		temp.m128i_u16[2] = *((u16*)src);

		__m128 result = _mm_cvtph_ps(temp);
		*((u64*)dest) = result.m128_u64[0];//Copy 2 floats
		dest += 2;
		*dest = result.m128_f32[2];//Copy last float
	} else if (elements == 2) {
		temp.m128i_u32[0] = *((u32*)src);

		__m128 result = _mm_cvtph_ps(temp);
		*((u64*)dest) = result.m128_u64[0];// Copy last 2 floats
	} else if (elements == 1) {
		temp.m128i_u16[0] = *((u16*)src);

		__m128 result = _mm_cvtph_ps(temp);
		*dest = result.m128_f32[0];
	}
}


#else
half FloatToHalf(float value) {
	FloatBits v, s;
	v.f = value;
	uint32_t sign = v.si & signN;
	v.si ^= sign;
	sign >>= shiftSign; // logical shift
	s.si = mulN;
	s.si = s.f * v.f; // correct subnormals
	v.si ^= (s.si ^ v.si) & -(minN > v.si);
	v.si ^= (infN ^ v.si) & -((infN > v.si) & (v.si > maxN));
	v.si ^= (nanN ^ v.si) & -((nanN > v.si) & (v.si > infN));
	v.ui >>= shift; // logical shift
	v.si ^= ((v.si - maxD) ^ v.si) & -(v.si > maxC);
	v.si ^= ((v.si - minD) ^ v.si) & -(v.si > subC);
	return half((u16) (v.ui | sign));
}

float HalfToFloat(half value) {
	FloatBits v;
	v.ui = value.value;
	int32_t sign = v.si & signC;
	v.si ^= sign;
	sign <<= shiftSign;
	v.si ^= ((v.si + minD) ^ v.si) & -(v.si > subC);
	v.si ^= ((v.si + maxD) ^ v.si) & -(v.si > maxC);
	FloatBits s;
	s.si = mulC;
	s.f *= v.si;
	int32_t mask = -(norC > v.si);
	v.si <<= shift;
	v.si ^= (s.si ^ v.si) & mask;
	v.si |= sign;
	return v.f;
}

template<typename A, typename B>
void Convert(A* dest, B* src, size_t elements) {
	for (int i = 0; i < elements; i++) {
		dest[i] = (A) src[i];
	}
}


void FloatsToHalves(half* dest, float* src, size_t elements) {
	Convert(dest, src, elements);
}

void HalvesToFloats(float* dest, half* src, size_t elements) {
	Convert(dest, src, elements);
}


void FloatsToDoubles(double* dest, float* src, size_t elements) {
	//D35 instructions R9 - 2 floats D48 35R- N floats
	Convert(dest, src, elements);
}

void DoublesToFloats(float* dest, double* src, size_t elements) {
	Convert(dest, src, elements);
}

#endif

}//namespace