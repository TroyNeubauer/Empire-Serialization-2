#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <immintrin.h>
#include <emmintrin.h>
#include <intrin.h>

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

struct u128 {
	u64 high, low;
	u128() {}
	u128(const u64 value) : high(0), low(value) {}
	u128(const u64 high, u64 low) : high(high), low(low) {}
	u128(const u128& value) : high(value.high), low(value.low) {}

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

	void plus(const u128& other, u128& result) {
		char carry = _addcarry_u64(0, this->low, other.low, &result.low);// Add low bits
		_addcarry_u64(carry, this->high, other.high, &result.high);// Add high bits with the carry from the low bits
	}
	const u128 operator+(const u128 other) {
		u128 result;
		plus(other, result);
		return result;
	}
	u128 operator+=(const u128 other) {
		plus(other, *this);
	}
};


struct s128 {

};

#endif

const char DIGITS[16] = {'0', '1', '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' };

std::ostream& operator<<(std::ostream& os, const u128 value) noexcept
{
	char buffer[40];
	buffer[0] = DIGITS[((value.high >> 60) & 0x0F)];
	buffer[1] = DIGITS[((value.high >> 56) & 0x0F)];
	buffer[2] = DIGITS[((value.high >> 52) & 0x0F)];
	buffer[3] = DIGITS[((value.high >> 48) & 0x0F)];
	buffer[4] = ' ';
	buffer[5] = DIGITS[((value.high >> 44) & 0x0F)];
	buffer[6] = DIGITS[((value.high >> 40) & 0x0F)];
	buffer[7] = DIGITS[((value.high >> 36) & 0x0F)];
	buffer[8] = DIGITS[((value.high >> 32) & 0x0F)];
	buffer[9] = ' ';
	buffer[10] = DIGITS[((value.high >> 28) & 0x0F)];
	buffer[11] = DIGITS[((value.high >> 24) & 0x0F)];
	buffer[12] = DIGITS[((value.high >> 20) & 0x0F)];
	buffer[13] = DIGITS[((value.high >> 16) & 0x0F)];
	buffer[14] = ' ';
	buffer[15] = DIGITS[((value.high >> 12) & 0x0F)];
	buffer[16] = DIGITS[((value.high >>  8) & 0x0F)];
	buffer[17] = DIGITS[((value.high >>  4) & 0x0F)];
	buffer[18] = DIGITS[((value.high >>  0) & 0x0F)];

	buffer[19] = ' ';
	buffer[20] = ' ';

	buffer[21] = DIGITS[((value.low >> 60) & 0x0F)];
	buffer[22] = DIGITS[((value.low >> 56) & 0x0F)];
	buffer[23] = DIGITS[((value.low >> 52) & 0x0F)];
	buffer[24] = DIGITS[((value.low >> 48) & 0x0F)];
	buffer[25] = ' ';
	buffer[26] = DIGITS[((value.low >> 44) & 0x0F)];
	buffer[27] = DIGITS[((value.low >> 40) & 0x0F)];
	buffer[28] = DIGITS[((value.low >> 36) & 0x0F)];
	buffer[29] = DIGITS[((value.low >> 32) & 0x0F)];
	buffer[30] = ' ';
	buffer[31] = DIGITS[((value.low >> 28) & 0x0F)];
	buffer[32] = DIGITS[((value.low >> 24) & 0x0F)];
	buffer[33] = DIGITS[((value.low >> 20) & 0x0F)];
	buffer[34] = DIGITS[((value.low >> 16) & 0x0F)];
	buffer[35] = ' ';
	buffer[36] = DIGITS[((value.low >> 12) & 0x0F)];
	buffer[37] = DIGITS[((value.low >> 8) & 0x0F)];
	buffer[38] = DIGITS[((value.low >> 4) & 0x0F)];
	buffer[39] = DIGITS[((value.low >> 0) & 0x0F)];
	os.write(buffer, sizeof(buffer));
	
	return os;
}
