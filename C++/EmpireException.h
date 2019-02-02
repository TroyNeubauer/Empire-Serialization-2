#pragma once


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
		#define ERROR_CODE_PARAMETER , EmpireError& errorCode
		#define ERROR_CODE_VAR , errorCode
		#define ERROR(code,unused)				\
			errorCode = EmpireError(code, info);\
			throw(CodeToException(code))// No ; here on purpose to force the caller to include one
	#endif
#else
	#if EMPIRE_DISABLE_ERROR_CODES
		#define ERROR_CODE_PARAMETER
		#define ERROR_CODE_VAR
		#define ERROR(code,returnValue) return returnValue
	#else
		#define ERROR_CODE_PARAMETER , EmpireError& errorCode
		#define ERROR_CODE_VAR , errorCode
		#define ERROR(code, info, returnValue)		\
			errorCode = EmpireError(code, info);	\
			return returnValue// No ; here on purpose to force the caller to include one
	#endif

#endif





}//namesapce
