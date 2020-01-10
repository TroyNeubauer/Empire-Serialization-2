#include "EmpireSerialization2.h"

#include "Internal.h"

#include "EmpireSerialization/ESString.h"

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
		Internal::GetError().AdditionalMessage.Clear();
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


	std::size_t StringCodingData::NumBytesRead() const { return GetCharsetInfo(SrcCharacterSet).WordSize * WordsRead; }
	std::size_t StringCodingData::NumBytesWritten() const { return GetCharsetInfo(DestCharacterSet).WordSize * WordsWritten; }

}