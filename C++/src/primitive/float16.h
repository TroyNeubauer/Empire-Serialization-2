#pragma once
#include <stdint.h>
#include "EmpirePrimitivesConfig.h"
#include "EmpirePrimitives.h"

float16 FloatToHalf(float value);
float HalfToFloat(float16 value);

void FloatsToHalves(float16* dest, float* src, size_t elements);
void HalvesToFloats(float* dest, float16* src, size_t elements);
#if EMPIRE_SOFTWARE_FLOAT_16
struct float16 {
	uint16_t m_Value;
	float16() {}
	float16(uint16_t value) : m_Value(value) {}
	float16(float value) : m_Value(FloatToHalf(value).m_Value) {}
	float16(double value) : float16((float) value) {}

	inline operator float() const { return HalfToFloat(*this); };
	inline operator double() const { return (double) HalfToFloat(*this); };
	inline operator uint16_t() const { return m_Value; };
};

union FloatBits {
	float f;
	int32_t si;
	uint32_t ui;
};

static int const shift = 13;
static int const shiftSign = 16;

static int32_t const infN = 0x7F800000; // flt32 infinity
static int32_t const maxN = 0x477FE000; // max float16 normal as a float
static int32_t const minN = 0x38800000; // min float16 normal as a float
static int32_t const signN = 0x80000000; // float sign bit

static int32_t const infC = infN >> shift;
static int32_t const nanN = (infC + 1) << shift; // minimum float16 nan as a float
static int32_t const maxC = maxN >> shift;
static int32_t const minC = minN >> shift;
static int32_t const signC = signN >> shiftSign; // float16 sign bit

static int32_t const mulN = 0x52000000; // (1 << 23) / minN
static int32_t const mulC = 0x33800000; // minN / (1 << (23 - shift))

static int32_t const subC = 0x003FF; // max float subnormal down shifted
static int32_t const norC = 0x00400; // min float normal down shifted

static int32_t const maxD = infC - maxC - 1;
static int32_t const minD = minC - subC - 1;
#endif