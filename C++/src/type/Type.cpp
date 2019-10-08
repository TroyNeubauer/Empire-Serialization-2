#include "Type.h"

namespace Empire {



	const Type& BuiltinTypes::Get(u64 id)
	{
		switch (id) {
			default:
			case INVALID_ID: return INVALID;
			case S8_ID: return S8;
			case U8_ID: return U8;
			case S16_ID: return U16;
			case U16_ID: return U16;
			case S32_ID: return S32;
			case U32_ID: return U32;
			case S64_ID: return S64;
			case U64_ID: return U64;
			case S128_ID: return S128;
			case U128_ID: return U128;
			case S256_ID: return S256;
			case U256_ID: return U256;
			case BIG_INTEGER_ID: return BIG_INTEGER;
			
			case F8_ID: return F8;
			case F16_ID: return F16;
			case F32_ID: return F32;
			case F64_ID: return F64;
			case F128_ID: return F128;
			case BIG_FLOAT_ID: return BIG_FLOAT;

			case UTF8_STRING_ID: return UTF8_STRING;
			case UTF16_STRING_ID: return UTF16_STRING;
			case UTF32_STRING_ID: return UTF32_STRING;
			case ESC4_STRING_ID: return ESC4_STRING;
			case ESC6_STRING_ID: return ESC6_STRING;
			case ESC8_PLUS_STRING_ID: return ESC8_PLUS_STRING;
		}
	}

}