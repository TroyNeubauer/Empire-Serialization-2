//===================================================================================
// MIT Liscense
// Copyright (c) 2020, Troy Neubauer
//
//	File: Charsets.h
//	Contains definitions for encoding and decoding cutsom charsets. 
//

#pragma once

#include <cstdint>
#include <array>

#include "ForwardTypes.h"
#include "Error.h"

namespace ES {

	const CharsetInfo& GetCharsetInfo(Charset charset);

	template<typename T, std::size_t Len>
	constexpr std::size_t EncodingSize(const std::array<T, Len>& decoding)
	{
		std::size_t result = 0;
		for (std::size_t i = 0; i < decoding.size(); i++)
		{
			if (decoding[i] > result)
			{
				result = decoding[i];
			}
		}
		return result + 1;
	}

	//Use const expressions to generate a reversed version of an array
	template<typename T, std::size_t DecodeLen, std::size_t EncodeLen>
	constexpr const std::array<T, EncodeLen> ConstructEncoding(const std::array<T, DecodeLen>& decoding)
	{
		std::array<T, EncodeLen> encoding {};
		for (int i = 0; i < EncodeLen; i++)
			encoding[i] = static_cast<T>(-1);
		for (std::size_t i = 0; i < DecodeLen; i++)
		{
			encoding[decoding[i]] = static_cast<T>(i);
		}
		
		return encoding;
	}

	//XXX_DECODE allows users to obtain the unicode value for the character value in another encoding scheme
	//XXX_ENCODE is the mathmatical inverse function XXX_DECODE
	constexpr const std::array<char, 16> ESC4_DECODE = { 0x00, 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'u', 'c', 'm', 'w', };
	static constexpr const std::size_t ESC4_ENCODE_SIZE = EncodingSize(ESC4_DECODE);
	constexpr const std::array<char, ESC4_ENCODE_SIZE> ESC4_ENCODE = ConstructEncoding<char, ESC4_DECODE.size(), ESC4_ENCODE_SIZE>(ESC4_DECODE);

	constexpr const std::array<char, 64> ESC6_DECODE =
	{ 
	    0x00, ' ', '\n', '.', ',', '\'', '!', '?', ';', ':', '-', '_', 'a', 'b', 'c', 'd',
		'e',  'f', 'g', 'h',  'i',  'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u',  'v', 'w', 'x',  'y',  'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K',  'L', 'M', 'N',  'O',  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	};
	static constexpr const std::size_t ESC6_ENCODE_SIZE = EncodingSize(ESC6_DECODE);
	constexpr const std::array<char, ESC6_ENCODE_SIZE> ESC6_ENCODE = ConstructEncoding<char, ESC6_DECODE.size(), ESC6_ENCODE_SIZE>(ESC6_DECODE);




	//Represents an abstract reader
	//The Read method can be called to obtain decoded unicode codepoints from the buffer
	template<typename T> struct CharsetDecoder;


	//This class is not functional. Its only purpose it to cleanly illustrate the API because the actual 
	//CharsetDecoder<utf8>, CharsetDecoder<utf16> implementations use a lot of macros and as a result look nasty.
	//This classes methods are identical to the actual ones found in CharsetDecoder<utf8>, CharsetDecoder<esc4> etc.

	template<typename T>
	struct CharsetDecoder_API
	{
	private:
		//Initalizes this reader with a bolb words for a particular charset
		//that will be decoded into unicode codepoints
		CharsetDecoder_API(int* /*Actual type is T::WordType*/ buf, std::size_t length);

		//Reads a single character from this stream, and store its unicode value into codepoint.
		//If an error happens while obtaining the character, the errorcode is returned to
		//the user and GetError() is set appropiatly
		ErrorCode Read(u32& codepoint);

		//Returns true if there is more characters that can be read
		bool HasChars() const;

		std::size_t BytesRead() const;
		std::size_t WordsRead() const;
		std::size_t CharactersRead() const;

	};

#define CHARSET_DECODER_PUBLIC_METHODS_DEF(ClassName)										\
		CharsetDecoder(const ClassName* buf, std::size_t length)							\
			: m_Buf(reinterpret_cast<const ClassName::WordType*>(buf)), m_BufEnd(m_Buf + length), m_BufInital(m_Buf) {}						\
																							\
		ErrorCode Read(u32& unicode);														\
		inline std::size_t WordsRead() const { return m_Buf - m_BufInital; }				\
		inline std::size_t BytesRead() const { return WordsRead() * sizeof(ClassName); }	\
		inline std::size_t CharactersRead() const { return m_CharacterIndex; }				\
		inline bool HasChars() const { return m_Buf != m_BufEnd; }							\




#define CHARSET_DECODER_PRIVATE_METHODS_DEF(ClassName)	\
		ErrorCode ReadWord(ClassName::WordType& word);	\
		ErrorCode InvalidCharacterError();				\



#define CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(ClassName)		\
		const ClassName::WordType* m_Buf;						\
		const ClassName::WordType* m_BufEnd;					\
		const ClassName::WordType* const m_BufInital;			\
		std::size_t m_CharacterIndex = 0;						\

