#include "float16.h"
#include "EmpirePrimitivesConfig.h"

#include <intrin.h>
#include <memory>

#ifdef EMPIRE_AVX_GENERAL

float16 FloatToHalf(float value) {
	__m128 src;
	src.m128_f32[0] = value;
	__m128i result = _mm_cvtps_ph(src, _MM_FROUND_TO_NEAREST_INT);
	return float16(result.m128i_u16[0]);
}

float HalfToFloat(float16 value) {
	__m128i src;
	src.m128i_u16[0] = value.m_Value;
	__m128 result = _mm_cvtph_ps(src);
	return result.m128_f32[0];
}


void FloatsToHalves(float16* dest, float* src, size_t elements) {
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
		temp.m128_u64[0] = *((unsigned __int64*)src);// Copy the first 2 elements
		src += 2;
		temp.m128_f32[2] = *src; src++;
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		std::memcpy(dest, &result, 3 * sizeof(float16));
	} else if (elements == 2) {
		__m128 temp;
		temp.m128_u64[0] = *((unsigned __int64*)src);// Copy the first 2 elements
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		std::memcpy(dest, &result, 2 * sizeof(float16));
	} else if (elements == 1) {
		__m128 temp;
		temp.m128_f32[0] = *src; src++;
		__m128i result = _mm_cvtps_ph(temp, _MM_FROUND_TO_NEAREST_INT);
		*dest = float16(result.m128i_u16[0]);
	}
}

void HalvesToFloats(float* dest, float16* src, size_t elements) {
	__m128i temp;
	while (elements >= 4) {
		temp.m128i_u64[0] = *((unsigned __int64*)src);// copy 4 * 16 bit float16 = 64 bits
		__m128 result = _mm_cvtph_ps(temp);
		*((__m128*) dest) = result;// Write 4 floats = 128 bits

		elements -= 4;
		dest += 4;
		src += 4;
	}

	if (elements == 3) {
		temp.m128i_u32[0] = *((unsigned __int32*)src);  src += 2;
		temp.m128i_u16[2] = *((unsigned __int16*)src);

		__m128 result = _mm_cvtph_ps(temp);
		*((unsigned __int64*)dest) = result.m128_u64[0];//Copy 2 floats
		dest += 2;
		*dest = result.m128_f32[2];//Copy last float
	} else if (elements == 2) {
		temp.m128i_u32[0] = *((unsigned __int32*) src);

		__m128 result = _mm_cvtph_ps(temp);
		*((unsigned __int64*) dest) = result.m128_u64[0];// Copy last 2 floats
	} else if (elements == 1) {
		temp.m128i_u16[0] = *((unsigned __int16*) src);

		__m128 result = _mm_cvtph_ps(temp);
		*dest = result.m128_f32[0];
	}
}


#else

float16 FloatToHalf(float value) {
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
	return float16((uint16_t) (v.ui | sign));
}

float HalfToFloat(float16 value) {
	FloatBits v;
	v.ui = value.m_Value;
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


void FloatsToHalves(float16* dest, float* src, size_t elements) {
	Convert(dest, src, elements);
}

void HalvesToFloats(float* dest, float16* src, size_t elements) {
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
