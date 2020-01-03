#include "EmpireSerialization/Conversions.h"

#include "EmpireSerialization2.h"
#include "Internal.h"
#include "Charsets.h"

namespace ES {
	namespace Conversions {

		template<typename SrcType, typename DestType>
		std::size_t RequiredCapacity(std::size_t bytes)
		{
			ES_ABORT("Conversion not allowed!");
		}


		//In the worst case, utf8 storing ASCII will be 4x larger
		template<> std::size_t RequiredCapacity <utf8, utf32>(std::size_t bytes) { return bytes * 4; }
		template<> std::size_t RequiredCapacity<utf16, utf32>(std::size_t bytes) { return bytes * 2; }
		template<> std::size_t RequiredCapacity <esc4, utf32>(std::size_t bytes) { return bytes * 8; }
		template<> std::size_t RequiredCapacity <esc6, utf32>(std::size_t bytes) { return bytes * 6; }
		template<> std::size_t RequiredCapacity <esc8, utf32>(std::size_t bytes) { return bytes * 2; }

		//a utf8/utf16 string will never be longer than a utf32 one
		template<> std::size_t RequiredCapacity<utf32,  utf8>(std::size_t bytes) { return bytes; }
		template<> std::size_t RequiredCapacity<utf32, utf16>(std::size_t bytes) { return bytes; }
		//esc* will be much smaller (assuming the utf32 doesnt use any unsupported characters)
		template<> std::size_t RequiredCapacity<utf32,  esc4>(std::size_t bytes) { return Internal::Math::DivideCeli<std::size_t>(bytes, 8); }
		template<> std::size_t RequiredCapacity<utf32,  esc6>(std::size_t bytes) { return Internal::Math::DivideCeli<std::size_t>(bytes, 6); }
		template<> std::size_t RequiredCapacity<utf32,  esc8>(std::size_t bytes) { return Internal::Math::DivideCeli<std::size_t>(bytes, 4); }


		struct StringConversionsState
		{
			Charset SrcCharset;
			Charset DestCharset;

			std::size_t WordReadIndex;
			std::size_t WordWriteIndex;
			
			std::size_t CharacterIndex;

		};

		static StringConversionsState s_StringState;

		template<typename SrcType, typename DestType>
		ErrorCode ConvertStringImpl(const SrcType* src, std::size_t srcLength, DestType* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ES_ABORT("Conversion not allowed!");
		}


		//Uses other specalized conversions and a transient intermediate Type to convert between two otherwise inconvertable types
		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcBytes, DestType* dest, size_t destBytes, StringCodingData& data)
		{
			static_assert(!std::is_same<SrcType, DestType>::value, "Types must be different!");
			static_assert(Exists<SrcType, DestType>::value, "Invalid conversion!");

			//String conversion. Use utf32 as the intermediate type because all charsets can read/write to/from utf32
			if (std::is_same<SrcType, utf8>::value || std::is_same<SrcType, utf16>::value || std::is_same<SrcType, utf32>::value 
				|| std::is_same<SrcType, esc4>::value || std::is_same<SrcType, esc6>::value || std::is_same<SrcType, esc8>::value)
			{
				static_assert(std::is_same<DestType, utf8>::value || std::is_same<DestType, utf16>::value || std::is_same<DestType, utf32>::value 
					|| std::is_same<DestType, esc4>::value || std::is_same<DestType, esc6>::value || std::is_same<DestType, esc8>::value);
				
				std::size_t tempWords = RequiredCapacity<SrcType, utf32>(srcBytes);
				Internal::TempBuffer<utf32> temp(tempWords);

				ErrorCode errorCode = ErrorCode::NONE;
				Error& currentError = Internal::GetError();
				Charset srcCharsetCode = GetCharsetCode<SrcType>::Code;
				Charset destCharsetCode = GetCharsetCode<DestType>::Code;

				data.Characters = 0;
				data.Words = 0;
				data.CharacterSet = Charset::INVALID;

				s_StringState.CharacterIndex = 0;
				s_StringState.SrcCharset = srcCharsetCode;
				s_StringState.DestCharset = Charset::UTF32;
				s_StringState.WordReadIndex = 0;
				s_StringState.WordWriteIndex = 0;
				//Perform the conversion returning errors to the caller
				if (errorCode = ConvertStringImpl<SrcType, utf32>(src, srcBytes / GetCharsetInfo(srcCharsetCode).WordSize,
					temp.Get(), tempWords, s_StringState.CharacterIndex, currentError.AdditionalMessage))
				{
					return errorCode;
				}
				size_t wordsWritten = s_StringState.WordWriteIndex;
				//Reset the state
				s_StringState.CharacterIndex = 0;
				s_StringState.SrcCharset = Charset::UTF32;
				s_StringState.DestCharset = destCharsetCode;
				s_StringState.WordReadIndex = 0;
				s_StringState.WordWriteIndex = 0;
				if (errorCode = ConvertStringImpl<utf32, DestType>(temp.Get(), wordsWritten, 
					dest, destBytes / GetCharsetInfo(destCharsetCode).WordSize, s_StringState.CharacterIndex, currentError.AdditionalMessage))
				{
					return errorCode;
				}
				
				data.Words = s_StringState.WordWriteIndex;
				data.Characters = s_StringState.CharacterIndex;
				data.CharacterSet = destCharsetCode;

				return ErrorCode::NONE;
			}
		}

