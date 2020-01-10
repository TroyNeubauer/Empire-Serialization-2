#pragma once

#include <cstddef>
#include <cstdint>

#include <type_traits>
#include <vector>
#include <string>


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
		ESC4, ESC6, ESC8, UTF8, UTF16, UTF32, 
		//Any charset exists here only for obtaining its name and CharsetCode with GetCharsetInfo
		//Any Charset is a wrapper for another charset so an "instance" of Any Charset will never really exist
		ANY_CHARSET
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
		u8 CharsetCode;
	};
	
	#define ES_BASIC_CHAR_TYPE(TypeName, ImplType)												\
		ImplType Value;																			\
																								\
		TypeName() {}																			\
		TypeName(ImplType value) : Value(value) {}												\
		using WordType = ImplType;																\
																								\
		explicit operator u8() const { return static_cast<u8>(Value); }							\
		explicit operator u16() const { return static_cast<u16>(Value); }						\
		explicit operator u32() const { return static_cast<u32>(Value); }						\
		explicit operator u64() const { return static_cast<u64>(Value); }						\
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


	struct utf8
	{
		ES_BASIC_CHAR_TYPE(utf8, u8)
	};

	struct utf16
	{
		ES_BASIC_CHAR_TYPE(utf16, char16_t)
	};

	struct utf32
	{
		ES_BASIC_CHAR_TYPE(utf32, char32_t)
	};

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

	//Stores the location of a character within a string of a given charset
	//Used for storing the location of problematic characters
	struct CharacterIndex
	{
		std::size_t Character;
		std::size_t Word;
	};

	using TypeID = es_length;

  template<typename, typename>
    struct is_same
    : public false_type { };

  template<typename _Tp>
    struct is_same<_Tp, _Tp>
    : public true_type { };

	namespace Info {
		template<typename T>
		struct IsCharacterType : public std::false_type {  };

		template<> struct IsCharacterType<utf8> : public std::true_type {  };
		template<> struct IsCharacterType<utf16> : public std::true_type {  };
		template<> struct IsCharacterType<utf32> : public std::true_type {  };
		template<> struct IsCharacterType<esc4> : public std::true_type {  };
		template<> struct IsCharacterType<esc6> : public std::true_type {  };
		template<> struct IsCharacterType<esc8> : public std::true_type {  };

	}

	namespace DefaultTypes {
		static constexpr TypeID
			INVALID = 0,
			S8 = 1,
			U8 = 2,
			S16 = 3,
			U16 = 4,
			S32 = 5,
			U32 = 6,
			S64 = 7,
			U64 = 8,
			S128 = 9,
			U128 = 10,
			S256 = 11,
			U256 = 12,
			BIG_INTEGER = 13,

			F8 = 14,
			F16 = 15,
			F32 = 16,
			F64 = 17,
			F128 = 18,
			BIG_FLOAT = 19,

			UTF8_STRING = 20,
			UTF16_STRING = 21,
			UTF32_STRING = 22,
			ESC4_STRING = 23,
			ESC6_STRING = 24,
			ESC8_PLUS_STRING = 25;
	}

	enum class SequenceType
	{
		LIST, MAP
	};

	struct MemberVariable
	{
		std::string Name;
		TypeID Type;
		u32 Offset;
	};


	struct Type
	{
		TypeID ID;

		union 
		{
			//No additional data for primitive types because the logic to handle them is hardcoded based on their TypeID

			struct
			{
				std::string Name;
				std::vector<MemberVariable> MemberVariables;
			} Class;
			
			struct
			{
				//A sequence can have a dynamic length (where the length is variable and written before the data for each sequence)
				//One registered sequence type can be used to encode a sequence of those types for any length
				//The static length sequence fixes the length of itself at type creation time so that it can encode sequences of only
				//that length (thus allowing the serializer to omit the length of the sequence because the sequence type rtains the length)
				//This is like std::vector (dynamic-length sequence) vs std::array (static-length sequence)
				//If IsStaticLength is false (indicating a dynamic-length sequence) then the valud of StaticLength is unspecified
				es_length StaticLength;
				SequenceType Type;
				bool IsStaticLength;

				//Each kind of sequence needs to store different data
				union 
				{
					struct
					{
						TypeID ListType;
					} ListSeqnence;

					struct
					{
						TypeID KeyType;
						TypeID ValueType;
					} MapSeqnence;
				};
				
			} Sequence;

		};
	};

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
}
