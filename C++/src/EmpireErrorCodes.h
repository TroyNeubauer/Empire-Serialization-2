#pragma once

#include <string>

#include "primitive/EmpirePrimitives.h"


#define EMPIRE_NO_ERROR				 0
#define EMPIRE_INVALID_ARGUMENT		 1
#define EMPIRE_MISMATCHED_TYPE		 2
#define EMPIRE_UNKNOWN_TYPE			 3
#define EMPIRE_INVALID_TYPE_DEF		 4
#define EMPIRE_INVALID_FLAGS		 5
#define EMPIRE_ALREADY_KNOWN_TYPE	 6

#define EMPIRE_BUFFER_OVERFLOW		10

#define EMPIRE_INVALID_CHARACTER 30
#define EMPIRE_PARSE_ERROR_OVERFLOW	31

#define EMPIRE_NO_ERROR_TEXT "No Error"
#define EMPIRE_INVALID_ARGUMENT_TEXT "Invalid Argument"
#define EMPIRE_MISMATCHED_TYPE_TEXT "Mismatched Type"
#define EMPIRE_UNKNOWN_TYPE_TEXT "Unknown Type"
#define EMPIRE_INVALID_TYPE_DEF_TEXT "Invalid Type"
#define EMPIRE_INVALID_FLAGS_TEXT "Invalid Flags"
#define EMPIRE_ALREADY_KNOWN_TYPE_TEXT "Cannot override pre-existing type"

#define EMPIRE_BUFFER_OVERFLOW_TEXT "Buffer Overflow"

#define EMPIRE_INVALID_CHARACTER_TEXT "Invalid Character"
#define EMPIRE_PARSE_ERROR_OVERFLOW_TEXT "Overflow while parsing. Too many digets!"

namespace Empire {

struct EmpireErrorInfo {
	virtual std::string ToString() = 0;
};

struct EmpireError {
	//Initalizes an error object to hold no error
	EmpireError();
	EmpireError(ErrorCode code, EmpireErrorInfo* errorInfo) : code(code), errorInfo(errorInfo) {}
	inline operator bool() { return code; }

	ErrorCode code;
	EmpireErrorInfo* errorInfo;

};

void clearError(EmpireError& error);

std::exception ErrorCodeToException(ErrorCode code);
std::exception ErrorToException(EmpireError& code);

std::string ErrorCodeToString(ErrorCode code);
std::string ErrorToString(EmpireError& error);

struct InvalidArgumentErrorData : EmpireErrorInfo {
	std::string argumentName;

	virtual std::string ToString() override;
};

struct MismatchedTypeErrorData : EmpireErrorInfo {
	EmpireType expected, encoded;
	u64 offset;

	MismatchedTypeErrorData(EmpireType expected, EmpireType encoded, u64 offset) :
		expected(expected), encoded(encoded), offset(offset) {}

	virtual std::string ToString() override;
};

struct UnknownTypeErrorData : EmpireErrorInfo {
	EmpireType type;

	UnknownTypeErrorData(EmpireType type) : type(type) {}

	virtual std::string ToString() override;
};

struct InvalidTypeDefErrorData : EmpireErrorInfo {
	//TODO

	virtual std::string ToString() override;
};

struct BufferOverflowErrorData : EmpireErrorInfo {
	BufferOverflowErrorData(u64 bytesRequested, u64 lastIndex, u64 capacity) :
		bytesRequested(bytesRequested), lastIndex(lastIndex), capacity(capacity) {}

	u64 bytesRequested;
	u64 lastIndex, capacity;

	virtual std::string ToString() override;
};

struct InvalidCharacterErrorData : EmpireErrorInfo {
	InvalidCharacterErrorData(char character, u64 index, std::string extra) : character(character), index(index), extra(extra) {}
	char character;
	u64 index;
	std::string extra;

	virtual std::string ToString() override;
};

struct ParseOverFlowData : EmpireErrorInfo {
	ParseOverFlowData(std::string parseInput, std::string info) : parseInput(parseInput), info(info) {}
	std::string parseInput, info;

	virtual std::string ToString() override;
};

struct AlreadyKnownTypeData : EmpireErrorInfo {
	AlreadyKnownTypeData(EmpireType name) : name(name) {}
	EmpireType name;

	virtual std::string ToString() override;
};


}//namespace