		static ErrorCode InvalidCharacterError(uint32_t invalidChar)
		{
			Error& error = Internal::GetError();
			error.InvalidCharacter.Char = invalidChar;
			error.InvalidCharacter.CharacterSet = s_StringState.SrcCharset;

			error.InvalidCharacter.Position.Word = s_StringState.WordReadIndex - 1;//minus one because end points to the next character to be read
			error.InvalidCharacter.Position.Character = s_StringState.CharacterIndex;

			return error.Type = ErrorCode::INVALID_CHARACTER;
		}

		static ErrorCode UnsupportedCharacterError(uint32_t unsupportedChar)
		{
			Error& error = Internal::GetError();
			error.UnsupportedCharacter.Char = unsupportedChar;
			error.UnsupportedCharacter.CharacterSet = s_StringState.DestCharset;

			error.UnsupportedCharacter.Position.Word = s_StringState.WordReadIndex - 1;
			error.UnsupportedCharacter.Position.Character = s_StringState.CharacterIndex;

			return error.Type = ErrorCode::UNSUPPORTED_CHARACTER;
		}

		template<typename ErrorCharset>
		static ErrorCode BufferOverflowError(ErrorCharset* begin, ErrorCharset* end)
		{
			Error& error = Internal::GetError();
			error.BufferOverflow.BufferSize = (end - begin) * sizeof(begin[0]);
			error.BufferOverflow.RequiredSize = error.BufferOverflow.BufferSize + GetCharsetInfo(GetCharsetCode<ErrorCharset>::Code).WordSize;

			return error.Type = ErrorCode::BUFFER_OVERFLOW;
		}

		template<typename ErrorCharset>
		static ErrorCode BufferUnderflowError(const ErrorCharset* begin, const ErrorCharset* end)
		{
			Error& error = Internal::GetError();
			error.BufferUnderflow.BufferSize = (end - begin) * sizeof(begin[0]);
			error.BufferUnderflow.RequiredSize = error.BufferUnderflow.BufferSize + GetCharsetInfo(GetCharsetCode<ErrorCharset>::Code).WordSize;

			return error.Type = ErrorCode::BUFFER_UNDERFLOW;
		}

		//Places the next character in s_StringState.CurrentChar, or returns an error if one happened
		template<typename CharsetType, typename DestPtr = const CharsetType*>
		static ErrorCode ReadCharacter(CharsetType& c, DestPtr& src, DestPtr srcEnd)
		{
#if ES_ASSERT_LEVEL >= ES_ASSERT_LEVEL_MIN
			if (src >= srcEnd)
			{
				return BufferUnderflowError<CharsetType>(src, srcEnd);
			}
#endif
			c = *src++;
			s_StringState.WordReadIndex++;

			return ErrorCode::NONE;
		}

