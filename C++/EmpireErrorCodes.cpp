#include "EmpireErrorCodes.h"

namespace Empire {

std::exception CodeToException(ErrorCode code) {
	return std::exception(ErrorCodeToString(code).c_str());
}

void EmpireErrorInfo::x() {}//Leave blank

EmpireError::EmpireError() {
	this->code = EMPIRE_NO_ERROR;
	this->errorInfo = nullptr;
}

void clearError(EmpireError& error) {
	error.code = EMPIRE_NO_ERROR;
	if (error.errorInfo != nullptr) {
		delete error.errorInfo;
	}
}

std::string ErrorCodeToString(ErrorCode code) {
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

	case EMPIRE_INVALID_CHARACTER:
		return EMPIRE_INVALID_CHARACTER_TEXT;

	case EMPIRE_PARSE_ERROR_OVERFLOW:
		return EMPIRE_PARSE_ERROR_OVERFLOW_TEXT;
	default:
		return "Unknown Error";
	}
}

}//namesapce