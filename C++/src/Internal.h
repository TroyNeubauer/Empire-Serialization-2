#pragma once
#include "EmpireSerialization2.h"


//========== COMPILER detect ==========
#if defined(__EMSCRIPTEN__ )
	#define ES_COMPILER_EMSCRIPTEN
#elif defined(_MSC_VER)
	#define ES_COMPILER_MSVC
#elif defined(__GNUC__)
	#define ES_COMPILER_GCC
#elif defined(__clang__)
	#define ES_COMPILER_CLANG

#else
	#error Unsupported compiler
#endif

//========== PLATFORM detect ==========
#if defined(ES_COMPILER_EMSCRIPTEN)
	#define ES_PLATFORM_EMSCRIPTEN
	#include <emscripten/emscripten.h>

#elif defined(__ANDROID__)
	#define ES_PLATFORM_ANDROID
	#define ES_PLATFORM_UNIX
	#define ES_PLATFORM_LINUX
	#error Android is not supported for now!

#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define ES_PLATFORM_WINDOWS
	#define ES_LITTLE_ENDIAN

#elif defined(__APPLE__) || defined(__MACH__)

    #define ES_PLATFORM_APPLE
    #define ES_PLATFORM_UNIX


#elif defined(__linux__)//Defined after android to isolate desktop linux
	#define ES_PLATFORM_UNIX
	#define ES_PLATFORM_LINUX
	#define ES_LITTLE_ENDIAN

#elif defined(embedded_look_for)//FIXME
	#define ES_PLATFORM_EMBEDDED
	//...Checks for particular embedded processors...
	//#define ES_EMBEDDED_AVR
	//#define ES_EMBEDDED_ARM

#else
	#error Unknown platform

#endif

//========== ARCHITECTURE detect
#if defined(__x86_64__) || defined(_M_AMD64)
	#define ES_X86_64
	#define ES_X86

#elif defined(__i386__) || defined(_M_IX86)
	#define ES_X86_32
	#define ES_X86
	
#else
	#error Unknown architecture
#endif


#ifdef ES_DEBUG
		// debug break
	#if defined(ES_PLATFORM_WINDOWS)
		#define ES_DEBUGBREAK(file, line) __debugbreak()

	#elif defined(ES_PLATFORM_UNIX)
		#include <signal.h>
		#define ES_DEBUGBREAK(file, line) raise(SIGTRAP)

	#elif defined(ES_PLATFORM_EMSCRIPTEN)
		#define ES_DEBUGBREAK(file, line) EM_ASM({ alert('Assertion failed at File $0, line $1'); }, file, line);
	
	#else
		#error No debug break!

	#endif

	#define ES_ASSERT(x, message) { if(!(x)) { ::ES::Internal::Log(message); ES_DEBUGBREAK(__FILE__, __LINE__); } }

#else
	#define ES_ASSERT(x, message)
	#define ES_DEBUGBREAK(file, line)

#endif


#if defined(ES_PLATFORM_WINDOWS) || defined(ES_PLATFORM_UNIX)
	#include <cstdlib>
	#define ES_ABORT(message) { ::ES::DefaultFormatter formatter; formatter << "[Empire Serialization] Aborting: " << message << ". At file " << __FILE__ << " line " << __LINE__; ::ES::Internal::Log(formatter); abort(); }
	
#elif defined(ES_PLATFORM_EMSCRIPTEN)
	#define ES_ABORT(message) EM_ASM({ throw new Error('[Empire Serialization] Abort has been called! File $0, line $1'); }, __FILE__, __LINE__);
	
#else
	#error No AbortImpl!

#endif

#define ES_MIN(a, b) (a < b ? a : b)
#define ES_MAX(a, b) (a > b ? a : b)

#ifndef ES_MAX_STACK_USAGE
	#define ES_MAX_STACK_USAGE (256 * 1024)// 256K
#endif



namespace ES {
	namespace Internal {
		void Init();

		//Mutable GetError()
		Error& GetError();

		//Allocates an array with the number of given elements hopefully on the stack.
		//Call FreeMemory when finished.
		//This function will always return a valid pointer. If no memory can be allocated
		template<typename T>
		inline T* AllocMemory(size_t length)
		{

		}

		inline void FreeMemory(void* mem)
		{

		}

		void SetAllocErrorHandler(AllocErrorHandler handler);
		void InvokeAllocErrorHandler();

		void Log(const char* message);

		namespace System {
			inline void* StackPointer()
			{
#ifdef ES_PLATFORM_WINDOWS
#elif defined(ES_PLATFORM_UNIX)

#else

#endif
			}

		}
	}
}