		template<typename CharsetType, typename DestPtr = CharsetType*>
		static ErrorCode WriteCharacter(CharsetType c, DestPtr& dest, DestPtr destEnd)
		{
#if ES_ASSERT_LEVEL >= ES_ASSERT_LEVEL_MIN
			if (dest >= destEnd)
			{
				return BufferOverflowError<CharsetType>(dest, destEnd);
			}
#endif
			*dest++ = c;
			s_StringState.WordWriteIndex++;

			return ErrorCode::NONE;
		}


		template<typename SrcType, typename DestType>
		ErrorCode ConvertStringImplWrapper(const SrcType* src, size_t srcBytes, DestType* dest, size_t destBytes, StringCodingData& data)
		{
			ErrorCode errorCode;
			Charset srcCharsetCode = GetCharsetCode<SrcType>::Code;
			Charset destCharsetCode = GetCharsetCode<DestType>::Code;

			s_StringState.CharacterIndex = 0;
			s_StringState.SrcCharset = srcCharsetCode;
			s_StringState.DestCharset = destCharsetCode;
			s_StringState.WordReadIndex = 0;
			s_StringState.WordWriteIndex = 0;
			if (errorCode = ConvertStringImpl<SrcType, DestType>(src, srcBytes / GetCharsetInfo(srcCharsetCode).WordSize, 
				dest, destBytes / GetCharsetInfo(destCharsetCode).WordSize, s_StringState.CharacterIndex, Internal::GetError().AdditionalMessage))
			{
				return errorCode;
			}
			
			data.Words = s_StringState.WordWriteIndex;
			data.Characters = s_StringState.CharacterIndex;
			data.CharacterSet = destCharsetCode;

			return ErrorCode::NONE;
			return ErrorCode::NOT_IMPLEMENTED;
		}

		template<>
		ErrorCode Convert<utf8, utf32>(const utf8* src, size_t srcBytes, utf32* dest, size_t destBytes, StringCodingData& data)
		{
			return ConvertStringImplWrapper(src, srcBytes, dest, destBytes, data);
		}

		//Implement core to/from UTF32 functions
		template<>
		ErrorCode ConvertStringImpl<utf8, utf32>(const utf8* src, std::size_t srcLength, utf32* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;
			utf32 codepoint;

			//Constants to make byte/length calculations easier
			const utf8* srcEnd = src + srcLength;
			utf32* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				std::size_t surrogateCount = 0;
				utf8 c;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				if (Internal::TopNBitsAre<1, 0b0>(c))// 7bit ASCII
				{
					codepoint = c;//Easy handling for single byte utf8 characters
				}
				else if (Internal::TopNBitsAre<3, 0b110>(c))// 11 bits
				{
					surrogateCount = 1;
				}
				else if (Internal::TopNBitsAre<4, 0b1110>(c))// 16 bits
				{
					surrogateCount = 2;
				}
				else if (Internal::TopNBitsAre<5, 0b11110>(c))// 21 bits
				{
					surrogateCount = 3;
				}
				//Handle invalid bit patterns
				else if (Internal::TopNBitsAre<2, 0b10>(c))// Invalid. Middle of character
				{
					errorFormatter.Write("Mis-aligned UTF8 character: 0b").Base(c, 2)
						.Write(" The high bits of a new character must start with with 0, or 110, or 1110, or 11110 per the utf8 standard");
					return InvalidCharacterError(c);
				}
				else // Unsupported bit format Could be a 26 or 31 or 36 bit encoding which are disallowed
				{
					errorFormatter.Write("Invalid UTF8 bit format: 0b").Base(c, 2);
					return InvalidCharacterError(c);
				}

				//We know how many bytes need to be read for multi byte encoding
				if (surrogateCount != 0)
				{
					//Save bottom X bytes of the first byte according to the utf8 pattern
					codepoint = Internal::BottomBits(6 - surrogateCount, c);
					int i = 0;
					while (surrogateCount)
					{
						//Look for buffer overflow, read char and check for errors in the second byte in this character
						if (code = ReadCharacter(c, src, srcEnd)) return code;
						if (!Internal::TopNBitsAre<2, 0b10>(c))
						{
							(errorFormatter << "The UTF8 character surrogate #" << i << " has invaid top bits. Expected 0b11... Got 0b").Base(c, 2);
							return InvalidCharacterError(c);
						}
						//Append aurrogate to codepoint
						codepoint = (codepoint << 6) | Internal::BottomBits<6>(c);

						surrogateCount--;
						i++;
					}
				}
				
				if (code = WriteCharacter(codepoint, dest, destEnd)) return code;

			}

