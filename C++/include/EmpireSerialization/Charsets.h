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

	template<typename T, std::size_t DecodeLen, std::size_t EncodeLen>
	constexpr const std::array<T, EncodeLen> ConstructEncoding(const std::array<T, DecodeLen>& decoding)
	{
		std::array<T, EncodeLen> encoding {};
		for (int i = 0; i < EncodeLen; i++)
			encoding[i] = static_cast<T>(-1);
		for (std::size_t i = 0; i < DecodeLen; i++)
		{
			encoding[decoding[i]] = i;
		}
		
		return encoding;
	}

	//XXX_DECODE allows users to obtain the unicode value for the character value in another encoding scheme
	//XXX_ENCODE is the mathmatical inverse function XXX_DECODE
	constexpr const std::array<char, 16> ESC4_DECODE = { 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'u', 'c', 'm', 'w', 'f' };
	static constexpr const std::size_t ESC4_ENCODE_SIZE = EncodingSize(ESC4_DECODE);
	constexpr const std::array<char, ESC4_ENCODE_SIZE> ESC4_ENCODE = ConstructEncoding<char, ESC4_DECODE.size(), ESC4_ENCODE_SIZE>(ESC4_DECODE);

	constexpr const std::array<char, 64> ESC6_DECODE =
	{ 
		' ', '\n', '.', ',', '\'', '\"', '!', '?', ':', ';', '-', '_', 'a', 'b', 'c', 'd',
		'e',  'f', 'g', 'h',  'i',  'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
		'u',  'v', 'w', 'x',  'y',  'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
		'K',  'L', 'M', 'N',  'O',  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	};
	static constexpr const std::size_t ESC6_ENCODE_SIZE = EncodingSize(ESC6_DECODE);
	constexpr const std::array<char, ESC6_ENCODE_SIZE> ESC6_ENCODE = ConstructEncoding<char, ESC6_DECODE.size(), ESC6_ENCODE_SIZE>(ESC6_DECODE);


	//Represents an abstract reader
	//The Read method can be called to obtain decoded unicode codepoints from the buffer
	//This class is not functional. Its only purpose it to cleanly illustrate the API because the actual 
	//CharsetDecoder<utf8>, CharsetDecoder<utf16> implementations use a lot of macros and as a result look nasty.
	template<typename T>
	struct CharsetDecoder
	{
	private:
		//Initalizes this reader with a bolb words for a particular charset
		//that will be decoded into unicode codepoints
		CharsetDecoder(const T::WordType* buf, std::size_t length);

		//Reads a single character from this stream, and store its unicode value into codepoint.
		//If an error happens while obtaining the character, the errorcode is returned to
		//the user and GetError() is set appropiatly
		ErrorCode Read(u32& codepoint);
		
		std::size_t BytesRead() const;
		std::size_t WordsRead() const;
		std::size_t CharactersRead() const;

	};

	template<typename T>
	struct CharsetEncoder
	{
	private:
		//Initalizes this reader with a binary bolb of data that will be used to write words of type T
		CharsetEncoder(u8* buf, std::size_t bytes);

		//Reads a single character from this stream into character. If an error happens while obtaining the character,
		//the errorcode is returned to the user and GetError() is set appropiatly
		ErrorCode Write(u32 unicode);

		std::size_t BytesWritten() const;

	};


}
