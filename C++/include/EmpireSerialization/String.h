#pragma once

#include "EmpireSerialization2.h"
#include "Conversions.h"

namespace ES {

	namespace String {
		void PrintError(Formatter& formatter, const Error& error);
		const char* GetErrorCodeString(ErrorCode code);
		const char* GetCharsetString(Charset charset);

		constexpr std::size_t UNABLE_TO_QUERY_WORD_COUNT = static_cast<std::size_t>(-1);
		constexpr std::size_t UNABLE_TO_QUERY_CHARACTER_COUNT = UNABLE_TO_QUERY_WORD_COUNT;
		constexpr std::size_t UNABLE_TO_QUERY_BYTES = UNABLE_TO_QUERY_WORD_COUNT;


		template<typename T> std::size_t WordCount(const T* string);

		template<typename T> std::size_t Bytes(const T* string)
		{
			std::size_t wordCount = WordCount<T>(string);
			if (wordCount == UNABLE_TO_QUERY_WORD_COUNT) return UNABLE_TO_QUERY_BYTES;
			return wordCount * GetCharsetInfo(GetCharsetCode<T>::Code).WordSize;
		}

		template<typename T> std::size_t CharacterCount(const T* string)
		{
			StringCodingData data;
			std::size_t bytes = Bytes<T>(string);
			if (bytes == UNABLE_TO_QUERY_BYTES) return UNABLE_TO_QUERY_CHARACTER_COUNT;
			Conversions::Convert<T, utf32>(string, bytes, nullptr, 0, data);
			return data.Characters;
		}
	}
}

