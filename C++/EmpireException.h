#pragma once

#include "EmpireErrorCodes.h"


namespace Empire {

#ifndef EMPIRE_ENABLE_EXCEPTIONS
	#define EMPIRE_ENABLE_EXCEPTIONS 1
#endif

#if EMPIRE_ENABLE_EXCEPTIONS
	#ifdef EMPIRE_DISABLE_ERROR_CODES
		#define ERROR_CODE_PARAMETER//Error code not used
		#define ERROR(code, unused) throw(CodeToException(code));
	#else
		#define ERROR_CODE_PARAMETER , ErrorCode& errorCode
		#define ERROR(code,unused)		\
			errorCode = code;			\
			throw(CodeToException(code));
	#endif
#else
	#ifdef EMPIRE_DISABLE_ERROR_CODES
		#define ERROR_CODE_PARAMETER
		#define ERROR(code,returnValue)
	#else
		#define ERROR_CODE_PARAMETER , ErrorCode& errorCode
		#define ERROR(code, returnValue)\
		errorCode = code;				\
		return returnValue;
	#endif

#endif





}//namesapce
