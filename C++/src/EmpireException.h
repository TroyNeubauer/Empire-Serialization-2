#pragma once

#include "EmpireErrorCodes.h"
#include "primitive/EmpirePrimitives.h"


//==========================================
//Tempoary Build Properties!
#define EMPIRE_ENABLE_EXCEPTIONS 0
#define EMPIRE_DISABLE_ERROR_CODES 0
//==========================================

#ifndef EMPIRE_ENABLE_EXCEPTIONS
	#define EMPIRE_ENABLE_EXCEPTIONS 1
#endif

#define EMPIRE_VOID_FUNCTION //Define it as nothing because we want an empty return; statement


#if EMPIRE_ENABLE_EXCEPTIONS
	#include <exception>

	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_IF_ERROR_CODES(code)
		#define EMPIRE_ERROR_PARAMETER//Error code not used
		#define EMPIRE_ERROR_VAR
		#define EMPIRE_ERROR(code, info, unused) throw( ErrorToException(EmpireError(code, info)) )
	#else
		#define EMPIRE_IF_ERROR_CODES(code) , code
		#define EMPIRE_ERROR_PARAMETER , EmpireError& errorCode
		#define EMPIRE_ERROR_VAR , errorCode
		#define EMPIRE_ERROR(code,info,unused)	\
			errorCode = EmpireError(code, info);\
			throw( ErrorToException(errorCode) )
	#endif
#else
	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_IF_ERROR_CODES(code)
		#define EMPIRE_ERROR_PARAMETER
		#define EMPIRE_ERROR_VAR
		#define EMPIRE_ERROR(code,info,returnValue) return returnValue
	#else
		#define EMPIRE_IF_ERROR_CODES(code) , code
		#define EMPIRE_ERROR_PARAMETER , EmpireError& empireError
		#define EMPIRE_ERROR_VAR , empireError
		#define EMPIRE_ERROR(code, info, returnValue)	\
			empireError = EmpireError(code, info);		\
			return returnValue// No ; here on purpose to force the caller to include one
	#endif

#endif

