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

namespace ES {
	namespace Conversions {

		template<typename SrcType, typename DestType>
		struct Exists
		{
			static const bool value =
				(//string to string
					(std::is_same<SrcType, utf8>::value || std::is_same<SrcType, utf16>::value || std::is_same<SrcType, utf32>::value ||
						std::is_same<SrcType, esc4>::value || std::is_same<SrcType, esc6>::value || std::is_same<SrcType, esc8>::value
					) 
					&&
					(std::is_same<DestType, utf8>::value || std::is_same<DestType, utf16>::value || std::is_same<DestType, utf32>::value ||
						std::is_same<DestType, esc4>::value || std::is_same<DestType, esc6>::value || std::is_same<DestType, esc8>::value
					)
				)
				||
				(//numeric to numeric
					false
				);
		};

		//Converts arrays of one type to arrays of another
		//Most commonly this is used for string conversions
		//Errors include BufferOverflow, BufferUnderflow, InvalidCharacter and UnsupportedCharacter
		template<typename SameType>
		ErrorCode Convert(const SameType* src, size_t srcBytes, SameType* dest, size_t destBytes, StringCodingData& data)
		{
			//Nop. SrcType and DestType are the same

			return ErrorCode::NONE;
		}

		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcBytes, DestType* dest, size_t destBytes, StringCodingData& data);

	}

}
