#pragma once
#include "EmpireSerialization2.h"
#include <climits>

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


//Inline
#if defined(ES_COMPILER_GCC) || defined(ES_COMPILER_CLANG)
	#define ES_ALWAYS_INLINE /*__attribule(inline)*/ inline
#elif defined(ES_COMPILER_MSVC)
	#define ES_ALWAYS_INLINE inline
#else
	#error No ES_ALWAYS_INLINE
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
		ES_ALWAYS_INLINE T* AllocMemory(std::size_t length)
		{
			return static_cast<T*>(malloc(length * sizeof(T)));
		}

		ES_ALWAYS_INLINE void FreeMemory(void* mem)
		{
			free(mem);
		}

		template<typename T>
		struct TempBuffer
		{
			ES_ALWAYS_INLINE TempBuffer(std::size_t length) : Ptr(AllocMemory<T>(length)) {}
			ES_ALWAYS_INLINE ~TempBuffer() { FreeMemory(Ptr); }

			inline T& operator-> () { return Ptr; }
			inline T* Get() { return Ptr; }

			T* Ptr;
		};

		void SetAllocErrorHandler(AllocErrorHandler handler);
		void InvokeAllocErrorHandler();

		void Log(const char* message);

		namespace System {
			inline void* StackPointer()
			{
#if defined(ES_PLATFORM_UNIX)

#else
				int x;
				return &x;
#endif
			}

			template<typename T>
			inline std::size_t ByteDifference(const T* a, const T* b)
			{
				if (a > b)	return reinterpret_cast<const uint8_t*>(a) - reinterpret_cast<const uint8_t*>(b);
				else 		return reinterpret_cast<const uint8_t*>(b) - reinterpret_cast<const uint8_t*>(a); 
			}
		}

		namespace Math {
			template<typename T>
			T DivideCeli(T value, T divide) { return (value + divide - 1) / divide; }
		}

		template<std::uint64_t n, typename T>
		inline bool BottomBitsFromTopSize(T value)
		{
			constexpr std::uint64_t totalBits = sizeof(T) * CHAR_BIT;
			static_assert(totalBits >= n, "Cannot evaluate bits outside the range of T");
			constexpr std::uint64_t bottomBits = totalBits - n;
			T mask = 0;
			//Turn all the lower bits on
			for (int i = 0; i < bottomBits; i++)
			{
				mask |= (0b1 << i);
			}
			return value & mask;
		}

		template<std::uint64_t n, typename T>
		inline bool BottomBits(T value)
		{
			T mask = 0;
			//Turn all the lower bits on
			for (int i = 0; i < n; i++)
			{
				mask |= (0b1 << i);
			}
			return value & mask;
		}

		template<typename T>
		inline bool BottomBits(std::size_t n, T value)
		{
			if (std::is_signed<T>::value)
			{
				typedef typename std::make_unsigned<T>::type UnsignedType;
				//Convert to the unsigned type using memcpy instead of a cast to make the compiler happy
				UnsignedType unsignedValue;
				std::memcpy(&unsignedValue, &value, sizeof(value));
				return BottomBits<UnsignedType>(n, unsignedValue);
			}
			else
			{
				T mask = 0;
				//Turn all the lower bits on
				for (int i = 0; i < n; i++)
				{
					mask |= (0b1 << i);
				}
				return value & mask;
			}
		}

		template<std::uint64_t n, std::uint64_t bits, typename T>
		inline bool TopNBitsAre(T value)
		{
			if (std::is_signed<T>::value)
			{
				typedef typename std::make_unsigned<T>::type UnsignedType;
				//Convert to the unsigned type using memcpy instead of a cast to make the compiler happy
				UnsignedType unsignedValue;
				std::memcpy(&unsignedValue, &value, sizeof(value));
				return TopNBitsAre<n, bits, UnsignedType>(unsignedValue);
			}
			else
			{
				constexpr std::uint64_t totalBits = sizeof(T) * CHAR_BIT;
				static_assert(totalBits >= n, "Cannot evaluate bits outside the range of T");
				constexpr std::uint64_t shiftCount = totalBits - n;
				return (value >> shiftCount) == bits;

			}

		}


		
	}
}




