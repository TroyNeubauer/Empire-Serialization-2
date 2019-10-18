#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_ENABLE_INTEL_INTRINSICS
	#include <intrin.h>
#endif

namespace Empire {

	class BitUtils {
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

		template<>
		static u32 HighestBitPosition(u32 value) {
			unsigned long result;
			_BitScanReverse(&result, value);
			return result;
		}

		template<>
		static u32 HighestBitPosition(u64 value) {
			unsigned long result;
			_BitScanReverse64(&result, value);
			return result;
		}
	};

	template <typename T>
	static T SwapEndianness(T value) {
		T result;
		u8* dest = static_cast<u8*>(&result);
		u8* start = static_cast<u8*>(&value);
		for (int i = 0; i < sizeof(T); i++) {
			dest[i] = start[sizeof(T) - i - 1];
		}
	}

	template<> static u16 SwapEndianness(u16 value) { return _byteswap_ushort(value); }
	template<> static u32 SwapEndianness(u32 value) { return _byteswap_ulong(value); }
	template<> static u64 SwapEndianness(u64 value) { return _byteswap_uint64(value); }

	template<> static s16 SwapEndianness(s16 value) { return SwapEndianness<u16>(value); }
	template<> static s32 SwapEndianness(s32 value) { return SwapEndianness<u32>(value); }
	template<> static s64 SwapEndianness(s64 value) { return SwapEndianness<u64>(value); }




}