			return ErrorCode::NONE;

		}

		template<>
		ErrorCode ConvertStringImpl<utf32, esc4>(const utf32* src, std::size_t srcLength, esc4* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode error;
			esc4 result;
			
			const utf32* srcEnd = src + srcLength;
			esc4* destEnd = dest + destLength;
			while (src != srcEnd)
			{ 
				utf32 codepoint;
				if (error = ReadCharacter(codepoint, src, srcEnd)) return error;
				esc4 encoded;
				if (codepoint >= ESC4_ENCODE.size() || (encoded = ESC4_ENCODE[codepoint]) == -1) return UnsupportedCharacterError(codepoint);

				if (characters % 2 == 0)
				{
					result = encoded << 4;
				}
				else
				{
					result |= encoded;
					if (error = WriteCharacter(result, dest, destEnd)) return error;
				}
				characters++;
				
			}

			//Write the last byte if there is an odd number of characters
			if (characters % 2 == 1)
				if (error = WriteCharacter(result, dest, destEnd)) return error;
			
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode ConvertStringImpl<utf32, esc6>(const utf32* src, std::size_t srcLength, esc6* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			uint32_t part;
			ErrorCode error;
			
			const utf32* srcEnd = src + srcLength;
			esc6* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				utf32 codepoint;
				if (error = ReadCharacter(codepoint, src, srcEnd)) return error;

				int8_t encoded;
				if (codepoint >= ESC6_ENCODE.size() || (encoded = ESC6_ENCODE[codepoint]) == -1) return UnsupportedCharacterError(codepoint);

				if (characters % 2 == 0)
				{
					//top 6 bits of 24 bit word
					part = (encoded << 18);
				}
				else if (characters % 2 == 1)
				{
					part |= (encoded << 12);
				}
				else if (characters % 2 == 2)
				{
					part |= (encoded << 6);
				}
				else if (characters % 2 == 3)
				{
					part |= (encoded << 6);
					//Write all three bytes
					if (error = WriteCharacter(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
					if (error = WriteCharacter(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
					if (error = WriteCharacter(static_cast<esc6>((part >>  0) & 0xFF), dest, destEnd)) return error;
				}
				characters++;
			}

			//Write the remaining characters if the string wasnt a mutiple of 4
			if (characters % 4 == 1)//Top byte
			{
				//Bytes 18-24 have data
				if (error = WriteCharacter(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
			}
			else if (characters % 4 == 2)
			{
				//Bytes 12-24 have data
				if (error = WriteCharacter(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
				if (error = WriteCharacter(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
			}
			else if (characters % 4 == 3)
			{
				//Bytes 6-24 have data
				if (error = WriteCharacter(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
				if (error = WriteCharacter(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
				if (error = WriteCharacter(static_cast<esc6>((part >>  0) & 0xFF), dest, destEnd)) return error;
			}
			
			return ErrorCode::NONE;
		}



		template ErrorCode Convert<utf8, esc4>(const utf8*, size_t, esc4*, size_t, StringCodingData&);
		template ErrorCode Convert<utf8, esc6>(const utf8*, size_t, esc6*, size_t, StringCodingData&);

	}
}
