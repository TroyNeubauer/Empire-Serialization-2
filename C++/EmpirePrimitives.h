#pragma once

#include "Empire.h"

#include <stdint.h>
#include <stdlib.h>
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
namespace Empire {


union u128 {
	u64 _64[2];
	u32 _32[4];
	u16 _16[8];
	u8  _8[16];

	u128() {}
	u128(const u64 value) {
		u128* t = this;
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			this->_64[0] = value;
		} else {
			this->_64[1] = value;
		}
	}

	u128(const u64 high, u64 low) {
		u128* t = this;
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			this->_64[1] = high;
			this->_64[0] = low;
		} else {
			this->_64[0] = high;
			this->_64[1] = low;
		}
	}

	u128(const u128& value) {
		u128* t = this;
		this->_64[0] = value._64[0];
		this->_64[1] = value._64[1];
	}

	const bool operator==(const u128 other) {
		return this->_64[0] == other._64[0] && this->_64[1] == other._64[1];
	}
	const bool operator!=(const u128 other) {
		return !(*this == other);
	}
	u128 operator=(const u128 other) {
		this->_64[0] = other._64[0];
		this->_64[1] = other._64[1];
		return *this;
	}

	//Little 00 11 22 33 44 55 66 77    88 99 AA BB CC DD EE FF - as 128 bit
	//Big    FF EE DD CC BB AA 99 88    77 66 55 44 33 22 11 00 - ad 128 bit
	void ShiftLeft(u64 bits, u128& result) {
		result._64[0] = this->_64[0] << bits;
		u64 lowerOfUpper = this->_64[1] >> (sizeof(u64) * 8 - bits);
		result._64[0] |= lowerOfUpper;
		result._64[1] = this->_64[1] << bits;
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
		result._64[1] = this->_64[1] >> bits;
		u64 upperOfLower = this->_64[0] << (sizeof(u64) * 8 - bits);
		result._64[1] |= upperOfLower;
		result._64[0] = this->_64[0] >> bits;
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
		char carry = _addcarry_u64(0, this->_64[1], other._64[1], &result._64[1]);// Add low bits
		_addcarry_u64(carry, this->_64[0], other._64[0], &result._64[0]);// Add high bits with the carry from the low bits
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
		char carry = _subborrow_u64(0, this->_64[1], other._64[1], &result._64[1]);// Add low bits
		_subborrow_u64(carry, this->_64[0], other._64[0], &result._64[0]);// Add high bits with the carry from the low bits
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

	inline u32 MutiplyWithCarry(u32 a, u32 b, u64& carry) {
		u64 total = ((u64)a) * ((u64)b) + ((u64)carry);
		carry = total >> 32;
		u32 result = (u32)total;
		return result;
	}

	void Multiply(const u32& other, u128& result) {
		u64 carry = 0;
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {//Start from lower addresses since thats where the lower values are
			this->_32[0] = MutiplyWithCarry(this->_32[0], other, carry);
			this->_32[1] = MutiplyWithCarry(this->_32[1], other, carry);
			this->_32[2] = MutiplyWithCarry(this->_32[2], other, carry);
			this->_32[3] = MutiplyWithCarry(this->_32[3], other, carry);
		} else {//Start from the higher addresses - we are BE
			this->_32[3] = MutiplyWithCarry(this->_32[3], other, carry);
			this->_32[2] = MutiplyWithCarry(this->_32[2], other, carry);
			this->_32[1] = MutiplyWithCarry(this->_32[1], other, carry);
			this->_32[0] = MutiplyWithCarry(this->_32[0], other, carry);
		}
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
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {//Start from higher addresses since thats where the higher values are
			this->_32[3] = DivideWithRemainder(this->_32[3], other, remainder);
			this->_32[2] = DivideWithRemainder(this->_32[2], other, remainder);
			this->_32[1] = DivideWithRemainder(this->_32[1], other, remainder);
			this->_32[0] = DivideWithRemainder(this->_32[0], other, remainder);
		}
		else {//Start from the lower addresses - we are BE
			this->_32[0] = DivideWithRemainder(this->_32[0], other, remainder);
			this->_32[1] = DivideWithRemainder(this->_32[1], other, remainder);
			this->_32[2] = DivideWithRemainder(this->_32[2], other, remainder);
			this->_32[3] = DivideWithRemainder(this->_32[3], other, remainder);
		}
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

const char DIGITS[16] = { '0', '1', '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' };

std::ostream& operator<<(std::ostream& os, const u128 value) noexcept
{
	if (os.flags() & std::ios_base::hex) {
		char buffer[36];
		int index = 0;
		for (int i = 0; i < sizeof(u128); i++) {
			char part;// We put the higher chars in the buffer first...
			if (SYSTEM_ENDIANESS == Endianness::LITTLE)
				part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
			else
				part = value._8[i];// The higher parts start at lower indices
			
			buffer[index++] = DIGITS[(part >> 4) & 0b1111];
			buffer[index++] = DIGITS[(part >> 0) & 0b1111];

			if ((i != 0) && (i % (sizeof(u128) / 2)) == 0) {// Put an extra space half way through
				buffer[index++] = ' ';
			}
			if ((i != 0) && (i % (sizeof(u128) / 4)) == 0) {// Put a space every 32 bits
				buffer[index++] = ' ';
			}
		}
		assert(index == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
	}
	else if (os.flags() & std::ios_base::oct) {//Print binary instead of oct
		char buffer[148];
		int index = 0;
		for (int i = 0; i < sizeof(u128); i++) {
			char part;// We put the higher chars in the buffer first...
			if (SYSTEM_ENDIANESS == Endianness::LITTLE) 
				part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
			else 
				part = value._8[i];// The higher parts start at lower indices
			
			for (int bit = 7; bit >= 0; bit--) {
				buffer[index++] = DIGITS[(part >> bit) & 0b1];
			}

			buffer[index++] = ' ';// Put a space every byte
			if ((i != 0) && (i % (sizeof(u128) / 2)) == 0) {// Put an extra space half way through
				buffer[index++] = ' ';
			}
			if ((i != 0) && (i % (sizeof(u128) / 4)) == 0) {// Put a space every 32 bits
				buffer[index++] = ' ';
			}
		}
		assert(index == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
	}
	return os;
}



}//namespace