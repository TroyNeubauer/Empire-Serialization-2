#include "EmpireSerialization2.h"
#include "EmpireSerialization/Conversions.h"
#include "Internal.h"

#include "Charsets.h"

namespace ES {
	namespace Conversions {

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

		template<typename SrcType, typename DestType>
		ErrorCode Convert(const SrcType* src, size_t srcBytes, DestType* dest, size_t destCapacity, StringCodingData& data)
		{
			static_assert(!std::is_same<SrcType, DestType>::value, "Types must be different!");
			static_assert(Exists<SrcType, DestType>::value, "Invalid conversion!");
			//String conversion
			if (std::is_same<SrcType, utf8>::value || std::is_same<SrcType, utf16>::value || std::is_same<SrcType, utf32>::value 
				|| std::is_same<SrcType, esc4>::value || std::is_same<SrcType, esc6>::value || std::is_same<SrcType, esc8>::value)
			{
				static_assert(std::is_same<DestType, utf8>::value || std::is_same<DestType, utf16>::value || std::is_same<DestType, utf32>::value 
					|| std::is_same<DestType, esc4>::value || std::is_same<DestType, esc6>::value || std::is_same<DestType, esc8>::value);
				std::size_t tempCapacity = RequiredCapacity<SrcType, utf32>(srcBytes);
				Internal::TempBuffer<utf32> temp(tempCapacity);

				StringCodingData tempData;
				ErrorCode error = ErrorCode::NONE;

				//Perform the conversion returning errors to the caller
				if (error = Convert<SrcType, utf32>(src, srcBytes, temp.Get(), tempCapacity, tempData)) return error;
				if (error = Convert<utf32, DestType>(temp.Get(), tempData.Bytes, dest, destCapacity, data)) return error;
				
				return ErrorCode::NONE;
			}
		}


		//Implement core to/from UTF32 functions
		template<>
		ErrorCode Convert<utf8, utf32>(const utf8* src, size_t srcBytes, utf32* dest, size_t destCapacity, StringCodingData& data)
		{
			data.Characters = 0;
			data.Bytes = 0;
			for (std::size_t& i = data.Characters; i < srcBytes; i++)
			{
				char c = src[i];
				if (c >= 128)
				{
					Error& error = Internal::GetError();
					error.InvalidCharacter.Char = c;
					error.InvalidCharacter.CharacterSet = Charset::UTF32;
					error.InvalidCharacter.Position = data;
					return error.Type = INVALID_CHARACTER;
				}
				else
				{
					if (data.Bytes > destCapacity - sizeof(utf32))
					{
						Error& error = Internal::GetError();
						error.BufferOverflow.BufferSize = destCapacity;
						error.BufferOverflow.RequiredSize = destCapacity + sizeof(utf32);
						return error.Type = BUFFER_OVERFLOW;
					}
					dest[data.Bytes / sizeof(utf32)] = c;
					data.Bytes += sizeof(utf32);
				}
			}
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode Convert<utf32, esc4>(const utf32* src, size_t srcBytes, esc4* dest, size_t destCapacity, StringCodingData& data)
		{
			uint8_t part;
			bool start = true;
			
			data.Characters = 0;
			data.Bytes = 0;
			for (std::size_t& i = data.Characters; i < srcBytes / sizeof(utf32); i++)
			{ 
				utf32 codepoint = src[i];
				int8_t encoded;
				if (codepoint >= ESC4_ENCODE.size() || (encoded = ESC4_ENCODE[codepoint]) == -1)
				{
					Error& error = Internal::GetError();
					error.InvalidCharacter.Char = codepoint;
					error.InvalidCharacter.CharacterSet = Charset::ESC4;
					error.InvalidCharacter.Position = data;
					return error.Type = INVALID_CHARACTER;
				}
				if (start)
				{
					part = encoded << 4;
				}
				else
				{
					part |= encoded;
					dest[data.Bytes++].Value = part & 0xFF;
				}
				start = !start;
				
			}
			//Write the last character if there is an odd number of characters
			if ((srcBytes / sizeof(utf32)) % 2)
				dest[data.Bytes++].Value = part & 0xFF;
			
			return ErrorCode::NONE;
		}

		template<>
		ErrorCode Convert<utf32, esc6>(const utf32* src, size_t srcBytes, esc6* dest, size_t destCapacity, StringCodingData& data)
		{
			uint32_t part;
			uint8_t state = 0;
			
			data.Characters = 0;
			data.Bytes = 0;
			for (std::size_t& i = data.Characters; i < srcBytes / sizeof(utf32); i++)
			{
				utf32 codepoint = src[i];
				int8_t encoded;
				if (codepoint >= ESC6_ENCODE.size() || (encoded = ESC6_ENCODE[codepoint]) == -1)
				{
					Error& error = Internal::GetError();
					error.InvalidCharacter.Char = codepoint;
					error.InvalidCharacter.CharacterSet = Charset::ESC6;
					error.InvalidCharacter.Position = data;
					return error.Type = INVALID_CHARACTER;
				}
				if (state == 0)
				{
					part = encoded << 18;//top 6 bits of 24 bit word
					state++;
				}
				else if (state == 1)
				{
					part |= encoded << 12;
					state++;
				}
				else if (state == 2)
				{
					part |= encoded << 6;
					state++;
				}
				else if (state == 3)
				{
					part |= encoded << 6;
					//Write all three bytes
					dest[data.Bytes++].Value = (part >> 16) & 0xFF;
					dest[data.Bytes++].Value = (part >>  8) & 0xFF;
					dest[data.Bytes++].Value = (part >>  0) & 0xFF;

					state = 0;
				}
				
			}

			//Write the remaining characters if the string wasnt a mutiple of 4
			if (state == 1)//Top byte
			{//Bytes 18-24 have data
				dest[data.Bytes++].Value = (part >> 16) & 0xFF;
			}
			else if (state == 2)
			{//Bytes 12-24 have data
				dest[data.Bytes++].Value = (part >> 16) & 0xFF;
				dest[data.Bytes++].Value = (part >>  8) & 0xFF;
				
			}
			else if (state == 3)
			{//Bytes 6-24 have data
				dest[data.Bytes++].Value = (part >> 16) & 0xFF;
				dest[data.Bytes++].Value = (part >>  8) & 0xFF;
				dest[data.Bytes++].Value = (part >>  0) & 0xFF;
			}
			
			return ErrorCode::NONE;
		}



		template ErrorCode Convert<utf8, esc4>(const utf8*, size_t, esc4*, size_t, StringCodingData&);
		template ErrorCode Convert<utf8, esc6>(const utf8*, size_t, esc6*, size_t, StringCodingData&);

	}
}
