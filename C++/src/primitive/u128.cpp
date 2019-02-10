#include "u128.h"

#include <intrin.h>
#include <sstream>
#include <assert.h>

#include "../EmpireErrorCodes.h"

namespace Empire {

#if EMPIRE_SOFTWARE_U128

u128::u128(const u64 value) {
	u128* t = this;
#ifdef LITTLE_ENDIAN
	this->_64[0] = value;
	this->_64[1] = 0;
#else
	this->_64[1] = value;
	this->_64[0] = 0;
#endif
}

u128::u128(const u64 high, u64 low) {
	u128* t = this;
#ifdef LITTLE_ENDIAN
	this->_64[1] = high;
	this->_64[0] = low;
#else
	this->_64[0] = high;
	this->_64[1] = low;
#endif
}

u128::u128(const u128& value) {
	u128* t = this;
	this->_64[0] = value._64[0];
	this->_64[1] = value._64[1];
}
u128::u128(const char* string, u64 length, u8 base EMPIRE_ERROR_CODE_PARAMETER) {
	FromString(string, length, base, this EMPIRE_ERROR_CODE_VAR);
}

inline u128::operator u64() const {
#ifdef LITTLE_ENDIAN
	return this->_64[0];
#else
	return this->_64[1];
#endif
}


inline u64 u128::Top64() const {
#ifdef LITTLE_ENDIAN
	return this->_64[1];
#else
	return this->_64[0];
#endif
}

inline u64 u128::Bottom64() const {
#ifdef LITTLE_ENDIAN
	return this->_64[0];
#else
	return this->_64[1];
#endif
}

inline u64& u128::Top64Ref() {
#ifdef LITTLE_ENDIAN
	return this->_64[1];
#else
	return this->_64[0];
#endif
}
inline u64& u128::Bottom64Ref() {
#ifdef LITTLE_ENDIAN
	return this->_64[0];
#else
	return this->_64[1];
#endif
}

inline const bool u128::operator<(u64 other) const {
	return Bottom64() < other && Top64() == 0;
}
inline const bool u128::operator>(u64 other) const {
	return Bottom64() > other || Top64() > 0;
}
inline const bool u128::operator<=(u64 other) const {
	return Bottom64() <= other && Top64() == 0;
}
inline const bool u128::operator>=(u64 other) const {
	return Bottom64() >= other || Top64() > 0;
}

inline const bool u128::operator<(u128 other) const {
	if (this->Top64() == other.Top64()) {
		return this->Bottom64() < other.Bottom64();
	}
	else if (this->Top64() < other.Top64()) {
		return true;
	}
	else {//this->Top64() > other.Top64()
		return false;
	}
}
inline const bool u128::operator>(u128 other) const {
	if (this->Top64() == other.Top64()) {
		return this->Bottom64() > other.Bottom64();
	}
	else if (this->Top64() > other.Top64()) {
		return true;
	}
	else {//this->Top64() < other.Top64()
		return false;
	}
}
inline const bool u128::operator<=(u128 other) const {
	if (*this == other) return true;
	return *this < other;
}
inline const bool u128::operator>=(u128 other) const {
	if (*this == other) return true;
	return *this > other;
}


inline const bool u128::operator==(const u128 other) const {
	return this->Bottom64() == other.Bottom64() && this->Top64() == other.Top64();
}
inline const bool u128::operator!=(const u128 other) const {
	return !(*this == other);
}

inline const bool u128::operator==(const u64 other) const {
	return this->Top64() == 0 && (this->Bottom64() == other);
}
inline const bool u128::operator!=(const u64 other) const {
	return !(*this == other);
}

inline u128 u128::operator=(const u128 other) {
	this->Bottom64Ref() = other.Bottom64();
	this->Top64Ref() = other.Top64();
	return *this;
}

inline u128 u128::operator=(const u64 value) {
	this->Bottom64Ref() = value;
	this->Top64Ref() = 0;

	return *this;
}

//Little 00 11 22 33 44 55 66 77    88 99 AA BB CC DD EE FF - as 128 bit
//Big    FF EE DD CC BB AA 99 88    77 66 55 44 33 22 11 00 - ad 128 bit
void u128::ShiftLeft(u64 bits, u128& result) const {
	result.Bottom64Ref() = this->Bottom64() << bits;
	u64 lowerOfUpper = this->Top64() >> (sizeof(u64) * 8 - bits);
	result.Bottom64Ref() |= lowerOfUpper;
	result.Top64Ref() = this->Top64() << bits;
}
const u128 u128::operator<<(u64 bits) const {
	u128 result;
	ShiftLeft(bits, result);
	return result;
}
u128 u128::operator<<=(u64 bits) {
	ShiftLeft(bits, *this);
	return *this;
}

void u128::ShiftRight(u64 bits, u128& result) const {
	result.Top64Ref() = this->Top64() >> bits;
	u64 upperOfLower = this->Bottom64() << (sizeof(u64) * 8 - bits);
	result.Top64Ref() |= upperOfLower;
	result.Bottom64Ref() = this->Bottom64() >> bits;
}
const u128 u128::operator>>(u64 bits) const {
	u128 result;
	ShiftRight(bits, result);
	return result;
}
u128 u128::operator>>=(u64 bits) {
	ShiftRight(bits, *this);
	return *this;
}

void u128::Plus(const u128& other, u128& result) const {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
	char carry = _addcarry_u64(0, this->Bottom64(), other.Bottom64(), &result.Bottom64Ref());// Add low bits
	_addcarry_u64(carry, this->Top64(), other.Top64(), &result.Top64Ref());// Add high bits with the carry from the low bits
#endif
}

void u128::Plus(const u64& other, u128& result) const {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
	char carry = _addcarry_u64(0, this->Bottom64(), other, &result.Bottom64Ref());// Add low bits
	_addcarry_u64(carry, this->Top64(), 0, &result.Top64Ref());// Add high bits with the carry from the low bits
#endif
}

const u128 u128::operator+(const u128 other) const {
	u128 result;
	Plus(other, result);
	return result;
}
u128 u128::operator+=(const u128 other) {
	Plus(other, *this);
	return *this;
}
const u128 u128::operator+(const u64 other) const {
	u128 result;
	Plus(other, result);
	return result;
}
u128 u128::operator+=(const u64 other) {
	Plus(other, *this);
	return *this;
}

u128 u128::operator++() {
	Plus(1, *this);
	return *this;
}
u128 u128::operator++(int) {// Postfix
	u128 temp = *this;
	++(*this);
	return temp;
}

void u128::Minus(const u128& other, u128& result) const {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
	char barrow = _subborrow_u64(0, this->Bottom64(), other.Bottom64(), &result.Bottom64Ref());// Subtract the low bits
	_subborrow_u64(barrow, this->Top64(), other.Top64(), &result.Top64Ref());// Subtract the high bits with the barrow
#endif
}
void u128::Minus(const u64& other, u128& result) const {
#if EMPIRE_ENABLE_INTEL_INTRINSICS
	char barrow = _subborrow_u64(0, this->Bottom64(), other, &result.Bottom64Ref());// Subtract the low bits
	_subborrow_u64(barrow, this->Top64(), 0, &result.Top64Ref());// Subtract the high bits with the barrow
#endif
}

const u128 u128::operator-(const u128 other) const {
	u128 result;
	Minus(other, result);
	return result;
}
u128 u128::operator-=(const u128 other) {
	Minus(other, *this);
	return *this;
}
const u128 u128::operator-(const u64 other) const {
	u128 result;
	Minus(other, result);
	return result;
}
u128 u128::operator-=(const u64 other) {
	Minus(other, *this);
	return *this;
}

u128 u128::operator--() {
	Minus(1, *this);
	return *this;
}
u128 u128::operator--(int) {// Postfix
	u128 temp = *this;
	--(*this);
	return temp;
}

inline u32 u128::MutiplyWithCarry(u32 a, u32 b, u64& carry) const {
	u64 total = ((u64)a) * ((u64)b) + ((u64)carry);
	carry = total >> 32;
	u32 result = (u32)total;
	return result;
}

void u128::Multiply(const u32& other, u128& result) const {
	u64 carry = 0;
#ifdef LITTLE_ENDIAN //Start from lower addresses since thats where the lower values are
	result._32[0] = MutiplyWithCarry(this->_32[0], other, carry);
	result._32[1] = MutiplyWithCarry(this->_32[1], other, carry);
	result._32[2] = MutiplyWithCarry(this->_32[2], other, carry);
	result._32[3] = MutiplyWithCarry(this->_32[3], other, carry);
#else//Start from the higher addresses - we are BE
	result._32[3] = MutiplyWithCarry(this->_32[3], other, carry);
	result._32[2] = MutiplyWithCarry(this->_32[2], other, carry);
	result._32[1] = MutiplyWithCarry(this->_32[1], other, carry);
	result._32[0] = MutiplyWithCarry(this->_32[0], other, carry);
#endif

}

inline void u128::Multiply(const u128& other, u128& result) const {
	u64 temp = 0;
	result.Bottom64Ref() = _umul128(this->Bottom64(), other.Bottom64(), &result.Top64Ref());
	result.Bottom64Ref() += _umul128(this->Bottom64(), other.Top64(), &temp);
	result.Top64Ref() += temp;// Add the top part of the last mul

	result.Top64Ref() += this->Top64() * other.Bottom64();
	result.Top64Ref() += this->Top64() * other.Top64();
}


const u128 u128::operator*(const u32 other) const {
	u128 result;
	Multiply(other, result);
	return result;
}
u128 u128::operator*=(const u32 other) {
	Multiply(other, *this);
	return *this;
}
const u128 u128::operator*(const u128 other) const {
	u128 result;
	Multiply(other, result);
	return result;
}
u128 u128::operator*=(const u128 other) {
	Multiply(other, *this);
	return *this;
}

inline u32 u128::DivideWithRemainder(const u32 a, const u32 b, u32& remainder) const {
	u64 helper = (((u64)remainder) << 32UL) | a;
	remainder = helper % b;
	return (u32)(helper / b);
}
u32 u128::Divide(const u32 other, u128& result) const {
	u32 remainder = 0;
#ifdef LITTLE_ENDIAN//Start from higher addresses since thats where the higher values are
	result._32[3] = DivideWithRemainder(this->_32[3], other, remainder);
	result._32[2] = DivideWithRemainder(this->_32[2], other, remainder);
	result._32[1] = DivideWithRemainder(this->_32[1], other, remainder);
	result._32[0] = DivideWithRemainder(this->_32[0], other, remainder);
#else//Start from the lower addresses - we are BE
	result._32[0] = DivideWithRemainder(this->_32[0], other, remainder);
	result._32[1] = DivideWithRemainder(this->_32[1], other, remainder);
	result._32[2] = DivideWithRemainder(this->_32[2], other, remainder);
	result._32[3] = DivideWithRemainder(this->_32[3], other, remainder);
#endif
	return remainder;
}
inline u32 u128::Divide(const u128 other, u128& result) const {

}

const u128 u128::operator/(const u32 other) const {
	u128 result;
	Divide(other, result);
	return result;
}
u128 u128::operator/=(const u32 other) {
	Divide(other, *this);
	return *this;
}
const u64 u128::operator%(const u32 other) const {
	u128 result;
	return Divide(other, result);
}

std::string u128::ToString(u8 base) const {
	return to_string(*this, base);
}

#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
static const u8 LogTable256[256] =
{
	0xFF, 0, 1, 1,
	2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	LT(4),
	LT(5), LT(5),
	LT(6), LT(6), LT(6), LT(6),
	LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};

u64 log10(u128 value) {
	if (value < 10)					return  0;
	else if (value < 100)					return  1;
	else if (value < 1000)					return  2;
	else if (value < 10000)					return  3;
	else if (value < 100000)				return  4;
	else if (value < 1000000)				return  5;
	else if (value < 10000000)				return  6;
	else if (value < 100000000)				return  7;
	else if (value < 1000000000)			return  8;
	else if (value < 10000000000)			return  9;
	else if (value < 100000000000)			return 10;
	else if (value < 1000000000000)			return 11;
	else if (value < 10000000000000)		return 12;
	else if (value < 100000000000000)		return 13;
	else if (value < 1000000000000000)		return 14;
	else if (value < 10000000000000000)		return 15;
	else if (value < 100000000000000000)	return 16;
	else if (value < 1000000000000000000)	return 17;
	else if (value < 10000000000000000000)	return 18;
	else if (value < u128(0x5, 0x6BC75E2D63100000)) {//56BC75E2D63100000 == 100000000000000000000
		return 19;
	}
	else if (value < u128(0x36, 0x35C9ADC5DEA00000)) {//3635C9ADC5DEA00000 == 1000000000000000000000
		return 20;
	}
	else if (value < u128(0x21E, 0x19E0C9BAB2400000)) {//21E19E0C9BAB2400000 == 10000000000000000000000
		return 21;
	}
	else if (value < u128(0x152D, 0x02C7E14AF6800000)) {//152D02C7E14AF6800000 == 100000000000000000000000
		return 22;
	}
	else if (value < u128(0xD3C2, 0x1BCECCEDA1000000)) {//D3C21BCECCEDA1000000 == 1000000000000000000000000
		return 23;
	}
	else if (value < u128(0x84595, 0x161401484A000000)) {//84595161401484A000000 == 10000000000000000000000000
		return 24;
	}
	else if (value < u128(0x52B7D2, 0xDCC80CD2E4000000)) {//52B7D2DCC80CD2E4000000 == 100000000000000000000000000
		return 25;
	}
	else if (value < u128(0x33B2E3C, 0x9FD0803CE8000000)) {//33B2E3C9FD0803CE8000000 == 1000000000000000000000000000
		return 26;
	}
	else if (value < u128(0x204FCE5E, 0x3E25026110000000)) {//204FCE5E3E25026110000000 == 10000000000000000000000000000
		return 27;
	}
	else if (value < u128(0x1431E0FAE, 0x6D7217CAA0000000)) {//1431E0FAE6D7217CAA0000000 == 100000000000000000000000000000
		return 28;
	}
	else if (value < u128(0xC9F2C9CD0, 0x4674EDEA40000000)) {//C9F2C9CD04674EDEA40000000 == 1000000000000000000000000000000
		return 29;
	}
	else if (value < u128(0x7E37BE2022, 0xC0914B2680000000)) {//7E37BE2022C0914B2680000000 == 10000000000000000000000000000000
		return 30;
	}
	else if (value < u128(0x4EE2D6D415B, 0x85ACEF8100000000)) {//4EE2D6D415B85ACEF8100000000 == 100000000000000000000000000000000
		return 31;
	}
	else if (value < u128(0x314DC6448D93, 0x38C15B0A00000000)) {//314DC6448D9338C15B0A00000000 == 1000000000000000000000000000000000
		return 32;
	}
	else if (value < u128(0x1ED09BEAD87C0, 0x378D8E6400000000)) {//1ED09BEAD87C0378D8E6400000000 == 10000000000000000000000000000000000
		return 33;
	}
	else if (value < u128(0x13426172C74D82, 0x2B878FE800000000)) {//13426172C74D822B878FE800000000 == 100000000000000000000000000000000000
		return 34;
	}
	else if (value < u128(0xC097CE7BC90715, 0xB34B9F1000000000)) {//C097CE7BC90715B34B9F1000000000 == 1000000000000000000000000000000000000
		return 35;
	}
	else if (value < u128(0x785EE10D5DA46D9, 0x00F436A000000000)) {//785EE10D5DA46D900F436A000000000 == 10000000000000000000000000000000000000
		return 36;
	}
	else if (value < u128(0x4B3B4CA85A86C47A, 0x098A224000000000)) {//4B3B4CA85A86C47A098A224000000000 == 100000000000000000000000000000000000000
		return 37;
	}
	else {
		return 38;
	}
}

u64 log2(u128 value) {//About 30 instructions to compute
	register u64 t, tt, ttt, tttt; // temporaries
	if (tttt = value.Top64()) {
		if (ttt = tttt >> 32) {//upper 32
			if (tt = ttt >> 16) {//upper 16
				if (t = tt >> 8)	return 120 + LogTable256[t];
				else				return 112 + LogTable256[tt];
			}
			else {//lower 16
				if (t = ttt >> 8)	return 104 + LogTable256[t];
				else				return 96 + LogTable256[ttt];
			}
		}
		else {//lower 32
			if (tt = static_cast<u64>(value >> 16UL)) {
				if (t = tt >> 8)	return 88 + LogTable256[t];
				else				return 80 + LogTable256[tt];
			}
			else {
				if (t = static_cast<u64>(value >> 8UL))
					return 72 + LogTable256[t];
				else				return 64 + LogTable256[static_cast<u64>(value)];
			}
		}
	}
	else {
		tttt = value.Bottom64();
		if (ttt = tttt >> 32) {
			if (tt = ttt >> 16) {
				if (t = tt >> 8)	return 56 + LogTable256[t];
				else				return 48 + LogTable256[tt];
			}
			else {
				if (t = ttt >> 8)	return 40 + LogTable256[t];
				else				return 32 + LogTable256[ttt];
			}
		}
		else {
			if (tt = static_cast<u64>(value >> 16UL)) {
				if (t = tt >> 8)	return 24 + LogTable256[t];
				else				return 16 + LogTable256[tt];
			}
			else {
				if (t = static_cast<u64>(value >> 8UL))
					return 8 + LogTable256[t];
				else				return LogTable256[static_cast<u64>(value)];
			}
		}
	}
}


std::string to_string(u128 value, u8 base) {
	std::stringstream ss;
	if (value == 0) {
		ss << '0';
	}
	else {
		while (value != 0) {
			ss << DIGITS[value % base];
			value /= base;
		}
	}

	std::string result = ss.str();
	std::reverse(result.begin(), result.end());
	return result;
}

void FromString(const char* string, u64 length, u8 base, u128* result EMPIRE_ERROR_CODE_PARAMETER) {
	u128 temp = 0;
	u128* prt = &temp;
	u128 multiplier = 1;
#if EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING
	u128 lastMultiplier = multiplier;
#endif
	for (u64 i = length - 1; /*Condition Later on to avoid 0UL -1 = big number*/; i--) {// Go backwards since the last place is ther ones place
		char c = string[i];
		int digit;
		if (c >= '0' && c <= '9') {
			digit = c - '0';
		}
		else if (c >= 'a' && c <= 'z') {
			digit = 10 + c - 'a';
		}
		else if (c >= 'A' && c <= 'Z') {
			digit = 10 + c - 'A';
		}
#if EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING
		else {
			EMPIRE_ERROR(EMPIRE_INVALID_CHARACTER, new InvalidCharacterErrorData(c, i, ""), EMPIRE_VOID_FUNCTION);
		}

		if (digit > base) {
			EMPIRE_ERROR(EMPIRE_INVALID_CHARACTER, new InvalidCharacterErrorData(c, i, "Numeric character larger then base"), EMPIRE_VOID_FUNCTION);
		}
#endif
		temp += multiplier * digit;
#if EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING
		lastMultiplier = multiplier;
#endif
		multiplier *= base;

#if EMPIRE_ENABLE_TEXT_PARSE_ERROR_CHECKING
		if (lastMultiplier > multiplier && i != 0) {
			EMPIRE_ERROR(EMPIRE_PARSE_ERROR_OVERFLOW, new ParseOverFlowData(std::string(string), "Input too long for target size of u128"), EMPIRE_VOID_FUNCTION);
		}
#endif
		if (i == 0) break;
	}
	result->_64[0] = temp._64[0];
	result->_64[1] = temp._64[1];
}

#endif// Use Foftware Implementation

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
#ifdef LITTLE_ENDIAN
			part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
#else
			part = value._8[i];// The higher parts start at lower indices
#endif
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
#ifdef LITTLE_ENDIAN
			part = value._8[sizeof(u128) - 1 - i];// The higher parts start at higher indices
#else
			part = value._8[i];// The higher parts start at lower indices
#endif
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