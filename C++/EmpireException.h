#pragma once

#include "EmpireErrorCodes.h"

//==========================================
//Tempoary Build Properties!
#define EMPIRE_ENABLE_EXCEPTIONS 0
#define EMPIRE_DISABLE_ERROR_CODES 0
//==========================================


namespace Empire {

#ifndef EMPIRE_ENABLE_EXCEPTIONS
	#define EMPIRE_ENABLE_EXCEPTIONS 1
#endif

#define EMPIRE_VOID_FUNCTION //Define it as nothing because we want an empty return; statement

#if EMPIRE_ENABLE_EXCEPTIONS
	#if EMPIRE_DISABLE_ERROR_CODES
		#define ERROR_CODE_PARAMETER//Error code not used
		#define ERROR_CODE_VAR
		#define ERROR(code, unused) throw(CodeToException(code))
	#else
		#define ERROR_CODE_PARAMETER , ErrorCode& errorCode
		#define ERROR_CODE_VAR , errorCode
		#define ERROR(code,unused)		\
			errorCode = code;			\
			throw(CodeToException(code))
	#endif
#else
	#if EMPIRE_DISABLE_ERROR_CODES
		#define ERROR_CODE_PARAMETER
		#define ERROR_CODE_VAR
		#define ERROR(code,returnValue)
	#else
		#define ERROR_CODE_PARAMETER , ErrorCode& errorCode
		#define ERROR_CODE_VAR , errorCode
		#define ERROR(code, returnValue)\
		errorCode = code;				\
		return returnValue
	#endif

#endif





}//namesapce
