#pragma once
/*
#ifndef __IEEE754_H__
#define __IEEE754_H__

#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <stdint.h>

#include "Integers.h"

template<unsigned M, unsigned E, int B >
class IEEE754;

namespace std {
	//The IEEE754 class with any template parameter is a floating point type.
	 
	template<unsigned M, unsigned E, int B >
	struct is_floating_point<IEEE754<M, E, B > > : public std::true_type { };

	//Specialization of std::numeric_limits for any bit format provided.
	template <unsigned M, unsigned E, int B >
	struct numeric_limits<IEEE754<M, E, B > > {
	public:
		static constexpr bool is_specialized = true;

		static IEEE754<M, E, B > min() { return from_components(+1, 0); }
		static IEEE754<M, E, B > max() { return from_components(-2, -1); }

		static constexpr int digits = M + 1;
		static constexpr int digits10 = M * M_LOG10E / M_LOG2E;
		static constexpr bool is_signed = true;
		static constexpr bool is_integer = false;
		static constexpr bool is_exact = false;
		static constexpr int radix = 2;

		static IEEE754<M, E, B > epsilon() { return from_components(B - M, 0); }
		static IEEE754<M, E, B > round_error() { return 0.5f; }

		static constexpr int min_exponent = -B + 2;
		static constexpr int max_exponent = B + 1;
		static constexpr int min_exponent10 = min_exponent * M_LOG10E / M_LOG2E;
		static constexpr int max_exponent10 = max_exponent * M_LOG10E / M_LOG2E;

		static constexpr bool has_infinity = true;
		static constexpr bool has_quiet_NaN = true;
		static constexpr bool has_signaling_NaN = false;
		static constexpr float_denorm_style has_denorm = denorm_present;
		static constexpr bool has_denorm_loss = false;

		static IEEE754<M, E, B > infinity() { return from_components(-1, 0); }
		static IEEE754<M, E, B > quiet_NaN() { return from_components(-1, +1); }
		static IEEE754<M, E, B > signaling_NaN() { return from_components(-1, +1); }
		static IEEE754<M, E, B > denorm_min() { return from_components(0, +1); }

		static constexpr bool is_iec559 = has_infinity && has_quiet_NaN && has_denorm == denorm_present;
		static constexpr bool is_bounded = true;
		static constexpr bool is_modulo = false;

		static constexpr bool traps = false;
		static constexpr bool tinyness_before = false;
		static constexpr float_round_style round_style = round_to_nearest;

	private:
		typedef typename IEEE754<M, E, B >::primitive primitive;

		inline static IEEE754<M, E, B > from_components(primitive exponent, primitive mantissa) {
			IEEE754<M, E, B > result;
			result.mantissa = mantissa;
			result.exponent = exponent;
			result.sign = 0;
			return result;
		}
	};

	// --------------------------- Classification --------------------------- //


	//Categorizes the given floating point value
	
	template <unsigned M, unsigned E, int B >
	int fpclassify(const IEEE754<M, E, B >& arg) {
		// Zero exponent can be either zero or a subnormal (denormal)
		if (arg.exponent == 0) {
			if (arg.mantissa == 0)
				return FP_ZERO;

			return FP_SUBNORMAL;
		}

		// Exponent of mask can be NaN or Infinite
		if (arg.exponent == IEEE754<M, E, B >::EXPONENT_MASK) {
			if (arg.mantissa == 0)
				return FP_INFINITE;

			return FP_NAN;
		}

		// Any other value is considered a normal value
		return FP_NORMAL;
	}

	
	//Checks if the given number has finite value
	
	template <unsigned M, unsigned E, int B >
	bool isfinite(const IEEE754<M, E, B >& arg) {
		return (arg.exponent != IEEE754<M, E, B >::EXPONENT_MASK);
	}

	//Checks if the given number is infinite
	template <unsigned M, unsigned E, int B >
	bool isinf(const IEEE754<M, E, B >& arg) {
		return (arg.exponent == IEEE754<M, E, B >::EXPONENT_MASK)
			&& arg.mantissa == 0;
	}

	template <unsigned M, unsigned E, int B >
	bool isnan(const IEEE754<M, E, B >& arg) {
		return (arg.exponent == IEEE754<M, E, B >::EXPONENT_MASK)
			&& arg.mantissa != 0;
	}

	template <unsigned M, unsigned E, int B >
	bool isnormal(const IEEE754<M, E, B >& arg) {
		return (arg.exponent != 0)
			&& (arg.exponent != IEEE754<M, E, B >::EXPONENT_MASK);
	}

	//Checks if two floating-point values are unordered
	template <unsigned M, unsigned E, int B >
	bool isunordered(const IEEE754<M, E, B >& arg1, const IEEE754<M, E, B >& arg2) {
		return isnan(arg1)
			|| isnan(arg2);
	}

	// ------------------------- Sign manipulation -------------------------- //

	//Checks if the given number is negative
	template <unsigned M, unsigned E, int B >
	bool signbit(const IEEE754<M, E, B >& arg) {
		return arg.sign != 0;
	}

	//Computes the absolute value of a floating point value
	template <unsigned M, unsigned E, int B >
	IEEE754<M, E, B > abs(const IEEE754<M, E, B >& arg) {
		IEEE754<M, E, B > result = arg;
		result.sign = 0;
		return result;
	}

	//Composes a floating point value with the magnitude of x and the sign of y
	template <unsigned M, unsigned E, int B >
	IEEE754<M, E, B > copysign(const IEEE754<M, E, B >& x, const IEEE754<M, E, B >& y) {
		IEEE754<M, E, B > result = x;
		result.sign = y.sign;
		return result;
	}

	// ----------------------------- Components ----------------------------- //

	//Returns the resulting floating point value from multiplying
	//the significand by 2 raised to the power of the exponent.
	template <unsigned M, unsigned E, int B >
	IEEE754<M, E, B > ldexp(const IEEE754<M, E, B >& x, int exp) {
		// XXX: Conversion of double to IEEE754 instance
		return x * std::pow(2.0, exp);
	}

	//Breaks the floating point number x into its binary significand
	//and an integral exponent for 2
	template <unsigned M, unsigned E, int B >
	IEEE754<M, E, B > frexp(const IEEE754<M, E, B >& x, int* exp) {
		IEEE754<M, E, B > result = x;

		switch (std::fpclassify(x)) {
		case FP_NAN:
		case FP_ZERO:
		case FP_INFINITE:
			*exp = 0;
			break;
		case FP_SUBNORMAL: {
			int log2 = std::log2(x.mantissa);

			*exp = log2 - B - 2;
			result.exponent = B - 1;
			result.mantissa = x.mantissa << (M - log2);
		} break;
		case FP_NORMAL:
			*exp = x.exponent - B + 1;
			result.exponent = B - 1;
			break;
		}

		return result;
	}
}

template<unsigned bits >
struct smallest_unsigned {
	typedef typename smallest_unsigned<bits + 1>::type type;
};

template<>
struct smallest_unsigned<8> {
	typedef uint8_t type;
};

template<>
struct smallest_unsigned<16> {
	typedef uint16_t type;
};

template<>
struct smallest_unsigned<32> {
	typedef uint32_t type;
};

template<>
struct smallest_unsigned<64> {
	typedef uint64_t type;
};

template<>
struct smallest_unsigned<128> {
	typedef u128 type;
};

template<>
struct smallest_unsigned<256> {
	typedef u256 type;
};

template<unsigned M, unsigned E, int B = (1 << (E - 1)) - 1 >
class IEEE754 {
	// XXX: This actually needs to be private, however I do not know how to make
	// The functions at std:: overrides able to access it without anything else does
	// (friending everything is very tedious and you cannot friend a namespace)
public:
	enum {
		MANTISSA_MASK = (1UL << M) - 1,
		EXPONENT_MASK = (1UL << E) - 1,

		MIN_EXPONENT = -B + 2,
		MAX_EXPONENT = B + 1,

		BITS = 1 + E + M,
	};

	typedef typename smallest_unsigned<BITS >::type primitive;
	typedef typename std::make_signed<primitive >::type signed_primitive;

	primitive mantissa : M;
	primitive exponent : E;
	primitive sign : 1;

private:
	//Build a float from components
	inline static IEEE754 from_components(primitive sign, primitive exponent, primitive mantissa) {
		IEEE754 result;
		result.sign = sign;
		result.exponent = exponent;
		result.mantissa = mantissa;
		return result;
	}

	//Renormalizes a signed fixed point into a float
	template<typename T >
	inline static IEEE754 renormalize(T unnormalized, int radix_point) {
		IEEE754 result;
		result.from_signed(unnormalized, radix_point);
		return result;
	}

	//Renormalizes an unsigned fixed point into a float
	template<typename T >
	inline static IEEE754 renormalize(T unnormalized, int radix_point, int sign) {
		IEEE754 result;
		result.sign = sign;
		result.from_unsigned(unnormalized, radix_point);
		return result;
	}

	//Returns nan with optional sign and mantissa
	inline static IEEE754 nan(primitive sign = 0, primitive mantissa = 1) {
		return from_components(sign, EXPONENT_MASK, mantissa);
	}

	//Returns Infinity
	inline static IEEE754 inf(primitive sign = 0) {
		return from_components(sign, EXPONENT_MASK, 0);
	}

	//Shift that allows negative values
	template <
		typename T,
		typename RT = typename std::conditional<(sizeof(T) > sizeof(primitive)), T, primitive >::type
		>
		static RT shift(T value, int shift_amount) {
		if (shift_amount < 0)
			return value >> -shift_amount;
		if (shift_amount > 0)
			return value << shift_amount;

		return value;
	}

	//Computes the real value of the mantissa.
	//This adds the implicit 1.xxxx to the mantissa when needed
	primitive real_mantissa() const {
		return exponent ? mantissa | (1 << M) : (mantissa << 1);
	}

	//Fills up exponent and mantissa from an unsigned value.
	//Sign is left unchanged.
	template <typename T >
	void from_unsigned(T unsigned_value, int radix_point = 0) {
		if (unsigned_value > shift<T >((1 << (M + 1)) - 1, E - radix_point)) {
			exponent = EXPONENT_MASK;
			mantissa = 0;
		}
		else {
			int log2 = std::log2(unsigned_value);

			if (radix_point + log2 + 1 < MIN_EXPONENT) {
				exponent = 0;
				mantissa = shift(unsigned_value, M - (MIN_EXPONENT - radix_point - 1));
			}
			else {
				exponent = unsigned_value ? log2 + radix_point + B : 0;
				mantissa = shift(unsigned_value, M - log2);
			}
		}
	}

	//Fills up sign, exponent and mantissa from an unsigned value.
	template <typename T >
	void from_signed(T signed_value, int radix_point = 0) {
		typedef typename std::make_signed<T >::type signed_T;
		typedef typename std::make_unsigned<T >::type unsigned_T;

		signed_T forced_signed = signed_value;

		sign = (forced_signed < 0);
		from_unsigned<unsigned_T >(std::abs(forced_signed), radix_point);
	}

	//Retrieve the value of this float as an unsigned value
	template <typename T >
	T to_unsigned(int radix_point = 0) const {
		return shift<T >(real_mantissa(), exponent - radix_point - B - M);
	}

	//Retrieve the value of this float as a signed value
	template <typename T >
	T to_signed(int radix_point = 0) const {
		return (to_unsigned<T >(radix_point) ^ -sign) + sign;
	}

public:
	// -------------------------- Constructors -------------------------- //

	//Default constructor, undefined value.
	IEEE754() = default;

	//Default copy constructor
	IEEE754(const IEEE754&) = default;

	//Conversion from another IEEE floating point object
	template <unsigned OM, unsigned OE, int OB >
	IEEE754(const IEEE754<OM, OE, OB >& other_ieee754) {
		sign = other_ieee754.sign;
		exponent = other_ieee754.exponent ? other_ieee754.exponent - OB + B : 0;
		mantissa = shift(other_ieee754.mantissa, M - OM);
	}

	//Conversion from a floating point value
	template <
		typename T,
		typename = typename std::enable_if<std::is_floating_point<T >::value, T >::type
	>
		IEEE754(T floating_point) {
		sign = std::signbit(floating_point);

		if (std::isnormal(floating_point)) {
			int exp = 0;
			primitive man = std::ldexp(std::frexp(floating_point, &exp), M + 1);

			if (exp > MAX_EXPONENT) {
				exponent = EXPONENT_MASK;
				mantissa = 0;
			}
			else if (exp < MIN_EXPONENT) {
				exponent = 0;
				mantissa = shift(man, exp - MIN_EXPONENT + 1);
			}
			else {
				exponent = exp + B - 1;
				mantissa = man;
			}
		}
		else {
			exponent = floating_point == T() ? 0 : EXPONENT_MASK;
			mantissa = std::isnan(floating_point);
		}
	}

	//Conversion from a signed integral type
	template <
		typename T,
		typename = typename std::enable_if<!std::is_floating_point<T >::value, T >::type,
		typename = typename std::enable_if< std::is_signed<T >::value, T >::type
	>
		IEEE754(T signed_integral) {
		from_signed(signed_integral);
	}

	//Conversion from an unsigned integral type
	template <
		typename T,
		typename = typename std::enable_if<!std::is_floating_point<T >::value, T >::type,
		typename = typename std::enable_if<!std::is_signed<T >::value, T >::type,
		typename = typename std::enable_if< std::is_unsigned<T >::value, T >::type
	>
		IEEE754(T unsigned_integral) {
		sign = 0;
		from_unsigned(unsigned_integral);
	}

	// ------------------------- Cast operators ------------------------- //

	//Convert to another floating point value
	template <
		typename T,
		typename = typename std::enable_if<std::is_floating_point<T >::value, T >::type
	>
		operator T() const {
		T result;
		if (exponent != EXPONENT_MASK) {
			result = std::ldexp(real_mantissa() / T(1 << M), exponent - B);
		}
		else {
			if (mantissa)
				result = std::numeric_limits<T >::quiet_NaN();
			else
				result = std::numeric_limits<T >::infinity();
		}

		return std::copysign(result, -sign);
	}

	//Convert to a signed integer
	template <
		typename T,
		typename = typename std::enable_if<!std::is_floating_point<T >::value, T >::type,
		typename = typename std::enable_if< std::is_signed<T >::value, T >::type
	>
		operator T() const {
		return to_signed<T >();
	}

	//Convert to an unsigned integer
	template <
		typename T,
		typename = typename std::enable_if<!std::is_floating_point<T >::value, T >::type,
		typename = typename std::enable_if<!std::is_signed<T >::value, T >::type,
		typename = typename std::enable_if< std::is_unsigned<T >::value, T >::type
	>
		operator T() const {
		return to_unsigned<T >();
	}

	// --------------------------- Arithmetic --------------------------- //

	// Unary

	friend IEEE754 operator + (const IEEE754& value) {
		return value;
	}

	friend IEEE754 operator - (const IEEE754& value) {
		return from_components(
			value.sign ^ 1,
			value.exponent, value.mantissa);
	}

	// Binary

	friend IEEE754 operator + (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return nan();
		if (std::isinf(lhs) || std::isinf(rhs)) {
			if (std::isinf(lhs) && !std::isinf(rhs))
				return lhs;
			if (std::isinf(rhs) && !std::isinf(lhs))
				return rhs;
			if (rhs == lhs)
				return rhs;

			return nan();
		}

		int exp = std::min(lhs.exponent - B, rhs.exponent - B) - M;
		return renormalize(
			lhs.to_signed<signed_primitive >(exp) +
			rhs.to_signed<signed_primitive >(exp), exp);
	}

	friend IEEE754 operator - (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return nan();
		if (std::isinf(lhs) || std::isinf(rhs)) {
			if (std::isinf(lhs) && !std::isinf(rhs))
				return lhs;
			if (std::isinf(rhs) && !std::isinf(lhs))
				return -rhs;
			if (rhs != lhs)
				return lhs;

			return nan();
		}

		int exp = std::min(lhs.exponent - B, rhs.exponent - B) - M;
		return renormalize(
			lhs.to_signed<signed_primitive >(exp) -
			rhs.to_signed<signed_primitive >(exp), exp);
	}

	friend IEEE754 operator * (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return nan();

		int exponent = (lhs.exponent - B) + (rhs.exponent - B);
		return renormalize(
			(lhs.real_mantissa() *
				rhs.real_mantissa()) >> M, exponent - M, lhs.sign ^ rhs.sign);
	}

	friend IEEE754 operator / (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return nan();
		if (rhs == 0)
			return inf(lhs.sign ^ rhs.sign);

		int exponent = (lhs.exponent - B) - (rhs.exponent - B);
		return renormalize(
			(lhs.real_mantissa() << M) /
			rhs.real_mantissa(), exponent - M, lhs.sign ^ rhs.sign);
	}

	// Placement

	IEEE754& operator += (const IEEE754& value) {
		return *this = *this + value;
	}

	IEEE754& operator -= (const IEEE754& value) {
		return *this = *this - value;
	}

	IEEE754& operator *= (const IEEE754& value) {
		return *this = *this * value;
	}

	IEEE754& operator /= (const IEEE754& value) {
		return *this = *this / value;
	}

	// --------------------------- Comparison --------------------------- //

	// Equality

	friend bool operator == (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return false;

		if (lhs.exponent == 0 && lhs.mantissa == 0
			&& rhs.exponent == 0 && rhs.mantissa == 0)
			return true;

		return (primitive&)lhs == (primitive&)rhs;
	}

	friend bool operator != (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return true;

		if (lhs.exponent == 0 && lhs.mantissa == 0
			&& rhs.exponent == 0 && rhs.mantissa == 0)
			return false;

		return (primitive&)lhs != (primitive&)rhs;
	}

	// Relative comparison

	friend bool operator < (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return false;

		if (lhs.exponent - B < rhs.exponent - B)
			return !rhs.sign;

		if (lhs.mantissa < rhs.mantissa)
			return !rhs.sign;

		return lhs.sign && !rhs.sign;
	}

	friend bool operator > (const IEEE754& lhs, const IEEE754& rhs) {
		if (std::isunordered(lhs, rhs))
			return false;

		if (lhs.exponent - B > rhs.exponent - B)
			return !lhs.sign;

		if (lhs.mantissa > rhs.mantissa)
			return !lhs.sign;

		return !lhs.sign && rhs.sign;
	}

	friend bool operator <= (const IEEE754& lhs, const IEEE754& rhs) {
		return (lhs < rhs)
			|| (rhs == lhs);
	}

	friend bool operator >= (const IEEE754& lhs, const IEEE754& rhs) {
		return (lhs > rhs)
			|| (rhs == lhs);
	}
};

template<unsigned M, unsigned E, int B>
std::ostream& operator<<(std::ostream& os, const IEEE754<M, E, B>& value)
{
	os << "Not implemented" << std::endl;

	return os;
}

#endif
*/