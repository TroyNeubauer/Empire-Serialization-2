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
	return std::string("Mismatched Type Error, Expected Type ID " + std::to_string(expected) + 
		", Encoded Type ID: " + std::to_string(encoded) + ", Offset " + std::to_string(offset));
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


EmpireError::EmpireError() {
	this->code = NO_ERROR;
	this->errorInfo = nullptr;
}

void clearError(EmpireError& error) {
	error.code = NO_ERROR;
	if (error.errorInfo != nullptr) {
		delete error.errorInfo;
		error.errorInfo = nullptr;
	}
}

#define NO_ERROR_TEXT "No Error"
#define INVALID_ARGUMENT_TEXT "Invalid Argument"
#define MISMATCHED_TYPE_TEXT "Mismatched Type"
#define UNKNOWN_TYPE_TEXT "Unknown Type"
#define INVALID_TYPE_DEF_TEXT "Invalid Type"
#define INVALID_FLAGS_TEXT "Invalid Flags"
#define ALREADY_KNOWN_TYPE_TEXT "Cannot override pre-existing type"

#define BUFFER_OVERFLOW_TEXT "Buffer Overflow"
#define ILLEGAL_OPCODE_TEXT "Illegal Opcode"
#define ILLEGAL_REFERENCE_TEXT "Illegal Reference"

std::string ErrorCodeToString(ErrorCode code) {
	switch (code) {
	case NO_ERROR:
		return NO_ERROR_TEXT;

	case INVALID_ARGUMENT:
		return INVALID_ARGUMENT_TEXT;

	case MISMATCHED_TYPE:
		return MISMATCHED_TYPE_TEXT;

	case UNKNOWN_TYPE:
		return UNKNOWN_TYPE_TEXT;

	case INVALID_TYPE_DEF:
		return INVALID_TYPE_DEF_TEXT;

	case INVALID_FLAGS:
		return INVALID_FLAGS_TEXT;

	case ALREADY_KNOWN_TYPE:
		return ALREADY_KNOWN_TYPE_TEXT;

	case BUFFER_OVERFLOW:
		return BUFFER_OVERFLOW_TEXT;

	case ILLEGAL_OPCODE:
		return ILLEGAL_OPCODE_TEXT;

	case ILLEGAL_REFERENCE:
		return ILLEGAL_REFERENCE_TEXT;

	default:
		return "Unknown Error";
	}
}

std::string ErrorToString(EmpireError& error) {
	return std::string("Error Code ") + std::to_string(error.code) + ": " + ((error.errorInfo == nullptr) ? "No Error Info" : error.errorInfo->ToString());
}


}//namesapce