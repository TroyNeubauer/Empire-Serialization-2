#pragma once

#include "Format.h"

namespace ES {
	const int ES_ASSERT_LEVEL_NONE = 0;

	const int ES_ASSERT_LEVEL_MIN = 1;
	const int ES_ASSERT_LEVEL_MAX = 2;

	#ifndef ES_ASSERT_LEVEL
		#define ES_ASSERT_LEVEL ES_ASSERT_LEVEL_MAX
	#endif

	//Not enum class to allow for if (ErrorCode) without the compiler complaining
	enum ErrorCode
	{
		NONE = 0,
		//Indicates that no erroe has taken place
		
		UNSUPPORTED_CHARACTER,
		//Thrown when a character set cannot represent a character
		
		INVALID_CHARACTER,
		//Thrown when an invalid character is read suring string decoding

		NOT_IMPLEMENTED,
		//Thrown when a function cannot complete its job because the code to execute perform some task
		//in not present in this binary.
		//This can be because the user configured Empire Serialization to omit rarely used features 
		//(like FIXME: add example) during compilation to reduce the size of the binary, or this can also be thrown
		//when somthing is a work in progress on the devolepment branch

		BUFFER_OVERFLOW,
		//Thrown when a write is attempted on a full buffer
		
		BUFFER_UNDERFLOW
		//Thrown when a buffer is attempted to be read past its end and no data is avilable
	};

	struct Error
	{
		ErrorCode Type;
		SizedFormatter<128> AdditionalMessage;

		union 
		{
			//Thrown during character set conversions when a character cannot be represented in the destination charset
			struct
			{
				u32 Char;// unicode value
				Charset CharacterSet;
				CharacterIndex Position;//Information about where the unsupported character is. In bytes and characters
			} UnsupportedCharacter;

			//Thrown when a character is not recognised during string decoding
			struct
			{
				u32 Char;
				Charset CharacterSet;
				CharacterIndex Position;
			} InvalidCharacter;
			
			struct
			{
				const char* Feature;
			} NotImplemented;

			struct
			{
				std::size_t BufferSize;
				std::size_t RequiredSize;
			} BufferOverflow;

			struct
			{
				std::size_t BufferSize;
				std::size_t RequiredSize;
			} BufferUnderflow;

		};
	};

	const Error& GetError();
	void ClearError();



}