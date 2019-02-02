#include "EmpireErrorCodes.h"
#include "EmpireException.h"

namespace Empire {

std::exception CodeToException(ErrorCode code) {
	return std::exception(ErrorCodeToString(code));
}

const char* ErrorCodeToString(ErrorCode code) {
	switch (code) {
	case EMPIRE_NO_ERROR:
		return EMPIRE_NO_ERROR_TEXT;

	case EMPIRE_INVALID_ARGUMENT:
		return EMPIRE_INVALID_ARGUMENT_TEXT;

	case EMPIRE_MISMATCHED_TYPE:
		return EMPIRE_MISMATCHED_TYPE_TEXT;

	case EMPIRE_UNKNOWN_TYPE:
		return EMPIRE_UNKNOWN_TYPE_TEXT;

	case EMPIRE_INVALID_TYPE_DEF:
		return EMPIRE_INVALID_TYPE_DEF_TEXT;

	case EMPIRE_INVALID_FLAGS:
		return EMPIRE_INVALID_FLAGS_TEXT;

	case EMPIRE_STRING_INVALID_CHARACTER:
		return EMPIRE_STRING_INVALID_CHARACTER_TEXT;

	case EMPIRE_PARSE_ERROR_OVERFLOW:
		return EMPIRE_PARSE_ERROR_OVERFLOW_TEXT;
	default:
		return "Unknown Error";
	}
}

}//namesapce