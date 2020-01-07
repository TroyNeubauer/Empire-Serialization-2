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

//Include intrinsic headers
#if defined(ES_X86_64) || defined(ES_X86)
	#include <immintrin.h>

#else
	#Add new case and intrinsics header here!

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
				if (a > b)	return reinterpret_cast<const u8*>(a) - reinterpret_cast<const u8*>(b);
				else 		return reinterpret_cast<const u8*>(b) - reinterpret_cast<const u8*>(a); 
			}
		}

		namespace Math {
			template<typename T>
			T DivideCeli(T value, T divide) { return (value + divide - 1) / divide; }
		}

		template<u8 n, typename T, typename UnsignedType = typename std::make_unsigned<T>::type>
		inline UnsignedType BottomBitsFromTopSize(T value)
		{
			static_assert(n <= sizeof(T) * CHAR_BIT, "Cannot evaluate bits outside the range of T");
			return BottomBits(sizeof(T) * CHAR_BIT - n, value);
		}

		template<typename T, typename UnsignedType = typename std::make_unsigned<T>::type>
		inline UnsignedType BottomBits(T value, u8 n)
		{
			//Convert to the unsigned type using memcpy instead of a cast to make the compiler happy
			UnsignedType unsignedValue;
			memcpy(&unsignedValue, &value, sizeof(T));

			//If they want the entire value dont do bit shifting
			if (n == sizeof(T) * CHAR_BIT)
			{
				return unsignedValue;
			}
			ES_ASSERT(n >= 0, "N cannot be negitive");

			UnsignedType mask = (static_cast<UnsignedType>(1) << n) - 1;
			return unsignedValue & mask;
		}

		template<u8 n, typename T, typename UnsignedType = typename std::make_unsigned<T>::type>
		inline UnsignedType BottomBits(T value)
		{
			return BottomBits(value, n);
		}

		template<u8 n, typename T, typename UnsignedType = typename std::make_unsigned<T>::type>
		inline UnsignedType TopNBits(T value)
		{
			constexpr u64 totalBits = sizeof(T) * CHAR_BIT;
			static_assert(n <= sizeof(T) * CHAR_BIT, "Cannot evaluate bits outside the range of T");

			UnsignedType unsignedValue;
			std::memcpy(&unsignedValue, &value, sizeof(T));
			return unsignedValue >> (totalBits - n);
		}

		template<u8 n, std::size_t expecting, typename T>
		bool TopNBitsAre(T value)
		{
			return TopNBits<n, T>(value) == expecting;
		}

		template<typename T, class = std::enable_if<std::is_unsigned<T>::value>>
		s8 MaxBitPlace(T value)
		{
			for (int i = sizeof(T) * CHAR_BIT - 1; i >= 0; i++)
			{
				if ((value >> i) & 0x01) return i;
			}
			return -1;
		}

#if defined(ES_COMPILER_GCC) || defined(ES_COMPILER_CLANG)
//TODO
/*		template<> s8 MaxBitPlace(u32 value) { return _bit_scan_reverse(value); }
		template<> s8 MaxBitPlace(u16 value) { return _bit_scan_reverse(value); }
		template<> s8 MaxBitPlace( u8 value) { return _bit_scan_reverse(value); }
*/
#elif defined(ES_COMPILER_MSVC)
		template<> s8 MaxBitPlace(u64 value) { unsigned long result; if (_BitScanReverse64(&result, value)) return result; else return -1; }
		template<> s8 MaxBitPlace(u32 value) { unsigned long result; if (_BitScanReverse  (&result, value)) return result; else return -1; }
		template<> s8 MaxBitPlace(u16 value) { unsigned long result; if (_BitScanReverse  (&result, value)) return result; else return -1; }
		template<> s8 MaxBitPlace( u8 value) { unsigned long result; if (_BitScanReverse  (&result, value)) return result; else return -1; }

#else

	#warning No special implementation for this compiler
#endif


	}
}

