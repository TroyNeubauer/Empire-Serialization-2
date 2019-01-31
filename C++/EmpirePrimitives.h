#pragma once

#include <stdint.h>
#include <iostream>
#include <ios>
#include <string>

#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <intrin.h>
#include <wmmintrin.h>



#define EMPIRE_ENABLE_INTEL_INTRINSICS 1


#if !EMPIRE_ENABLE_INTEL_INTRINSICS
#error Cannot compile without intrinsics! intrinsics needed for addition and subtraction
#endif

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)
typedef __int128 s128;
typedef unsigned __int128 u128;

#else

union S {
	__m128i m128i;
	struct {
		u64 high, low;
	};
};


union u128 {
#ifdef _INCLUDED_EMM // __m128i is inside of this header file, so we know __m128i exists if the file is included
	#define EMPIRE_M128_UNION
	__m128i m128i;
#endif
	struct {
		u64 high, low;
	};
	u32 u128_u32[4];

	u128() {}
	u128(const u64 value) : high(0), low(value) {}
	u128(const u64 high, u64 low) : high(high), low(low) {}
	u128(const u128& value) : high(value.high), low(value.low) {}
	u128(const __m128i& value) : high(value.m128i_u64[0]), low(value.m128i_u64[1]) {}

	const __m128i To__m128i() {
		__m128i result;
		result.m128i_u64[0] = this->high;
		result.m128i_u64[1] = this->low;
		return result;
	}

	const bool operator==(const u128 other) {
		return this->high == other.high && this->low == other.low;
	}
	const bool operator!=(const u128 other) {
		return !(*this == other);
	}
	u128 operator=(const u128 other) {
		this->high = other.high;
		this->low = other.low;
		return *this;
	}
	void ShiftLeft(u64 bits, u128& result) {
		result.high = this->high << bits;
		u64 lowerOfUpper = this->low >> (sizeof(u64) * 8 - bits);
		result.high |= lowerOfUpper;
		result.low = this->low << bits;
	}
	u128 operator<<(u64 bits) {
		u128 result;
		ShiftLeft(bits, result);
		return *this;
	}
	u128 operator<<=(u64 bits) {
		ShiftLeft(bits, *this);
		return *this;
	}

	void ShiftRight(u64 bits, u128& result) {
		result.low = this->low >> bits;
		u64 upperOfLower = this->high << (sizeof(u64) * 8 - bits);
		result.low |= upperOfLower;
		result.high = this->high >> bits;
	}
	u128 operator>>(u64 bits) {
		u128 result;
		ShiftRight(bits, result);
		return result;
	}
	u128 operator>>=(u64 bits) {
		ShiftRight(bits, *this);
		return *this;
	}
	
	void Plus(const u128& other, u128& result) {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
		char carry = _addcarry_u64(0, this->low, other.low, &result.low);// Add low bits
		_addcarry_u64(carry, this->high, other.high, &result.high);// Add high bits with the carry from the low bits
#endif
	}
	const u128 operator+(const u128 other) {
		u128 result;
		Plus(other, result);
		return result;
	}
	u128 operator+=(const u128 other) {
		Plus(other, *this);
		return *this;
	}
	u128 operator++() {
		Plus(1, *this);
		return *this;
	}
	u128 operator++(int) {// Postfix
		u128 temp = *this;
		++(*this);
		return temp;
	}

	void Minus(const u128& other, u128& result) {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
		char carry = _subborrow_u64(0, this->low, other.low, &result.low);// Add low bits
		_subborrow_u64(carry, this->high, other.high, &result.high);// Add high bits with the carry from the low bits
#endif
	}

	const u128 operator-(const u128 other) {
		u128 result;
		Minus(other, result);
		return result;
	}
	u128 operator-=(const u128 other) {
		Minus(other, *this);
		return *this;
	}
	u128 operator--() {
		Minus(1, *this);
		return *this;
	}
	u128 operator--(int) {// Postfix
		u128 temp = *this;
		--(*this);
		return temp;
	}

