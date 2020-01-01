#pragma once

#include "EmpireSerialization2.h"

namespace ES {
	namespace Conversions {

		struct StringCodingData
		{
			std::size_t Characters;
			std::size_t Bytes;
		};

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

		template<typename SrcType, typename DestType>
		std::size_t RequiredCapacity(std::size_t bytes);

		template<typename SameType>
		ErrorCode Convert(const SameType* src, size_t srcBytes, SameType* dest, size_t destCapacity, StringCodingData& data)
		{
			//Nop. SrcType and DestType are the same

			return ErrorCode::NONE;
		}

		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcBytes, DestType* dest, size_t destCapacity, StringCodingData& data);


		//Explicit specializations

	}

}
