#pragma once
#include "EmpirePrimitives.h"

namespace Empire {

struct half;

half FloatToHalf(float value);
float HalfToFloat(half value);

void FloatsToHalves(half* dest, float* src, size_t elements);
void HalvesToFloats(float* dest, half* src, size_t elements);

struct half {
	uint16_t value;
	half() {}
	half(uint16_t value) : value(value) {}
	half(float value) : value(FloatToHalf(value)) {}
	half(double value) : half((float) value) {}

	operator float() const { return HalfToFloat(*this); };
	operator double() const { return (double) HalfToFloat(*this); };
	operator uint16_t() const { return value; };
};

union FloatBits {
	float f;
	int32_t si;
	uint32_t ui;
};

static int const shift = 13;
static int const shiftSign = 16;

static int32_t const infN = 0x7F800000; // flt32 infinity
static int32_t const maxN = 0x477FE000; // max half normal as a float
static int32_t const minN = 0x38800000; // min half normal as a float
static int32_t const signN = 0x80000000; // float sign bit

static int32_t const infC = infN >> shift;
static int32_t const nanN = (infC + 1) << shift; // minimum half nan as a float
static int32_t const maxC = maxN >> shift;
static int32_t const minC = minN >> shift;
static int32_t const signC = signN >> shiftSign; // half sign bit

static int32_t const mulN = 0x52000000; // (1 << 23) / minN
static int32_t const mulC = 0x33800000; // minN / (1 << (23 - shift))

static int32_t const subC = 0x003FF; // max float subnormal down shifted
static int32_t const norC = 0x00400; // min float normal down shifted

static int32_t const maxD = infC - maxC - 1;
static int32_t const minD = minC - subC - 1;


}
