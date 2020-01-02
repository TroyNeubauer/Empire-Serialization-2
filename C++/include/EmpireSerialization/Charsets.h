#pragma once

#include <cstdint>
#include <array>

namespace ES {

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
	constexpr const std::array<char, 16> ESC4_DECODE = { 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'c', 'u', 'm', 'w', 'f' };
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


}
