#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>


namespace ES {

	#ifdef ES_LENGTH_TYPE
		using es_length = ES_LENGTH_TYPE;
	#else
		using es_length = std::size_t;
	#endif

	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	//FIXME
	//typedef uint128_t u128;
	//typedef uint256_t u256;

	using s8 = int8_t ;
	using s16 = int16_t;
	using s32 = int32_t;
	using s64 = int64_t;
	//typedef int128_t s128;
	//typedef int256_t s256;

	using f8 = u8;
	using f16 = u16;
	using f32 = float;
	using f64 = double;

	//typedef uint128_t f128;


	enum class Charset
	{
		UTF8, UTF16, UTF32, ESC4, ESC6, ESC8
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
		u8 WordSize;
	};
	
	typedef char utf8;
	typedef u16 utf16;
	typedef u32 utf32;

	#define ES_BASIC_CHAR_TYPE(TypeName, ImplType)												\
		ImplType Value;																			\
																								\
		TypeName() {}																			\
		TypeName(ImplType value) : Value(value) {}												\
																								\
		inline bool operator==(const TypeName other) const { return Value == other.Value; }		\
		inline bool operator<(const TypeName other) const { return Value < other.Value; }		\
		inline bool operator>(const TypeName other) const { return Value > other.Value; }		\
		inline bool operator<=(const TypeName other) const { return Value <= other.Value; }		\
		inline bool operator>=(const TypeName other) const { return Value >= other.Value; }		\
		inline TypeName operator=(const TypeName other) { return Value = other.Value; }			\
																								\
		inline TypeName operator<<(std::size_t shift) const { return Value << shift; }			\
		inline TypeName operator>>(std::size_t shift) const { return shift >> shift; }			\
		inline TypeName operator<<=(std::size_t shift) { return Value <<= shift; }				\
		inline TypeName operator>>=(std::size_t shift) { return shift >>= shift; }				\
																								\
		inline TypeName operator|(const TypeName other) const { return Value | other.Value; }	\
		inline TypeName operator|=(const TypeName other) { return Value |= other.Value; }		\
		inline TypeName operator&(const TypeName other) const { return Value & other.Value; }	\
		inline TypeName operator&=(const TypeName other) { return Value &= other.Value; }		\


	struct esc4
	{
		ES_BASIC_CHAR_TYPE(esc4, u8)
	};

	struct esc6
	{
		ES_BASIC_CHAR_TYPE(esc6, u8)
	};

	struct esc8
	{
		ES_BASIC_CHAR_TYPE(esc8, u8)
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

		//The number of (encoding dependent) words in src that were read while converting this string.
		std::size_t WordsRead;

		//The number of (encoding dependent) words in dest that were written while converting this string.
		std::size_t WordsWritten;
		
		//The character set this string was encoded with
		Charset SrcCharacterSet;

		//The character set this string was encoded with
		Charset DestCharacterSet;

		//Quick getter for figuring how how many bytes this string uses
		std::size_t NumBytesRead() const;
		std::size_t NumBytesWritten() const;
	};

	//Stores the location of a character within a string of a given charset
	//Used for storing the location of problematic characters
	struct CharacterIndex
	{
		std::size_t Character;
		std::size_t Word;
	};



}