	inline u32 MutiplyWithCarry(u32 a, u32 b, u32& carry) {
		u64 result = ((u64) a) * ((u64) b) + ((u64) carry);
		carry = result >> 32;
		return (u32) (result & 0xFFFFFF);
	}

	void Multiply(const u32& other, u128& result) {
		u32 carry = 0;
		this->u128_u32[3] = MutiplyWithCarry(this->u128_u32[3], other, carry);
		this->u128_u32[2] = MutiplyWithCarry(this->u128_u32[2], other, carry);
		this->u128_u32[1] = MutiplyWithCarry(this->u128_u32[1], other, carry);
		this->u128_u32[0] = MutiplyWithCarry(this->u128_u32[0], other, carry);
	}

	const u128 operator*(const u32 other) {
		u128 result;
		Multiply(other, result);
		return result;
	}
	u128 operator*=(const u32 other) {
		Multiply(other, *this);
		return *this;
	}

	inline u32 DivideWithRemainder(u32 a, u32 b, u32& remainder) {
		u64 helper = (remainder << 32) | a;
		remainder = helper % b;
		return (u32)((helper / b) & 0xFFFFFF);
	}
	void Divide(const u32 other, u128& result) {
		u32 remainder = 0;
		this->u128_u32[0] = DivideWithRemainder(this->u128_u32[0], other, remainder);
		this->u128_u32[1] = DivideWithRemainder(this->u128_u32[1], other, remainder);
		this->u128_u32[2] = DivideWithRemainder(this->u128_u32[2], other, remainder);
		this->u128_u32[3] = DivideWithRemainder(this->u128_u32[3], other, remainder);
	}

	const u128 operator/(const u32 other) {
		u128 result;
		Divide(other, result);
		return result;
	}
	u128 operator/=(const u32 other) {
		Divide(other, *this);
		return *this;
	}
};


struct s128 {

};

#endif//#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)

const char DIGITS[16] = {'0', '1', '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' };

std::ostream& operator<<(std::ostream& os, const u128 value) noexcept
{
	if (os.flags() & std::ios_base::hex) {
		char buffer[40];
		int i = 0;
		for (int byte = sizeof(u64) - 1; byte >= 0; byte--) {
			buffer[i++] = DIGITS[(value.high >> (byte * 8 + 4)) & 0b1111];
			buffer[i++] = DIGITS[(value.high >> (byte * 8 + 0)) & 0b1111];
			if ((byte != 0) && (byte % 2) == 0) {
				buffer[i++] = ' ';
			}
		}
		buffer[i++] = ' ';
		buffer[i++] = ' ';
		for (int byte = sizeof(u64) - 1; byte >= 0; byte--) {
			buffer[i++] = DIGITS[(value.low >> (byte * 8 + 4)) & 0b1111];
			buffer[i++] = DIGITS[(value.low >> (byte * 8 + 0)) & 0b1111];
			if ((byte != 0) && (byte % 2) == 0) {
				buffer[i++] = ' ';
			}
		}
		assert(i == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
	} else if (os.flags() & std::ios_base::oct) {//Print binary instead of oct
		char buffer[147];
		int i = 0;
		for (int bit = sizeof(u64)*8 - 1; bit >= 0; bit--) {
			buffer[i++] = DIGITS[(value.high >> bit) & 0b1];
			if ((bit != 0) && (bit % 8) == 0) {
				buffer[i++] = ' ';
			}
			if ((bit != 0) && (bit % 32) == 0) {
				buffer[i++] = ' ';
			}
		}
		buffer[i++] = ' ';
		buffer[i++] = ' ';
		buffer[i++] = ' ';
		for (int bit = sizeof(u64) * 8 - 1; bit >= 0; bit--) {
			buffer[i++] = DIGITS[(value.low >> bit) & 0b1];
			if ((bit != 0) && (bit % 8) == 0) {
				buffer[i++] = ' ';
			}
			if ((bit != 0) && (bit % 32) == 0) {
				buffer[i++] = ' ';
			}
		}
		assert(i == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
		
	}
	return os;
}
