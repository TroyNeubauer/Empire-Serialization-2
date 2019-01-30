#pragma once

#include <exception>

namespace Empire {

#define EMPIRE_NO_ERROR			0
#define EMPIRE_INVALID_ARGUMENT 1
#define EMPIRE_MISMATCHED_TYPE	2
#define EMPIRE_UNKNOWN_TYPE		3
#define EMPIRE_INVALID_TYPE_DEF	4
#define EMPIRE_INVALID_FLAGS	5

#define EMPIRE_NO_ERROR_TEXT "No Error"
#define EMPIRE_INVALID_ARGUMENT_TEXT "Invalid Argument"
#define EMPIRE_MISMATCHED_TYPE_TEXT "Mismatched Type"
#define EMPIRE_UNKNOWN_TYPE_TEXT "Unknown Type"
#define EMPIRE_INVALID_TYPE_DEF_TEXT "Invalid Type"
#define EMPIRE_INVALID_FLAGS_TEXT "Invalid Flags"

typedef int ErrorCode;

std::exception CodeToException(ErrorCode code);
const char* CodeToString(ErrorCode code);


}//namespace