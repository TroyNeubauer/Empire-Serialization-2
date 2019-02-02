#pragma once

#include "Empire.h"

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <ios>
#include <string>
#include <sstream>

#include <assert.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <intrin.h>
#include <wmmintrin.h>

const char DIGITS[36] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
						  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
const int MAX_BASE = sizeof(DIGITS);


#define EMPIRE_ENABLE_INTEL_INTRINSICS 1
#define EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING 1

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

	std::string to_string(u128 value, u8 base = 10);
	std::string to_string(s128 value, u8 base = 10);
	void FromString(const char* string, u64 length, u8 base, u128* result ERROR_CODE_PARAMETER);
	void FromString(const char* string, u64 length, u8 base, s128* result ERROR_CODE_PARAMETER);

	union u128 {
		u64 _64[2];
		u32 _32[4];
		u16 _16[8];
		u8  _8[16];

		u128() {}
		u128(const u64 value);

		u128(const u64 high, u64 low);

		u128(const u128& value);
		u128(const char* string, u64 length, u8 base ERROR_CODE_PARAMETER);

		u128(std::string string ERROR_CODE_PARAMETER, u8 base = 10) : u128(string.c_str(), string.length(), base ERROR_CODE_VAR) {}

		inline explicit operator u64() const;
		inline u64 Top64() const;
		inline u64 Bottom64() const;

		inline const bool operator<(u64 other) const;
		inline const bool operator>(u64 other) const;
		inline const bool operator<=(u64 other) const;
		inline const bool operator>=(u64 other) const;

		inline const bool operator<(u128 other) const;
		inline const bool operator>(u128 other) const;
		inline const bool operator<=(u128 other) const;
		inline const bool operator>=(u128 other) const;


		inline const bool operator==(const u128 other) const;
		inline const bool operator!=(const u128 other) const;

		inline u128 operator=(const u128 other);
		
		inline const bool operator==(const u64 other) const;
		inline const bool operator!=(const u64 other) const;
		inline u128 operator=(const u64 value);

		void ShiftLeft(u64 bits, u128& result) const;
		const u128 operator<<(u64 bits) const;
		u128 operator<<=(u64 bits);

		void ShiftRight(u64 bits, u128& result) const;
		const u128 operator>>(u64 bits) const;
		u128 operator>>=(u64 bits);

		void Plus(const u128& other, u128& result) const;
		const u128 operator+(const u128 other) const;
		u128 operator+=(const u128 other);
		u128 operator++();
		u128 operator++(int);//Postfix

		void Minus(const u128& other, u128& result) const;

		const u128 operator-(const u128 other) const;
		u128 operator-=(const u128 other);
		u128 operator--();
		u128 operator--(int);//Postfix

		inline u32 MutiplyWithCarry(u32 a, u32 b, u64& carry) const;

		void Multiply(const u32& other, u128& result) const;

		const u128 operator*(const u32 other) const;
		u128 operator*=(const u32 other);

		inline u32 DivideWithRemainder(const u32 a, const u32 b, u32& remainder) const;
		u32 Divide(const u32 other, u128& result) const;

		const u128 operator/(const u32 other) const;
		u128 operator/=(const u32 other);
		const u64 operator%(const u32 other) const;

		std::string ToString(u8 base = 10) const;
	};//u128


	union s128 {
		std::string ToString(u8 base = 10) const;
	};

	u64 log10(u128 value);
	u64 log2(u128 value);

}//namespace

#endif//#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)

