#include "EmpireSerialization/Conversions.h"

#include "EmpireSerialization2.h"
#include "Internal.h"
#include "String.h"
#include "Charsets.h"

namespace ES {
	namespace Conversions {


		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcWords, DestType* dest, size_t destWords)
		{
			if (Info::IsCharacterType<SrcType>::value && Info::IsCharacterType<DestType>::value)
			{
				
			}
			else
			{
				//Normal array conversion
			}
			
		}

		static ErrorCode InvalidCharacterError(u32 invalidChar)
		{
			Error& error = Internal::GetError();
			error.InvalidCharacter.Char = invalidChar;
			error.InvalidCharacter.CharacterSet = s_StringState.SrcCharset;

			error.InvalidCharacter.Position.Word = s_StringState.WordReadIndex - 1;//minus one because end points to the next character to be read
			error.InvalidCharacter.Position.Character = s_StringState.CharacterIndex;

			s_StringState.DataPtr->Characters = s_StringState.CharacterIndex;
			s_StringState.DataPtr->WordsRead = s_StringState.WordReadIndex;
			s_StringState.DataPtr->WordsWritten = s_StringState.WordWriteIndex;
			
			s_StringState.DataPtr = nullptr;
			return error.Type = ErrorCode::INVALID_CHARACTER;
		}

		static ErrorCode UnsupportedCharacterError(u32 unsupportedChar)
		{
			Error& error = Internal::GetError();
			error.UnsupportedCharacter.Char = unsupportedChar;
			error.UnsupportedCharacter.CharacterSet = s_StringState.DestCharset;

			error.UnsupportedCharacter.Position.Word = s_StringState.WordReadIndex - 1;
			error.UnsupportedCharacter.Position.Character = s_StringState.CharacterIndex;

			s_StringState.DataPtr->Characters = s_StringState.CharacterIndex;
			s_StringState.DataPtr->WordsRead = s_StringState.WordReadIndex;
			s_StringState.DataPtr->WordsWritten = s_StringState.WordWriteIndex;

			s_StringState.DataPtr = nullptr;
			return error.Type = ErrorCode::UNSUPPORTED_CHARACTER;
		}

		template<typename ErrorCharset>
		static ErrorCode BufferOverflowError(ErrorCharset* begin, ErrorCharset* end)
		{
			Error& error = Internal::GetError();
			error.BufferOverflow.BufferSize = (end - begin) * sizeof(ErrorCharset);
			error.BufferOverflow.RequiredSize = error.BufferOverflow.BufferSize + GetCharsetInfo(GetCharsetCode<ErrorCharset>::Code).WordSize;

			s_StringState.DataPtr->Characters = s_StringState.CharacterIndex;
			s_StringState.DataPtr->WordsRead = s_StringState.WordReadIndex;
			s_StringState.DataPtr->WordsWritten = s_StringState.WordWriteIndex;

			s_StringState.DataPtr = nullptr;
			return error.Type = ErrorCode::BUFFER_OVERFLOW;
		}

		template<typename ErrorCharset>
		static ErrorCode BufferUnderflowError(const ErrorCharset* begin, const ErrorCharset* end)
		{
			Error& error = Internal::GetError();
			error.BufferUnderflow.BufferSize = s_StringState.WordReadIndex * sizeof(ErrorCharset);
			error.BufferUnderflow.RequiredSize = error.BufferUnderflow.BufferSize + sizeof(ErrorCharset);

			s_StringState.DataPtr->Characters = s_StringState.CharacterIndex;
			s_StringState.DataPtr->WordsRead = s_StringState.WordReadIndex;
			s_StringState.DataPtr->WordsWritten = s_StringState.WordWriteIndex;

			s_StringState.DataPtr = nullptr;
			return error.Type = ErrorCode::BUFFER_UNDERFLOW;
		}

		//Places the next character in s_StringState.CurrentChar, or returns an error if one happened
		template<typename CharsetType, typename DestPtr = const CharsetType*>
		static ErrorCode ReadCharacter(CharsetType& c, DestPtr& src, DestPtr srcEnd, bool flip = false)
		{
		}

