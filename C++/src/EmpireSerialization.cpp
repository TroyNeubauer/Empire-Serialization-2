#include "EmpireSerialization2.h"

#include "Internal.h"

#include <cstring>
#include <cstdio>
#include <cinttypes>

namespace ES {

	const Error& GetError()
	{
		return Internal::GetError();
	}
	
	void ClearError()
	{
		Internal::GetError().Type = ErrorCode::NONE;
	}

	namespace ToString {

		void PrintError(Formatter& formatter, const Error& error)
		{
			switch(error.Type)
			{
				case ErrorCode::NONE: formatter << "No Error"; break;
				
				case ErrorCode::INVALID_CHARACTER:
				{
					uint32_t unicode = error.InvalidCharacter.Char;
					if (unicode < 128)
					{
						formatter.W("Invalid character U+").Base(unicode, 16) << " (" << static_cast<char>(unicode) << 
							") at character " << error.InvalidCharacter.Position.Characters << " (byte " << error.InvalidCharacter.Position.Bytes << 
							") for charset " << GetCharsetString(error.InvalidCharacter.CharacterSet);
					}
					else
					{
						//Dont attempt to print non ASCII characters because of bad terminal handling
						formatter.W("Invalid character U+").Base(unicode, 16) << ") at character " << error.InvalidCharacter.Position.Characters <<
							" (byte " << error.InvalidCharacter.Position.Bytes << ") for charset " << GetCharsetString(error.InvalidCharacter.CharacterSet);
					}
					
					break;
				}
				case ErrorCode::NOT_IMPLEMENTED:
					formatter << "Feature \"" << error.NotImplemented.Feature << "\" is not implemented in this build of Empier Serialization!";
					break;
				
				case ErrorCode::BUFFER_OVERFLOW:
					formatter << "Buffer overflow. Buffer size " << error.BufferOverflow.BufferSize << " bytes, size needed " << error.BufferOverflow.RequiredSize << " bytes";
					break;
				
			}
		}

		const char* GetErrorCodeString(ErrorCode code)
		{
			switch(code)
			{
				case ErrorCode::NONE: return "No Error";
				case ErrorCode::INVALID_CHARACTER: return "Invalid Character";
				case ErrorCode::NOT_IMPLEMENTED: return "Not Implemented";
				case ErrorCode::BUFFER_OVERFLOW: return "Buffer Overflow";
				default: ES_ABORT("Invalid error code!");
			}
			
		}

		const char* GetCharsetString(Charset charset)
		{
			switch(charset)
			{
				case Charset::UTF8: return "UTF8";
				case Charset::UTF16:return "UTF16";
				case Charset::UTF32:return "UTF32";
				case Charset::ESC4: return "ESC4";
				case Charset::ESC6: return "ESC6";
				case Charset::ESC8: return "ESC8+";
				default: ES_ABORT("Invalid charset!");
			}
		}

	}

	
	void SetAllocErrorHandler(AllocErrorHandler handler)
	{
		Internal::SetAllocErrorHandler(handler);
	}

}