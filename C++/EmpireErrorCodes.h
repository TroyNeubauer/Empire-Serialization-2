#pragma once

#include <exception>
#include <string>

namespace Empire {

#define EMPIRE_NO_ERROR				 0
#define EMPIRE_INVALID_ARGUMENT		 1
#define EMPIRE_MISMATCHED_TYPE		 2
#define EMPIRE_UNKNOWN_TYPE			 3
#define EMPIRE_INVALID_TYPE_DEF		 4
#define EMPIRE_INVALID_FLAGS		 5

#define EMPIRE_INVALID_CHARACTER 30
#define EMPIRE_PARSE_ERROR_OVERFLOW	31

#define EMPIRE_NO_ERROR_TEXT "No Error"
#define EMPIRE_INVALID_ARGUMENT_TEXT "Invalid Argument"
#define EMPIRE_MISMATCHED_TYPE_TEXT "Mismatched Type"
#define EMPIRE_UNKNOWN_TYPE_TEXT "Unknown Type"
#define EMPIRE_INVALID_TYPE_DEF_TEXT "Invalid Type"
#define EMPIRE_INVALID_FLAGS_TEXT "Invalid Flags"
#define EMPIRE_INVALID_CHARACTER_TEXT "Invalid Character"
#define EMPIRE_PARSE_ERROR_OVERFLOW_TEXT "Overflow while parsing. Too many digets!"


typedef int ErrorCode;
struct EmpireErrorInfo {
	virtual void x();//So that we can use dynamic cast
};

struct InvalidArgumentErrorData : EmpireErrorInfo {
	std::string argumentName;
};

struct MismatchedTypeErrorData : EmpireErrorInfo {
	//TODO
};

struct UnknownTypeErrorData : EmpireErrorInfo {
	//TODO
};

struct InvalidTypeDefErrorData : EmpireErrorInfo {
	//TODO
};

struct InvalidTypeDefErrorData : EmpireErrorInfo {
	//TODO
};

struct InvalidCharacterErrorData : EmpireErrorInfo {
	InvalidCharacterErrorData(char character, unsigned long long index, std::string extra) : character(character), index(index), extra(extra) {}
	char character;
	unsigned long long index;
	std::string extra;
};

struct ParseOverFlowData : EmpireErrorInfo {
	ParseOverFlowData(std::string parseInput, std::string info) : parseInput(parseInput), info(info) {}
	std::string parseInput, info;
};

struct EmpireError {
	//Initalizes an error object to hold no error
	EmpireError();
	EmpireError(ErrorCode code, EmpireErrorInfo* errorInfo) : code(code), errorInfo(errorInfo){}

	ErrorCode code;
	EmpireErrorInfo* errorInfo;

};

void clearError(EmpireError& error);

std::exception CodeToException(ErrorCode code);
std::string ErrorCodeToString(ErrorCode code);


}//namespace