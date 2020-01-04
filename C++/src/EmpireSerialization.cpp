#include "EmpireSerialization2.h"

#include "Internal.h"

#include "EmpireSerialization/String.h"

#include <cstring>
#include <cstdio>
#include <cinttypes>

namespace ES {	


	const CharsetInfo& GetCharsetInfo(Charset charset)
	{
		switch(charset)
		{
			case Charset::UTF8:
			{
				static CharsetInfo UTF8Info;
				UTF8Info.Name = "UTF-8";
				UTF8Info.WordSize = sizeof(utf8);
				UTF8Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;

				return UTF8Info;
			}
			case Charset::UTF16:
			{
				static CharsetInfo UTF16Info;
				UTF16Info.Name = "UTF-16";
				UTF16Info.WordSize = sizeof(utf8);
				UTF16Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;

				return UTF16Info;
			}
			case Charset::UTF32:
			{
				static CharsetInfo UTF32Info;
				UTF32Info.Name = "UTF-8";
				UTF32Info.WordSize = sizeof(utf8);
				UTF32Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;

				return UTF32Info;
			}
			case Charset::ESC4:
			{
				static CharsetInfo ESC4Info;
				ESC4Info.Name = "ESC4";
				ESC4Info.WordSize = sizeof(esc4);
				ESC4Info.CharactersPerWord = 2.0;

				return ESC4Info;
			}
			case Charset::ESC6:
			{
				static CharsetInfo ESC6Info;
				ESC6Info.Name = "ESC6";
				ESC6Info.WordSize = sizeof(esc6);
				ESC6Info.CharactersPerWord = 4.0 / 3.0;//4 characters per every three words

				return ESC6Info;
			}
			case Charset::ESC8:
			{
				static CharsetInfo ESC8Info;
				ESC8Info.Name = "ESC8+";
				ESC8Info.WordSize = sizeof(esc8);
				ESC8Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;

				return ESC8Info;
			}
			default: ES_ABORT("Invalid charset!");
		}
	}



	const Error& GetError()
	{
		return Internal::GetError();
	}
	
	void ClearError()
	{
		Internal::GetError().Type = ErrorCode::NONE;
	}

	void SetAllocErrorHandler(AllocErrorHandler handler)
	{
		Internal::SetAllocErrorHandler(handler);
	}

	//Instantiate templates
	template struct GetCharsetCode<utf8>;
	template struct GetCharsetCode<utf16>;
	template struct GetCharsetCode<utf32>;
	template struct GetCharsetCode<esc4>;
	template struct GetCharsetCode<esc6>;
	template struct GetCharsetCode<esc8>;


}