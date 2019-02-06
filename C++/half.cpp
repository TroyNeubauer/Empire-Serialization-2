#include "half.h"

namespace Empire {


half FloatToHalf(float value) {
	
	__m128 src;
	src.m128_f32[0] = value;
	__m128i result = _mm_maskz_cvt_roundps_ph(0b0001, src, _MM_FROUND_TO_NEAREST_INT);

	return result.m128i_u16[0];
}

float HalfToFloat(half value) {

}


}//namespace