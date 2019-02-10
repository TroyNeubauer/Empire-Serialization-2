#pragma once

#include <string>

#include "../EmpireException.h"
#include "EmpirePrimitivesConfig.h"
#include "EmpirePrimitives.h"

using namespace Empire;

std::string to_string(uint128_t value, u8 base = 10);
void FromString(const char* string, uint64_t length, u8 base, uint128_t* result EMPIRE_ERROR_PARAMETER);

uint64_t log10(uint128_t value);
uint64_t log2(uint128_t value);

#if EMPIRE_SOFTWARE_UINT_128
union uint128_t {
	uint64_t _64[2];
	uint32_t _32[4];
	uint16_t _16[8];
	uint8_t  _8[16];

	uint128_t() {}
	uint128_t(const uint64_t value);

	uint128_t(const uint64_t high, uint64_t low);

	uint128_t(const uint128_t& value);
	uint128_t(const char* string, uint64_t length, u8 base EMPIRE_ERROR_PARAMETER);

	uint128_t(std::string string EMPIRE_ERROR_PARAMETER, u8 base = 10) : uint128_t(string.c_str(), string.length(), base EMPIRE_ERROR_VAR) {}

	inline explicit operator uint64_t() const;
	inline uint64_t Top64() const;
	inline uint64_t Bottom64() const;
	inline uint64_t& Top64Ref();
	inline uint64_t& Bottom64Ref();

	inline const bool operator<(uint64_t other) const;
	inline const bool operator>(uint64_t other) const;
	inline const bool operator<=(uint64_t other) const;
	inline const bool operator>=(uint64_t other) const;

	inline const bool operator<(uint128_t other) const;
	inline const bool operator>(uint128_t other) const;
	inline const bool operator<=(uint128_t other) const;
	inline const bool operator>=(uint128_t other) const;


	inline const bool operator==(const uint128_t other) const;
	inline const bool operator!=(const uint128_t other) const;

	inline uint128_t operator=(const uint128_t other);

	inline const bool operator==(const uint64_t other) const;
	inline const bool operator!=(const uint64_t other) const;
	inline uint128_t operator=(const uint64_t value);

	void ShiftLeft(uint64_t bits, uint128_t& result) const;
	const uint128_t operator<<(uint64_t bits) const;
	uint128_t operator<<=(uint64_t bits);

	void ShiftRight(uint64_t bits, uint128_t& result) const;
	const uint128_t operator>>(uint64_t bits) const;
	uint128_t operator>>=(uint64_t bits);

	void Plus(const uint128_t& other, uint128_t& result) const;
	void Plus(const uint64_t& other, uint128_t& result) const;

	const uint128_t operator+(const uint128_t other) const;
	uint128_t operator+=(const uint128_t other);
	const uint128_t operator+(const uint64_t other) const;
	uint128_t operator+=(const uint64_t other);


	uint128_t operator++();
	uint128_t operator++(int);//Postfix

	void Minus(const uint128_t& other, uint128_t& result) const;
	void Minus(const uint64_t& other, uint128_t& result) const;

	const uint128_t operator-(const uint128_t other) const;
	uint128_t operator-=(const uint128_t other);
	const uint128_t operator-(const uint64_t other) const;
	uint128_t operator-=(const uint64_t other);

	uint128_t operator--();
	uint128_t operator--(int);//Postfix


	inline uint32_t MutiplyWithCarry(uint32_t a, uint32_t b, uint64_t& carry) const;

	inline void Multiply(const uint32_t& other, uint128_t& result) const;
	inline void Multiply(const uint128_t& other, uint128_t& result) const;

	const uint128_t operator*(const uint32_t other) const;
	uint128_t operator*=(const uint32_t other);
	const uint128_t operator*(const uint128_t other) const;
	uint128_t operator*=(const uint128_t other);

	inline uint32_t DivideWithRemainder(const uint32_t a, const uint32_t b, uint32_t& remainder) const;

	inline uint32_t Divide(const uint32_t other, uint128_t& result) const;
	inline uint32_t Divide(const uint128_t other, uint128_t& result) const;

	const uint128_t operator/(const uint32_t other) const;
	uint128_t operator/=(const uint32_t other);
	const uint128_t operator/(const uint128_t other) const;
	uint128_t operator/=(const uint128_t other);


	const uint64_t operator%(const uint32_t other) const;
	const uint128_t operator%(const uint128_t other) const;

	std::string ToString(u8 base = 10) const;
};//uint128_t

#endif
