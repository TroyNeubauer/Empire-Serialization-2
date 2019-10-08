#pragma once

#include "../primitive/EmpirePrimitivesConfig.h"

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
		template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
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


}
