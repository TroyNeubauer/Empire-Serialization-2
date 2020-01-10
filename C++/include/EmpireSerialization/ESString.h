//===================================================================================
// MIT Liscense
// Copyright (c) 2020, Troy Neubauer
//
//	File: String.h
//	Contains definitions for querying information on strings. For a given string:
//
//	- Wordcount Refers to the number of words charset dependent words needed to
//		store the characters of the string. Wordcount() is identical strlen() returns
//		except we provide
//		implementations for all charsets (utf8, utf16, utf32, esc4, esc6, esc8...)
//
//	- Bytes Which returns the number of bytes needed to store the string Wordcount * the
//		size of a word. 
//

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

		template<typename T>
		std::size_t CharacterCount(const T* string);


	}
}

