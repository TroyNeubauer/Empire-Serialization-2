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


const char DIGITS[36] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
						  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
const int MAX_BASE = sizeof(DIGITS);


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
typedef signed __int128 s128;
typedef unsigned __int128 u128;

#else
namespace Empire {
union u128;
union s128;

std::string to_string(u128 value);
std::string to_string(s128 value);
void FromString(const char* string, u64 length, u8 base, u128& result);
void FromString(const char* string, u64 length, u8 base, s128& result);

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
	u128(const char* string, u64 length, u8 base) {
		FromString(string, length, base, *this);
	}
	u128(const char* string, u8 base = 10) {
		u128(string, std::strlen(string), base);
	}
	u128(std::string string, u8 base = 10) {
		u128(string.c_str(), string.length(), base);
	}

	inline explicit operator u64() const {
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			return this->_64[0];
		} else {
			return this->_64[1];
		}
	}
	inline explicit operator s64() const {
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			return this->_64[0] & 0x8FFFFFFFFFFFFFFF;// make sure the sign bit is a zero
		} else {
			return this->_64[1] & 0x8FFFFFFFFFFFFFFF;// make sure the sign bit is a zero
		}
	}
	inline u64 Top64() {
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			return this->_64[1];
		}
		else {
			return this->_64[0];
		}
	}
	inline u64 Bottom64() {
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {
			return this->_64[0];
		} else {
			return this->_64[1];
		}
	}
	inline const bool operator<(u64 other) {
		return Bottom64() < other;
	}
	inline const bool operator>(u64 other) {
		return Bottom64() > other;
	}
	inline const bool operator<=(u64 other) {
		return Bottom64() <= other;
	}
	inline const bool operator>=(u64 other) {
		return Bottom64() >= other;
	}

	inline const bool operator<(u128 other) {
		if (this->Top64() < other.Top64()) return true;
		else return this->Bottom64() < other.Bottom64();
	}
	inline const bool operator>(u128 other) {
		if (this->Top64() > other.Top64()) return true;
		else return this->Bottom64() > other.Bottom64();
	}
	inline const bool operator<=(u128 other) {
		if (*this == other) return true;
		if (this->Top64() < other.Top64()) return true;
		else return this->Bottom64() < other.Bottom64();
	}
	inline const bool operator>=(u128 other) {
		if (*this == other) return true;
		if (this->Top64() > other.Top64()) return true;
		else return this->Bottom64() > other.Bottom64();
	}


	inline const bool operator==(const u128 other) {
		return this->_64[0] == other._64[0] && this->_64[1] == other._64[1];
	}
	inline const bool operator!=(const u128 other) {
		return !(*this == other);
	}
	inline u128 operator=(const u128 other) {
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
	const u128 operator<<(u64 bits) {
		u128 result;
		ShiftLeft(bits, result);
		return result;
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
	const u128 operator>>(u64 bits) {
		u128 result;
		ShiftRight(bits, result);
		return result;
	}
	u128 operator>>=(u64 bits) {
		ShiftRight(bits, *this);
		return *this;
	}
	u128 operator>>=(s64 bits) {
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
			result._32[0] = MutiplyWithCarry(this->_32[0], other, carry);
			result._32[1] = MutiplyWithCarry(this->_32[1], other, carry);
			result._32[2] = MutiplyWithCarry(this->_32[2], other, carry);
			result._32[3] = MutiplyWithCarry(this->_32[3], other, carry);
		} else {//Start from the higher addresses - we are BE
			result._32[3] = MutiplyWithCarry(this->_32[3], other, carry);
			result._32[2] = MutiplyWithCarry(this->_32[2], other, carry);
			result._32[1] = MutiplyWithCarry(this->_32[1], other, carry);
			result._32[0] = MutiplyWithCarry(this->_32[0], other, carry);
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

	inline u32 DivideWithRemainder(const u32 a, const u32 b, u32& remainder) {
		u64 helper = (((u64) remainder) << 32UL) | a;
		remainder = helper % b;
		return (u32)(helper / b);
	}
	void Divide(const u32 other, u128& result) {
		u32 remainder = 0;
		if (SYSTEM_ENDIANESS == Endianness::LITTLE) {//Start from higher addresses since thats where the higher values are
			result._32[3] = DivideWithRemainder(this->_32[3], other, remainder);
			result._32[2] = DivideWithRemainder(this->_32[2], other, remainder);
			result._32[1] = DivideWithRemainder(this->_32[1], other, remainder);
			result._32[0] = DivideWithRemainder(this->_32[0], other, remainder);
		}
		else {//Start from the lower addresses - we are BE
			result._32[0] = DivideWithRemainder(this->_32[0], other, remainder);
			result._32[1] = DivideWithRemainder(this->_32[1], other, remainder);
			result._32[2] = DivideWithRemainder(this->_32[2], other, remainder);
			result._32[3] = DivideWithRemainder(this->_32[3], other, remainder);
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

	std::string ToString() {
		return to_string(*this);
	}
};//u128


union s128 {

	std::string ToString() {
		return to_string(*this);
	}
};

#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
static const u8 LogTable256[256] =
{
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
	LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};

u64 log10(u128 value) {
	if (value < 1000000) {
		if        (value < 10) {
			return 0;
		} else if (value < 100) {
			return 1;
		} else if (value < 1000) {
			return 2;
		} else if (value < 10000) {
			return 3;
		} else if (value < 100000) {
			return 4;
		} else if (value < 1000000) {
			return 5;
		}
	} else {

	}
}

u64 log2(u128 value) {//About 30 instructions to compute
	register u64 t, tt, ttt, tttt; // temporaries
	if (tttt = value.Top64()) {
		if (ttt = tttt >> 32) {//upper 32
			if (tt = ttt >> 16) {//upper 16
				if (t = tt >> 8)	return 120 + LogTable256[t];
				else				return 112 + LogTable256[tt];
			} else {//lower 16
				if (t = ttt >> 8)	return 104 + LogTable256[t]; 
				else				return 96 + LogTable256[ttt];
			}
		} else {//lower 32
			if (tt = static_cast<u64>(value >> 16UL)) {
				if (t = tt >> 8)	return 88 + LogTable256[t];
				else				return 80 + LogTable256[tt];
			} else {
				if (t = static_cast<u64>(value >> 8UL)) 
									return 72 + LogTable256[t];
				else				return 64 + LogTable256[static_cast<u64>(value)];
			}
		}
	} else {
		tttt = value.Bottom64();
		if (ttt = tttt >> 32) {
			if (tt = ttt >> 16) {
				if (t = tt >> 8)	return 56 + LogTable256[t];
				else				return 48 + LogTable256[tt];
			} else {
				if (t = ttt >> 8)	return 40 + LogTable256[t];
				else				return 32 + LogTable256[ttt];
			}
		} else {
			if (tt = static_cast<u64>(value >> 16UL)) {
				if (t = tt >> 8)	return 24 + LogTable256[t];
				else				return 16 + LogTable256[tt];
			} else {
				if (t = static_cast<u64>(value >> 8UL)) 
									return 8 + LogTable256[t];
				else				return LogTable256[static_cast<u64>(value)];
			}
		}
	}
}


std::string to_string(u128 value) {
	return "TODO";
}

std::string to_string(s128 value) {
	return "TODO";
}

void FromString(const char* string, u64 length, u8 base, u128& result) {

}

void FromString(const char* string, u64 length, u8 base, s128& result) {

}

#endif//#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)

std::ostream& operator<<(std::ostream& os, const u128 value) noexcept
{
	if (os.flags() & std::ios_base::hex) {
		char buffer[36];
		int index = 0;
		for (int i = 0; i < sizeof(u128); i++) {
			if ((i != 0) && (i % (sizeof(u128) / 2)) == 0) {// Put an extra space half way through
				buffer[index++] = ' ';
			}
			if ((i != 0) && (i % (sizeof(u128) / 4)) == 0) {// Put a space every 32 bits
				buffer[index++] = ' ';
			}

			char part;// We put the higher chars in the buffer first...
			if (SYSTEM_ENDIANESS == Endianness::LITTLE)
				part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
			else
				part = value._8[i];// The higher parts start at lower indices
			
			buffer[index++] = DIGITS[(part >> 4) & 0b1111];
			buffer[index++] = DIGITS[(part >> 0) & 0b1111];
		}
		assert(index == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
	}
	else if (os.flags() & std::ios_base::oct) {//Print binary instead of oct
		char buffer[148];
		int index = 0;
		for (int i = 0; i < sizeof(u128); i++) {
			if ((i != 0) && (i % (sizeof(u128) / 2)) == 0) {// Put an extra space half way through
				buffer[index++] = ' ';
			}
			if ((i != 0) && (i % (sizeof(u128) / 4)) == 0) {// Put a space every 32 bits
				buffer[index++] = ' ';
			}

			char part;// We put the higher chars in the buffer first...
			if (SYSTEM_ENDIANESS == Endianness::LITTLE) 
				part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
			else 
				part = value._8[i];// The higher parts start at lower indices
			
			for (int bit = 7; bit >= 0; bit--) {
				buffer[index++] = DIGITS[(part >> bit) & 0b1];
			}
			buffer[index++] = ' ';// Put a space every byte
		}
		assert(index == sizeof(buffer));
		os.write(buffer, sizeof(buffer));
	}
	return os;
}



}//namespace