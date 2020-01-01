#pragma once

#include "EmpireSerialization2.h"

namespace ES {
	namespace Conversions {

		ErrorCode UTF8ToUTF16	(utf8* src, size_t srcBytes, utf16* dest, size_t destBytes);
		ErrorCode UTF8ToUTF32	(utf8* src, size_t srcBytes, utf32* dest, size_t destBytes);
		ErrorCode UTF8ToESC4	(utf8* src, size_t srcBytes,  esc4* dest, size_t destBytes);
		ErrorCode UTF8ToESC6	(utf8* src, size_t srcBytes,  esc6* dest, size_t destBytes);
		ErrorCode UTF8ToESC8	(utf8* src, size_t srcBytes,  esc8* dest, size_t destBytes);

		ErrorCode UTF16ToUTF8	(utf16* src, size_t srcBytes,  utf8* dest, size_t destBytes);
		ErrorCode UTF16ToUTF32	(utf16* src, size_t srcBytes, utf32* dest, size_t destBytes);
		ErrorCode UTF16ToESC4	(utf16* src, size_t srcBytes,  esc4* dest, size_t destBytes);
		ErrorCode UTF16ToESC6	(utf16* src, size_t srcBytes,  esc6* dest, size_t destBytes);
		ErrorCode UTF16ToESC8	(utf16* src, size_t srcBytes,  esc8* dest, size_t destBytes);
		
		ErrorCode UTF32ToUTF8	(utf32* src, size_t srcBytes,  utf8* dest, size_t destBytes);
		ErrorCode UTF32ToUTF16	(utf32* src, size_t srcBytes, utf16* dest, size_t destBytes);
		ErrorCode UTF32ToESC4	(utf32* src, size_t srcBytes,  esc4* dest, size_t destBytes);
		ErrorCode UTF32ToESC6	(utf32* src, size_t srcBytes,  esc6* dest, size_t destBytes);
		ErrorCode UTF32ToESC8	(utf32* src, size_t srcBytes,  esc8* dest, size_t destBytes);
		
		ErrorCode ESC4ToUTF8	(esc4* src, size_t srcBytes,  utf8* dest, size_t destBytes);
		ErrorCode ESC4ToUTF16	(esc4* src, size_t srcBytes, utf16* dest, size_t destBytes);
		ErrorCode ESC4ToUTF32	(esc4* src, size_t srcBytes, utf32* dest, size_t destBytes);
		ErrorCode ESC4ToESC6	(esc4* src, size_t srcBytes,  esc6* dest, size_t destBytes);
		ErrorCode ESC4ToESC8	(esc4* src, size_t srcBytes,  esc8* dest, size_t destBytes);
		
		ErrorCode ESC6ToUTF8	(esc6* src, size_t srcBytes,  utf8* dest, size_t destBytes);
		ErrorCode ESC6ToUTF16	(esc6* src, size_t srcBytes, utf16* dest, size_t destBytes);
		ErrorCode ESC6ToUTF32	(esc6* src, size_t srcBytes, utf32* dest, size_t destBytes);
		ErrorCode ESC6ToESC4	(esc6* src, size_t srcBytes,  esc4* dest, size_t destBytes);
		ErrorCode ESC6ToESC8	(esc6* src, size_t srcBytes,  esc8* dest, size_t destBytes);
		
		ErrorCode ESC8ToUTF8	(esc8* src, size_t srcBytes,  utf8* dest, size_t destBytes);
		ErrorCode ESC8ToUTF16	(esc8* src, size_t srcBytes, utf16* dest, size_t destBytes);
		ErrorCode ESC8ToUTF32	(esc8* src, size_t srcBytes, utf32* dest, size_t destBytes);
		ErrorCode ESC8ToESC4	(esc8* src, size_t srcBytes,  esc4* dest, size_t destBytes);
		ErrorCode ESC8ToESC6	(esc8* src, size_t srcBytes,  esc6* dest, size_t destBytes);

	}

}