		template<typename CharsetType, typename DestPtr = CharsetType*>
		static ErrorCode WriteWord(CharsetType c, DestPtr& dest, DestPtr destEnd)
		{
			if (dest == nullptr) return ErrorCode::NONE;
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

		//Implement core to/from UTF32 functions

		// ========== UTF-8 ==========


		template<>
		ErrorCode ConvertStringImpl<utf32, utf8>(const utf32* src, std::size_t srcLength, utf8* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const utf32* srcEnd = src + srcLength;
			utf8* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				std::size_t surrogateCount = 0;
				utf32 c;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				s8 bits = Internal::MaxBitPlace(c);
				if (bits == 0 || bits <= 7)
				{
					//ASCII
					utf8 utf8Char = static_cast<utf8>(c);
					if (code = WriteWord(utf8Char, dest, destEnd)) return code;
				}
				else
				{
					utf8 byte;
					//Fix bits to specific size based on the utf8 template
					if (bits <= 11)
					{
						byte = (0b110 << 5) | (c >> 6);
						bits = 6;//6 more bits to encode
					}
					else if (bits <= 16)
					{
						byte = (0b1110 << 4) | (c >> 12);
						bits = 12;//12 more bits to encode
					}
					else if (bits <= 21)
					{
						byte = (0b11110 << 3) | (c >> 18);
						bits = 18;//18 more bits to encode
					}
					else
					{
						errorFormatter.Write("Character is outside the allowed range for a unicode codepoint");
						return InvalidCharacterError(c);
					}
					//Write the first byte
					if (code = WriteWord(byte, dest, destEnd)) return code;
					//Encode the remaining 6-18 bits
					while (bits > 0)
					{
						byte = static_cast<utf8>((c >> (bits - 6)) & 0b00111111);
						byte |= 0b10000000;
						if (code = WriteWord(byte, dest, destEnd)) return code;

						bits -= 6;
					}
					ES_ASSERT(bits == 0, "utf32 codepoint was not fully encoded!");
				}
				
				characters++;
			}
			return ErrorCode::NONE;

		}

		// ========== UTF-16 ==========

