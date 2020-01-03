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


	enum class Charset
	{
		UTF8, UTF16, UTF32, ESC4, ESC6, ESC8, INVALID
	};

	struct CharsetInfo
	{
		//Word size is being equal to this indicates that this character set uses variable length encoding
		static constexpr double VARIABLE_CHARACTERS_PER_WORD = 0.0;

		const char* Name;

		//The number of characters stored in a word.
		//Double because ESC6 has a non integer number of characters per word
		double CharactersPerWord = 1.0;

		//The number of bytes a single word occupies
		uint8_t WordSize;
	};

	const CharsetInfo& GetCharsetInfo(Charset charset);
	
	typedef char utf8;
	typedef uint16_t utf16;
	typedef uint32_t utf32;

	#define ES_BASIC_CHAR_TYPE(TypeName, ImplType)										\
		ImplType Value;																	\
																						\
		TypeName() {}																	\
		TypeName(ImplType value) : Value(value) {}										\
																						\
		bool operator==(const TypeName other) const { return Value == other.Value; }	\
		bool operator<(const TypeName other) const { return Value < other.Value; }		\
		bool operator>(const TypeName other) const { return Value > other.Value; }		\
		bool operator<=(const TypeName other) const { return Value <= other.Value; }	\
		bool operator>=(const TypeName other) const { return Value >= other.Value; }	\
		TypeName operator=(const TypeName other) { return Value = other.Value; }		\
																						\
		TypeName operator<<(std::size_t shift) const { return Value << shift; }			\
		TypeName operator>>(std::size_t shift) const { return shift >> shift; }			\
		TypeName operator<<=(std::size_t shift) { return Value <<= shift; }				\
		TypeName operator>>=(std::size_t shift) { return shift >>= shift; }				\
																						\
		TypeName operator|(const TypeName other) const { return Value | other.Value; }	\
		TypeName operator|=(const TypeName other) { return Value |= other.Value; }		\
		TypeName operator&(const TypeName other) const { return Value & other.Value; }	\
		TypeName operator&=(const TypeName other) { return Value &= other.Value; }		\

	struct esc4 {
		ES_BASIC_CHAR_TYPE(esc4, uint8_t)
	};

	struct esc6 {
		ES_BASIC_CHAR_TYPE(esc6, uint8_t)
	};

	struct esc8 {
		ES_BASIC_CHAR_TYPE(esc8, uint8_t)
	};

	template<typename T>
	struct GetCharsetCode
	{
	};

	template<> struct GetCharsetCode<utf8>	{ static constexpr Charset Code =  Charset::UTF8; };
	template<> struct GetCharsetCode<utf16>	{ static constexpr Charset Code = Charset::UTF16; };
	template<> struct GetCharsetCode<utf32>	{ static constexpr Charset Code = Charset::UTF32; };
	template<> struct GetCharsetCode<esc4>	{ static constexpr Charset Code =  Charset::ESC4; };
	template<> struct GetCharsetCode<esc6>	{ static constexpr Charset Code =  Charset::ESC6; };
	template<> struct GetCharsetCode<esc8>	{ static constexpr Charset Code =  Charset::ESC8; };

	//The data given back to the caller when a string is encoded into a particular character set
	struct StringCodingData
	{
		//A character is the a character code. For variable length encoding schemes one multi byte sequence is a character
		//For a utf8 string of two emojies, Characters will be 2  
		std::size_t Characters;

		//The number of (encoding dependent) words used by this string.
		//The storage type for utf8 is uint8_t, uint16_t for utf16 etc.
		//For a utf8 string of two emojies, Words will be 2 
		std::size_t Words;
		
		//The character set this string was encoded with
		Charset CharacterSet;

		//Quick getter for figuring how how many bytes this string uses
		inline std::size_t GetNumBytes() const { return GetCharsetInfo(CharacterSet).WordSize * Words; }
	};

	//Stores the location of a character within a string of a given charset
	//Used for storing the location of problematic characters
	struct CharacterIndex
	{
		std::size_t Character;
		std::size_t Word;
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
		NONE = 0,
		//Indicates that no erroe has taken place
		
		UNSUPPORTED_CHARACTER,
		//Thrown when a character set cannot represent a character
		
		INVALID_CHARACTER,
		//Thrown when an invalid character is read suring string decoding

		NOT_IMPLEMENTED,
		//Thrown when a function cannot complete its job because the code to execute perform some task
		//in not present in this binary.
		//This can be because the user configured Empire Serialization to omit rarely used features 
		//(like FIXME: add example) during compilation to reduce the size of the binary, or this can also be thrown
		//when somthing is a work in progress on the devolepment branch

		BUFFER_OVERFLOW,
		//Thrown when a write is attempted on a full buffer
		
		BUFFER_UNDERFLOW
		//Thrown when a buffer is attempted to be read past its end and no data is avilable
	};

	struct Error
	{
		ErrorCode Type;
		SizedFormatter<128> AdditionalMessage;

		union 
		{
			//Thrown during character set conversions when a character cannot be represented in the destination charset
			struct
			{
				uint32_t Char;
				Charset CharacterSet;
				CharacterIndex Position;//Information about where the unsupported character is. In bytes and characters
			} UnsupportedCharacter;

			//Thrown when a character is not recognised during string decoding
			struct
			{
				uint32_t Char;
				Charset CharacterSet;
				CharacterIndex Position;
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

			struct
			{
				size_t BufferSize;
				size_t RequiredSize;
			} BufferUnderflow;

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