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
	constexpr const std::size_t ESC4_ENCODE_SIZE = EncodingSize(ESC4_DECODE);
	constexpr const std::array<char, ESC4_ENCODE_SIZE> ESC4_ENCODE = ConstructEncoding<char, ESC4_DECODE.size(), ESC4_ENCODE_SIZE>(ESC4_DECODE);

}

