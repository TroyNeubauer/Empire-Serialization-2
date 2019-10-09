#pragma once

#include <string>
#include <stdexcept>

#include "primitive/EmpirePrimitives.h"


namespace Empire {
	enum ErrorCodes {
			NO_ERROR = 0,
			INVALID_ARGUMENT, MISMATCHED_TYPE, UNKNOWN_TYPE, INVALID_TYPE_DEF, INVALID_FLAGS, ALREADY_KNOWN_TYPE, BUFFER_OVERFLOW, BUFFER_UNDERFLOW,
			ILLEGAL_OPCODE, ILLEGAL_REFERENCE, IO_ERROR
	};

	struct EmpireError {
		//Initalizes an error object to hold no error
		EmpireError() : Code(NO_ERROR) { ErrorInfo[0] = 0x00; }

		template<typename... Args>
		EmpireError(ErrorCode code, const char* format, Args... args) : Code(code) {
			snprintf(ErrorInfo, sizeof(ErrorInfo), format, args...);
		}

		inline operator bool() { return Code; }

		ErrorCode Code;
		char ErrorInfo[512];

	};

	inline std::exception ErrorToException(EmpireError& error) {
		return std::runtime_error(std::string(error.ErrorInfo));
	}

	inline const char* ErrorCodeToString(ErrorCode code) {
		switch (code) {
			default:					return "Unknown Error Code";
			case NO_ERROR:				return "No Error";
			case INVALID_ARGUMENT:		return "Invalid Argument";
			case MISMATCHED_TYPE:		return "Mismatched Type";
			case UNKNOWN_TYPE:			return "Unknown Type";
			case INVALID_TYPE_DEF:		return "Invalid Type";
			case INVALID_FLAGS:			return "Invalid Flags";
			case ALREADY_KNOWN_TYPE:	return "Cannot override pre-existing type";
			case BUFFER_OVERFLOW:		return "Buffer Overflow";
			case BUFFER_UNDERFLOW:		return "Buffer Underflow";
			case ILLEGAL_OPCODE:		return "Illegal Opcode";
			case ILLEGAL_REFERENCE:		return "Illegal Reference";
			case IO_ERROR:				return "I/O Error";
		}
	}



}//namespace