		template<>
		ErrorCode ConvertStringImpl<utf32, utf16>(const utf32* src, std::size_t srcLength, utf16* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const utf32* srcEnd = src + srcLength;
			utf16* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				utf32 c;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				if (c < 0xD800)
				{
					//Single byte
					if (code = WriteWord(static_cast<utf16>(c), dest, destEnd)) return code;
				}
				else
				{
					utf16 W1 = 0xD800 | static_cast<utf16>(c >> 10);
					utf16 W2 = 0xDC00 | static_cast<utf16>(c & 0b1111111111);
					if (code = WriteWord(W1, dest, destEnd)) return code;
					if (code = WriteWord(W2, dest, destEnd)) return code;

				}
				
				characters++;

			}
			return ErrorCode::NONE;

		}

		template<>
		ErrorCode ConvertStringImpl<utf16, utf32>(const utf16* src, std::size_t srcLength, utf32* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const utf16* srcEnd = src + srcLength;
			utf32* destEnd = dest + destLength;
			while (src != srcEnd)
			{
			}
			return ErrorCode::NONE;

		}


		// ========== ESC-4 ==========


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
					if (error = WriteWord(result, dest, destEnd)) return error;
				}
				characters++;
				
			}

			//Write the last byte if there is an odd number of characters
			if (characters % 2 == 1)
				if (error = WriteWord(result, dest, destEnd)) return error;
			
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode ConvertStringImpl<esc4, utf32>(const esc4* src, std::size_t srcLength, utf32* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const esc4* srcEnd = src + srcLength;
			utf32* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				esc4 c;
				utf32 codepoint;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				
				
				if (code = WriteWord(codepoint, dest, destEnd)) return code;
				characters++;
			}
			return ErrorCode::NONE;
		}



		// ========== ESC-6 ==========

		template<>
		ErrorCode ConvertStringImpl<utf32, esc6>(const utf32* src, std::size_t srcLength, esc6* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			u32 part;
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
					if (error = WriteWord(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
					if (error = WriteWord(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
					if (error = WriteWord(static_cast<esc6>((part >>  0) & 0xFF), dest, destEnd)) return error;
				}
				characters++;
			}

			//Write the remaining characters if the string wasnt a mutiple of 4
			if (characters % 4 == 1)//Top byte
			{
				//Bytes 18-24 have data
				if (error = WriteWord(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
			}
			else if (characters % 4 == 2)
			{
				//Bytes 12-24 have data
				if (error = WriteWord(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
				if (error = WriteWord(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
			}
			else if (characters % 4 == 3)
			{
				//Bytes 6-24 have data
				if (error = WriteWord(static_cast<esc6>((part >> 16) & 0xFF), dest, destEnd)) return error;
				if (error = WriteWord(static_cast<esc6>((part >>  8) & 0xFF), dest, destEnd)) return error;
				if (error = WriteWord(static_cast<esc6>((part >>  0) & 0xFF), dest, destEnd)) return error;
			}
			
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode ConvertStringImpl<esc6, utf32>(const esc6* src, std::size_t srcLength, utf32* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const esc6* srcEnd = src + srcLength;
			utf32* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				esc6 c;
				utf32 codepoint;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				
				if (code = WriteWord(codepoint, dest, destEnd)) return code;
				characters++;
			}
			return ErrorCode::NONE;

		}



		// ========== ESC-8+ ==========

		template<>
		ErrorCode ConvertStringImpl<utf32, esc8>(const utf32* src, std::size_t srcLength, esc8* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode error;
			
			const utf32* srcEnd = src + srcLength;
			esc8* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				utf32 codepoint;
				if (error = ReadCharacter(codepoint, src, srcEnd)) return error;
				if (error = WriteWord(static_cast<esc8>(codepoint), dest, destEnd)) return error;
				characters++;
			}
			
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode ConvertStringImpl<esc8, utf32>(const esc8* src, std::size_t srcLength, utf32* dest, std::size_t destLength, std::size_t& characters, Formatter& errorFormatter)
		{
			ErrorCode code;

			//Constants to make byte/length calculations easier
			const esc8* srcEnd = src + srcLength;
			utf32* destEnd = dest + destLength;
			while (src != srcEnd)
			{
				esc8 c;
				if (code = ReadCharacter(c, src, srcEnd)) return code;
				
				utf32 codepoint = c.Value;
				if (code = WriteWord(codepoint, dest, destEnd)) return code;
				characters++;
			}
			return ErrorCode::NONE;
		}


		//Explicitally instantiate src->utf32->dest conversions

		//utf32 can store any codepoint the smaller encodings can store in 1 word
		template<> std::size_t RequiredCapacity <utf8, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf16, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc4, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc6, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc8, utf32>(std::size_t words) { return words; }

		//1 word in utf32 can take more words in utr8/utf16 for the worst case
		template<> std::size_t RequiredCapacity<utf32,  utf8>(std::size_t words) { return words * 4; }
		template<> std::size_t RequiredCapacity<utf32, utf16>(std::size_t words) { return words * 2; }

		//If esc4/esc6 can't store things they cant support. So compilant utf32 strings -> esc4/esc6 will always be 1-1
		template<> std::size_t RequiredCapacity<utf32, esc4>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf32, esc6>(std::size_t words) { return words; }

		//esc8 uses VLE which can be bloated to up to 3 words per utf32 codepoint if it has a high unicode value
		template<> std::size_t RequiredCapacity<utf32, esc8>(std::size_t words) { return words * 3; }

		//Converting a type to itself is always 1-1
		template<> std::size_t RequiredCapacity<utf8,   utf8>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf16, utf16>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf32, utf32>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc4,   esc4>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc6,   esc6>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc8,   esc8>(size_t words) { return words; }

	}
}
