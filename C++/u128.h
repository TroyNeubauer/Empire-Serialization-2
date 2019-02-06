#pragma once
#include "EmpirePrimitives.h"

namespace Empire {

#if EMPIRE_SOFTWARE_U128
union u128;
#else
typedef unsigned __int128 u128;
#endif

std::string to_string(u128 value, u8 base = 10);
void FromString(const char* string, u64 length, u8 base, u128* result ERROR_CODE_PARAMETER);

u64 log10(u128 value);
u64 log2(u128 value);

#if EMPIRE_SOFTWARE_U128
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
	inline u64& Top64Ref();
	inline u64& Bottom64Ref();

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
	void Plus(const u64& other, u128& result) const;

	const u128 operator+(const u128 other) const;
	u128 operator+=(const u128 other);
	const u128 operator+(const u64 other) const;
	u128 operator+=(const u64 other);


	u128 operator++();
	u128 operator++(int);//Postfix

	void Minus(const u128& other, u128& result) const;
	void Minus(const u64& other, u128& result) const;

	const u128 operator-(const u128 other) const;
	u128 operator-=(const u128 other);
	const u128 operator-(const u64 other) const;
	u128 operator-=(const u64 other);

	u128 operator--();
	u128 operator--(int);//Postfix


	inline u32 MutiplyWithCarry(u32 a, u32 b, u64& carry) const;

	inline void Multiply(const u32& other, u128& result) const;
	inline void Multiply(const u128& other, u128& result) const;

	const u128 operator*(const u32 other) const;
	u128 operator*=(const u32 other);
	const u128 operator*(const u128 other) const;
	u128 operator*=(const u128 other);

	inline u32 DivideWithRemainder(const u32 a, const u32 b, u32& remainder) const;

	inline u32 Divide(const u32 other, u128& result) const;
	inline u32 Divide(const u128 other, u128& result) const;

	const u128 operator/(const u32 other) const;
	u128 operator/=(const u32 other);
	const u128 operator/(const u128 other) const;
	u128 operator/=(const u128 other);


	const u64 operator%(const u32 other) const;
	const u128 operator%(const u128 other) const;

	std::string ToString(u8 base = 10) const;
};//u128

#endif

}//namespace