	template<>
	struct CharsetDecoder<utf8>
	{

	public:  CHARSET_DECODER_PUBLIC_METHODS_DEF(utf8)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(utf8)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf8)

	};

	template<>
	struct CharsetDecoder<utf16>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(utf16)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(utf16)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf16)
	};


	template<>
	struct CharsetDecoder<utf32>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(utf32)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(utf32)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf32)
	};



	template<>
	struct CharsetDecoder<esc4>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc4)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(esc4)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc4)

		bool m_Stale = true;
		  esc4::WordType m_CurrentWord;
	};

	template<>
	struct CharsetDecoder<esc6>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc6)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(esc6)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc6)

		union
	{
		esc6::WordType m_Words[4];
		u32 m_PackedWords;
	};



	};

	template<>
	struct CharsetDecoder<esc8>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc8)

	private: CHARSET_DECODER_PRIVATE_METHODS_DEF(esc8)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc8)


	};


	
	template<typename T> struct CharsetEncoder;
	
	template<> struct CharsetEncoder<utf8>;
	template<> struct CharsetEncoder<utf16>;
	template<> struct CharsetEncoder<utf32>;
	template<> struct CharsetEncoder<esc4>;
	template<> struct CharsetEncoder<esc6>;
	template<> struct CharsetEncoder<esc8>;

	template<typename T>
	struct CharsetEncoder_API
	{
	private:
		//Initalizes this reader with a binary bolb of data that will be used to write words of type T
		CharsetEncoder_API(int* /*Actual type is T::WordType*/ buf, std::size_t bytes);

		//Reads a single character from this stream into character. If an error happens while obtaining the character,
		//the errorcode is returned to the user and GetError() is set appropiatly
		ErrorCode Write(u32 unicode);

		std::size_t BytesWritten() const;
		std::size_t WordsWritten() const;
		std::size_t Written() const;

	};

	#define CHARSET_ENCODER_PUBLIC_METHODS_DEF(ClassName)															\
		CharsetEncoder(ClassName* buf, std::size_t length)															\
			: m_Buf(reinterpret_cast<ClassName::WordType*>(buf)), m_BufEnd(m_Buf + length), m_BufInital(m_Buf) {}	\
																													\
		ErrorCode Write(u32 unicode);																				\
		inline std::size_t WordsWritten() const { return m_Buf - m_BufInital; }										\
		inline std::size_t BytesWritten() const { return WordsWritten() * sizeof(ClassName); }						\
		inline std::size_t CharactersWritten() const { return m_CharacterIndex; }									\




#define CHARSET_ENCODER_PRIVATE_METHODS_DEF(ClassName)			\
		ErrorCode WriteWord(ClassName::WordType word);			\
		ErrorCode UnsupportedCharacterError(u32 unicode);		\


#define CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(ClassName)		\
		ClassName::WordType* m_Buf;								\
		ClassName::WordType* const m_BufEnd;					\
		ClassName::WordType* const m_BufInital;					\
		std::size_t m_CharacterIndex = 0;						\



	template<>
	struct CharsetEncoder<utf8>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf8)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(utf8)
				
	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf8)

	};


	template<>
	struct CharsetEncoder<utf16>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf16)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(utf16)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf16)

	};



	template<>
	struct CharsetEncoder<utf32>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf32)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(utf32)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf32)

	};





	template<>
	struct CharsetEncoder<esc4>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc4)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(esc4)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc4)
		esc4::WordType m_CurrentWord;
	};


	template<>
	struct CharsetEncoder<esc6>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc6)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(esc6)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc6)
		u32 m_State;
	};



	template<>
	struct CharsetEncoder<esc8>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc8)

	private: CHARSET_ENCODER_PRIVATE_METHODS_DEF(esc8)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc8)

	};




	const utf8* SL(const char* literal);
	const utf16* SL(const char16_t* literal);
	const utf32* SL(const char32_t* literal);

}
