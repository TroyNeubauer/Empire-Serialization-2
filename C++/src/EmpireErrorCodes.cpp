#include "EmpireErrorCodes.h"

namespace Empire {

std::exception ErrorCodeToException(ErrorCode code) {
	return std::exception(ErrorCodeToString(code).c_str());
}

std::exception ErrorToException(EmpireError& error) {
	return std::exception(error.errorInfo->ToString().c_str());
}

std::string InvalidArgumentErrorData::ToString() {
	return std::string("Invalid Argument, Argument Name: " + argumentName);
};

std::string BufferOverflowErrorData::ToString() {
	return std::string("Buffer Overflow, Bytes Requested: " + std::to_string(bytesRequested) + 
		", Accessing Index: " + std::to_string(lastIndex) + ", Buffer Capacity: " + std::to_string(capacity));
}


std::string MismatchedTypeErrorData::ToString() {
	return std::string("Mismatched Type Error TODO");
};

std::string UnknownTypeErrorData::ToString() {
	return std::string("Unknown Type Error, Type ID: " + std::to_string(type));
};

std::string InvalidTypeDefErrorData::ToString() {
	return std::string("Invalid Type Def Error TODO");
};


std::string InvalidCharacterErrorData::ToString() {
	return std::string("Invalid Character Error, Character ID ") + std::to_string((int) character) + " \'" + character + "\'" +
		", Index: " + std::to_string(index) + " - " + extra;
};

std::string ParseOverFlowData::ToString() {
	return std::string("ParseOver Flow, Parse Input: ") + parseInput + " - " + info;
};

std::string AlreadyKnownTypeData::ToString() {
	return  std::string("Already Known Type, ID: ")  + std::to_string(name);
};



EmpireError::EmpireError() {
	this->code = EMPIRE_NO_ERROR;
	this->errorInfo = nullptr;
}

void clearError(EmpireError& error) {
	error.code = EMPIRE_NO_ERROR;
	if (error.errorInfo != nullptr) {
		delete error.errorInfo;
		error.errorInfo = nullptr;
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

	case EMPIRE_ALREADY_KNOWN_TYPE:
		return EMPIRE_ALREADY_KNOWN_TYPE_TEXT;

	case EMPIRE_BUFFER_OVERFLOW:
		return EMPIRE_BUFFER_OVERFLOW_TEXT;
	default:
		return "Unknown Error";
	}
}

std::string ErrorToString(EmpireError& error) {
	return std::string("Error Code ") + std::to_string(error.code) + ": " + ((error.errorInfo == nullptr) ? "No Error Info" : error.errorInfo->ToString());
}


}//namesapce