#include "EmpireErrorCodes.h"

namespace Empire {

std::exception CodeToException(ErrorCode code) {
	return std::exception(ErrorCodeToString(code).c_str());
}

std::string InvalidArgumentErrorData::ToString() {
	return "Invalid Argument Error, Argument Name: " + argumentName;
};

std::string MismatchedTypeErrorData::ToString() {
	return "MismatchedTypeErrorData TODO";
};

std::string UnknownTypeErrorData::ToString() {
	return "UnknownTypeErrorData TODO";
};

std::string InvalidTypeDefErrorData::ToString() {
	return "InvalidTypeDefErrorData TODO";
};


std::string InvalidCharacterErrorData::ToString() {
	return "Invalid Character Error, Character ID " + std::to_string((int) character) + " \'" + character + "\'" + 
		", Index: " + std::to_string(index) + " - " + extra;
};

std::string ParseOverFlowData::ToString() {
	return "ParseOver Flow, Parse Input: " + parseInput + " - " + info;
};



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