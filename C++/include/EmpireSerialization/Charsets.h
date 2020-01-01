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
		return result;
	}

	template<typename T, std::size_t Len>
	constexpr const std::array<T, Len> ConstructEncoding(const T* decoding)
	{
		std::array<T, Len> result;

		return result;
	}

	//XXX_DECODE allows users to obtain the unicode value for the character value in another encoding scheme
	//XXX_ENCODE is the mathmatical inverse function XXX_DECODE
	constexpr const std::array<char, 16> ESC4_DECODE = { 'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'c', 'u', 'm', 'w', 'f' };
	constexpr const std::size_t ESC4_ENCODE_SIZE = EncodingSize(ESC4_DECODE);
	constexpr const std::array<char, ESC4_ENCODE_SIZE> ESC4_ENCODE = ConstructEncoding<char, ESC4_ENCODE_SIZE>(ESC4_DECODE);
}

