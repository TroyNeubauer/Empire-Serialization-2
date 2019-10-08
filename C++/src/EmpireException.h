#pragma once

#include "EmpireErrorCodes.h"
#include "primitive/EmpirePrimitives.h"


//==========================================
//Tempoary Build Properties!
#define EMPIRE_ENABLE_EXCEPTIONS 1
#define EMPIRE_DISABLE_ERROR_CODES 0
//==========================================

#ifndef EMPIRE_ENABLE_EXCEPTIONS
	#define EMPIRE_ENABLE_EXCEPTIONS 1
#endif

#define EMPIRE_VOID_FUNCTION //Define it as nothing because we want an empty return; statement

#define CONCAT(a, b) a ## b

#define CREATE_EMPIRE_ERROR(code, format, ...) EmpireError(code, CONCAT("Empire Error: %u (%s): ", format), code, ErrorCodeToString(code), __VA_ARGS__)

#if EMPIRE_ENABLE_EXCEPTIONS
	#include <exception>


	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_IF_ERROR_CODES(code)
		#define EMPIRE_ERROR_PARAMETER1
		#define EMPIRE_ERROR_PARAMETER
		#define EMPIRE_ERROR_VAR1
		#define EMPIRE_ERROR_VAR
		#define EMPIRE_ERROR(code, returnValue, format, ...) throw( ErrorToException(CREATE_EMPIRE_ERROR(code, format, __VA_ARGS__) )
	#else
		#define EMPIRE_IF_ERROR_CODES(code) , code
		#define EMPIRE_ERROR_PARAMETER1 EmpireError& empireError
		#define EMPIRE_ERROR_PARAMETER , EMPIRE_ERROR_PARAMETER1
		#define EMPIRE_ERROR_VAR1 empireError
		#define EMPIRE_ERROR_VAR , EMPIRE_ERROR_VAR1
		#define EMPIRE_ERROR(code, returnValue, format, ...)	\
			empireError = CREATE_EMPIRE_ERROR(code, format, __VA_ARGS__);\
			throw( ErrorToException(empireError) )
	#endif
#else
	#if EMPIRE_DISABLE_ERROR_CODES
		#define EMPIRE_IF_ERROR_CODES(code)
		#define EMPIRE_ERROR_PARAMETER1
		#define EMPIRE_ERROR_PARAMETER
		#define EMPIRE_ERROR_VAR1
		#define EMPIRE_ERROR_VAR
		#define EMPIRE_ERROR(code, returnValue, format, ...) return returnValue
	#else
		#define EMPIRE_IF_ERROR_CODES(code) , code
		#define EMPIRE_ERROR_PARAMETER1 EmpireError& empireError
		#define EMPIRE_ERROR_PARAMETER , EMPIRE_ERROR_PARAMETER1
		#define EMPIRE_ERROR_VAR1 empireError
		#define EMPIRE_ERROR_VAR , EMPIRE_ERROR_VAR1
		#define EMPIRE_ERROR(code, returnValue, format, ...)		\
			empireError = CREATE_EMPIRE_ERROR(code, format, __VA_ARGS__);	\
			return returnValue// No ; here on purpose to force the caller to include one
	#endif

#endif

