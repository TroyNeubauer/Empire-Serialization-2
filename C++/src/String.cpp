
#include "EmpireSerialization2.h"
#include "String.h"
#include "Internal.h"

namespace ES {
	namespace String {

		void PrintError(Formatter& formatter, const Error& error)
		{
			switch(error.Type)
			{
				case ErrorCode::NONE: formatter << "No Error"; break;
				
				case ErrorCode::UNSUPPORTED_CHARACTER:
				{
					u32 unicode = error.UnsupportedCharacter.Char;
					if (unicode < 128)
					{
						formatter.W("Unsupported character U+").Base(unicode, 16) << " (" << static_cast<char>(unicode) << 
							") at character " << error.UnsupportedCharacter.Position.Character << " (word " << error.UnsupportedCharacter.Position.Word << 
							") for charset " << GetCharsetString(error.UnsupportedCharacter.CharacterSet);
					}
					else
					{
						//Dont attempt to print non ASCII characters because of bad terminal handling
						formatter.W("Unsupported character U+").Base(unicode, 16) << ") at character " << error.UnsupportedCharacter.Position.Character <<
							" (word " << error.UnsupportedCharacter.Position.Word << ") for charset " << GetCharsetString(error.UnsupportedCharacter.CharacterSet);
					}
					
					break;
				}
				case ErrorCode::NOT_IMPLEMENTED:
					formatter << "Feature \"" << error.NotImplemented.Feature << "\" is not implemented in this build of Empier Serialization!";
					break;
				
				case ErrorCode::BUFFER_OVERFLOW:
					formatter << "Buffer overflow. Not enough space to hold new bytes. Buffer size " 
						<< error.BufferOverflow.BufferSize << " bytes, size needed " << error.BufferOverflow.RequiredSize << " bytes";
					break;

				case ErrorCode::BUFFER_UNDERFLOW:
					formatter << "Buffer underflow. Buffer is out of bytes to read. Buffer size " 
						<< error.BufferUnderflow.BufferSize << " bytes, size needed " << error.BufferUnderflow.RequiredSize << " bytes";
					break;
			}
		}

		const char* GetErrorCodeString(ErrorCode code)
		{
			switch(code)
			{
				case ErrorCode::NONE: return "No Error";
				case ErrorCode::UNSUPPORTED_CHARACTER: return "Unsupported Character";
				case ErrorCode::NOT_IMPLEMENTED: return "Not Implemented";
				case ErrorCode::BUFFER_OVERFLOW: return "Buffer Overflow";
				default: ES_ABORT("Invalid error code!");
			}
			
		}

		const char* GetCharsetString(Charset charset)
		{
			return GetCharsetInfo(charset).Name;
		}

		//TODO use bit twiddling to read mutiple words at a time
		template<typename T>
		static std::size_t NullTerminatedWordCount(const T* ptr)
		{
			const T* start = ptr;
			while (*ptr)
			{
				ptr++;
			}
			return ptr - start;
		}

		template<>
		std::size_t WordCount(const utf8* string)
		{
			return NullTerminatedWordCount(string);
		}

		template<>
		std::size_t WordCount(const utf16* string)
		{
			return NullTerminatedWordCount(string);
		}

		template<>
		std::size_t WordCount(const utf32* string)
		{
			return NullTerminatedWordCount(string);
		}

		template<> std::size_t WordCount(const esc4* string) { return UNABLE_TO_QUERY_BYTES; }
		template<> std::size_t WordCount(const esc6* string) { return UNABLE_TO_QUERY_BYTES; }
	

		template<>
		std::size_t WordCount(const esc8* string)
		{
			return 0;//TODO
		}

		template<typename T>
		std::size_t CharacterCount(const T* string)
		{
			
			return WordCount(string);
		}

		template std::size_t CharacterCount(const utf8*);
		template std::size_t CharacterCount(const utf16*);
		template std::size_t CharacterCount(const utf32*);
		template std::size_t CharacterCount(const esc4*);
		template std::size_t CharacterCount(const esc6*);
		template std::size_t CharacterCount(const esc8*);
	}
}

