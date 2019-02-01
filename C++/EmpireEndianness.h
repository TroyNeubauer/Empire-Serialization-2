#pragma once

#include <stdint.h>

namespace Empire {

	enum Endianness {
		BIG = 0, LITTLE = 1
	};

	volatile uint32_t i = 0x01234567;
	// return 0 for big endian, 1 for little endian.
	const Endianness SYSTEM_ENDIANESS = (Endianness)((*((uint8_t*)(&i))) == 0x67);

}//namespace
