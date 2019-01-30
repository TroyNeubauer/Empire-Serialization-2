#include "EmpireErrorCodes.h"
#include "EmpireException.h"

namespace Empire {

std::exception CodeToException(ErrorCode code) {
	return std::exception(CodeToString(code));
}

const char* CodeToString(ErrorCode code) {
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
	default:
		return "Unknown Error";
	}
}

}//namesapce