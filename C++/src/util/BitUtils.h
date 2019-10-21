#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_ENABLE_INTEL_INTRINSICS
	#ifdef EMPIRE_COMPILER_MSVC
		#include <intrin.h>
	#endif
#endif

#if !defined(EMPIRE_COMPILER_MSVC) && !defined(EMPIRE_COMPILER_GCC)
	#error Compiler not supported!
#endif

namespace Empire {

	namespace BitUtils {
		template <typename T>
		static T NextPowerOfTwo(T value) {
			value--;
			for (int i = 1; i < CHAR_BIT * sizeof(T); i *= 2) {//Bleed all the 1 bits togther
				value |= value >> i;
			}
			value++;//Increment to cause all to carry
			return value;
		}

		//Standard implementation
		template<typename T>
		static u32 HighestBitPosition(T value) {
			u32 result = 0;
			while (value >>= 1) result++;
			return result;
		}

		//template<>
		static u32 HighestBitPosition(u32 value) {
			u32 result;
#ifdef EMPIRE_COMPILER_MSVC
			_BitScanReverse(&result, value);
#elif defined(EMPIRE_COMPILER_GCC)
			result = __builtin_ctz(value);
#endif
			return result;
		}

		//template<>
		static u32 HighestBitPosition(u64 value) {
			unsigned long result;
#ifdef EMPIRE_COMPILER_MSVC
			_BitScanReverse64(&result, value);
#elif defined(EMPIRE_COMPILER_GCC)
			result = __builtin_ctzll(value);
#endif
			return result;
		}
	};

	template <typename T>
	static T SwapEndianness(T value) {
		T result;
		u8* dest = reinterpret_cast<u8*>(&result);
		u8* start = reinterpret_cast<u8*>(&value);
		for (int i = 0; i < sizeof(T); i++) {
			dest[i] = start[sizeof(T) - i - 1];
		}
	}


#if defined(EMPIRE_COMPILER_MSVC)

	static u16 SwapEndianness(u16 value) { return _byteswap_ushort(value); }
	static u32 SwapEndianness(u32 value) { return _byteswap_ulong(value); }
	static u64 SwapEndianness(u64 value) { return _byteswap_uint64(value); }

#elif defined(EMPIRE_COMPILER_GCC)

	static u16 SwapEndianness(u16 value) { return __builtin_bswap16(value); }
	static u32 SwapEndianness(u32 value) { return __builtin_bswap32(value); }
	static u64 SwapEndianness(u64 value) { return __builtin_bswap64(value); }

#endif

	static s16 SwapEndianness(s16 value) { return SwapEndianness<u16>(value); }
	static s32 SwapEndianness(s32 value) { return SwapEndianness<u32>(value); }
	static s64 SwapEndianness(s64 value) { return SwapEndianness<u64>(value); }



}
