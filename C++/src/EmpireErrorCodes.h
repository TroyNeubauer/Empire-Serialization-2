#pragma once

#include <string>

#include "primitive/EmpirePrimitives.h"


namespace Empire {
	enum ErrorCodes {
			NO_ERROR			= 0,
			INVALID_ARGUMENT, MISMATCHED_TYPE, UNKNOWN_TYPE, INVALID_TYPE_DEF, INVALID_FLAGS, ALREADY_KNOWN_TYPE, BUFFER_OVERFLOW, ILLEGAL_OPCODE, ILLEGAL_REFERENCE
	};


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


}//namespace