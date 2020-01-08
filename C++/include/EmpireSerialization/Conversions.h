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

		//Converts strings or arrays from one type to another
		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcWords, DestType* dest, size_t destWords);



	}

}
