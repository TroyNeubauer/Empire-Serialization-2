#pragma once

#include "EmpireErrorCodes.h"


//==========================================
//Tempoary Build Properties!
#define EMPIRE_ENABLE_EXCEPTIONS 1
#define EMPIRE_DISABLE_ERROR_CODES 1
//==========================================


#ifndef EMPIRE_ENABLE_EXCEPTIONS
	#define EMPIRE_ENABLE_EXCEPTIONS 1
#endif

#define EMPIRE_VOID_FUNCTION //Define it as nothing because we want an empty return; statement


#if EMPIRE_ENABLE_EXCEPTIONS
	#include <exception>

	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_ERROR_CODE_PARAMETER//Error code not used
		#define EMPIRE_ERROR_CODE_VAR
		#define EMPIRE_ERROR(code, info, unused) throw(std::exception((info)->ToString().c_str()))
	#else
		#define EMPIRE_ERROR_CODE_PARAMETER , EmpireError& errorCode
		#define EMPIRE_ERROR_CODE_VAR , errorCode
		#define EMPIRE_ERROR(code,info,unused)	\
			errorCode = EmpireError(code, info);\
			throw(info.ToString())// No ; here on purpose to force the caller to include one
	#endif
#else
	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_ERROR_CODE_PARAMETER
		#define EMPIRE_ERROR_CODE_VAR
		#define EMPIRE_ERROR(code,info,returnValue) return returnValue
	#else
		#define EMPIRE_ERROR_CODE_PARAMETER , EmpireError& errorCode
		#define EMPIRE_ERROR_CODE_VAR , errorCode
		#define EMPIRE_ERROR(code, info, returnValue)	\
			errorCode = EmpireError(code, info);		\
			return returnValue// No ; here on purpose to force the caller to include one
	#endif

#endif

