#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "EmpireSerialization/Format.h"

namespace ES {

	#ifdef ES_LENGTH_TYPE
		using es_length = ES_LENGTH_TYPE;
	#else
		using es_length = size_t;
	#endif

	typedef uint8_t u8;
	typedef uint16_t u16;
	typedef uint32_t u32;
	typedef uint64_t u64;

	//FIXME
	//typedef uint128_t u128;
	//typedef uint256_t u256;

	typedef int8_t s8;
	typedef int16_t s16;
	typedef int32_t s32;
	typedef int64_t s64;
	//typedef int128_t s128;
	//typedef int256_t s256;

	typedef uint8_t f8;
	typedef uint16_t f16;
	typedef float f32;
	typedef double f64;

	//typedef uint128_t f128;

	typedef char utf8;
	typedef uint16_t utf16;
	typedef uint32_t utf32;

	struct esc4 {
		esc4() {}
		esc4(uint8_t value) : Value(value) {}
		
		bool operator==(const esc4& other) const { return Value == other.Value; }
		bool operator<(const esc4& other) const { return Value < other.Value; }
		bool operator>(const esc4& other) const { return Value > other.Value; }
		bool operator<=(const esc4& other) const { return Value <= other.Value; }
		bool operator>=(const esc4& other) const { return Value >= other.Value; }
		esc4 operator=(const esc4& other) { return Value = other.Value; }

		uint8_t Value;
	};

	struct esc6 {
		esc6() {}
		esc6(uint8_t value) : Value(value) {}

		bool operator==(const esc6& other) const { return Value == other.Value; }
		bool operator<(const esc6& other) const { return Value < other.Value; }
		bool operator>(const esc6& other) const { return Value > other.Value; }
		bool operator<=(const esc6& other) const { return Value <= other.Value; }
		bool operator>=(const esc6& other) const { return Value >= other.Value; }
		esc6 operator=(const esc6& other) { return Value = other.Value; }


		uint8_t Value;
	};

	struct esc8 {
		esc8() {}
		esc8(uint8_t value) : Value(value) {}

		bool operator==(const esc8& other) const { return Value == other.Value; }
		bool operator<(const esc8& other) const { return Value < other.Value; }
		bool operator>(const esc8& other) const { return Value > other.Value; }
		bool operator<=(const esc8& other) const { return Value <= other.Value; }
		bool operator>=(const esc8& other) const { return Value >= other.Value; }
		esc8 operator=(const esc8& other) { return Value = other.Value; }


		uint8_t Value;
	};

	struct StringCodingData
	{
		std::size_t Characters;
		std::size_t Bytes;
	};


	const int ES_ASSERT_LEVEL_NONE = 0;

	const int ES_ASSERT_LEVEL_MIN = 1;
	const int ES_ASSERT_LEVEL_MAX = 2;

	#ifndef ES_ASSERT_LEVEL
		#define ES_ASSERT_LEVEL ES_ASSERT_LEVEL_MAX
	#endif

	//Not enum class to allow for if (ErrorCode) without the compiler complaining
	enum ErrorCode
	{
		NONE = 0, INVALID_CHARACTER, NOT_IMPLEMENTED, BUFFER_OVERFLOW
	};

	enum class Charset
	{
		UTF8, UTF16, UTF32, ESC4, ESC6, ESC8
	};

	template<typename T>
	struct GetCharsetCode
	{
		static const Charset Code;
	};

	// template<> struct GetCharsetCode<utf8>	{ static const Charset Code =  Charset::UTF8; };
	// template<> struct GetCharsetCode<utf16>	{ static const Charset Code = Charset::UTF16; };
	// template<> struct GetCharsetCode<utf32>	{ static const Charset Code = Charset::UTF32; };
	// template<> struct GetCharsetCode<esc4>	{ static const Charset Code =  Charset::ESC4; };
	// template<> struct GetCharsetCode<esc6>	{ static const Charset Code =  Charset::ESC6; };
	// template<> struct GetCharsetCode<esc8>	{ static const Charset Code =  Charset::ESC8; };


	struct Error
	{
		ErrorCode Type;

		union 
		{
			struct
			{
				uint32_t Char;
				Charset CharacterSet;
				StringCodingData Position;//The index of the bad character
			} InvalidCharacter;
			
			struct
			{
				const char* Feature;
			} NotImplemented;

			struct
			{
				size_t BufferSize;
				size_t RequiredSize;
			} BufferOverflow;

		};
	};

	const Error& GetError();
	void ClearError();


	using AllocErrorHandler = std::add_pointer<void()>::type;

	void SetAllocErrorHandler(AllocErrorHandler handler);

	namespace ToString {
		void PrintError(Formatter& formatter, const Error& error);
		const char* GetErrorCodeString(ErrorCode codr);
		const char* GetCharsetString(Charset charset);

	}
	
}