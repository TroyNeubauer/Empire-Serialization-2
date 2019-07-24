#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <malloc.h>
#else
	#error Not Implemented!
#endif


namespace Empire {


__forceinline void* AllocateTempBuffer(u64 bytes) {
	return _malloca(bytes);

}

}//namespace

