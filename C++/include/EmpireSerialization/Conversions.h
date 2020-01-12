//===================================================================================
// MIT Liscense
// Copyright (c) 2020, Troy Neubauer
//
//	File: Conversions.h
//	Contains definitions for converting arrays of types to other types. For now a 
//		string of any charset to any other charset. This will be expanded later to
//		support converting arrays of primitive types. 
//

#pragma once

#include "EmpireSerialization2.h"
#include "Charsets.h"

namespace ES {

	namespace Conversions {

		//Determines how large the worst case buffer needs to be when converting
		template<typename SrcType, typename DestType>
		std::size_t RequiredCapacity(std::size_t srcWords)
		{
			if (std::is_same<SrcType, DestType>::value)
			{
				//Converting somthing to itself always requires only the same amount of words
				return srcWords;
			}
			else
			{
				std::size_t utf32Words = RequiredCapacity<SrcType, utf32>(srcWords);
				return RequiredCapacity<utf32, DestType>(utf32Words);
			}
		}

		template<> std::size_t RequiredCapacity <utf8, utf32>(std::size_t words);
		template<> std::size_t RequiredCapacity<utf16, utf32>(std::size_t words);
		template<> std::size_t RequiredCapacity <esc4, utf32>(std::size_t words);
		template<> std::size_t RequiredCapacity <esc6, utf32>(std::size_t words);
		template<> std::size_t RequiredCapacity <esc8, utf32>(std::size_t words);

		template<> std::size_t RequiredCapacity<utf32, utf8>(std::size_t words);
		template<> std::size_t RequiredCapacity<utf32, utf16>(std::size_t words);
		template<> std::size_t RequiredCapacity<utf32, esc4>(std::size_t words);
		template<> std::size_t RequiredCapacity<utf32, esc6>(std::size_t words);
		template<> std::size_t RequiredCapacity<utf32, esc8>(std::size_t words);

		template<> std::size_t RequiredCapacity<utf8, utf8>(size_t words);
		template<> std::size_t RequiredCapacity<utf16, utf16>(size_t words);
		template<> std::size_t RequiredCapacity<utf32, utf32>(size_t words);
		template<> std::size_t RequiredCapacity<esc4, esc4>(size_t words);
		template<> std::size_t RequiredCapacity<esc6, esc6>(size_t words);
		template<> std::size_t RequiredCapacity<esc8, esc8>(size_t words);

		template<typename DestType, typename SrcType>
		void StoreData(const CharsetEncoder<DestType>& encoder, const CharsetDecoder<SrcType>& decoder, StringCodingData& data)
		{
			data.Characters = encoder.CharactersWritten();
			data.SrcCharacterSet = GetCharsetCode<SrcType>::Code;
			data.DestCharacterSet = GetCharsetCode<DestType>::Code;
			data.WordsRead = decoder.WordsRead();
			data.WordsWritten = encoder.WordsWritten();
		}

		template<typename SrcType, typename DestType>
		ErrorCode ConvertString(const SrcType* src, size_t srcWords, DestType* dest, size_t destWords, StringCodingData& data)
		{
			CharsetDecoder<SrcType> decoder(src, srcWords);
			CharsetEncoder<DestType> encoder(dest, destWords);
			ErrorCode code;

			while (decoder.HasChars())
			{
				u32 codepoint;
				if (code = decoder.Read(codepoint)) { StoreData(encoder, decoder, data); return code; }
				if (code = encoder.Write(codepoint)) { StoreData(encoder, decoder, data); return code; }
			}
			StoreData(encoder, decoder, data);

			return ErrorCode::NONE;
		}

	}

}
