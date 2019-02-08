#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <malloc.h>
	#define EMPIRE_MIN_STACK_SIZE 30000// Its usually around 25K so 30K should be safe
#else
	#error Not Implemented!
#endif


namespace Empire {

__forceinline u64 GetStackSpace() {
#ifdef EMPIRE_PLATFORM_WINDOWS
	MEMORY_BASIC_INFORMATION mbi;
	// get range
	VirtualQuery((PVOID) &mbi, &mbi, sizeof(mbi));
	// subtract from top (stack grows downward on win)
	u64 length = (UINT_PTR) &mbi - (UINT_PTR) mbi.AllocationBase;
	if (length < EMPIRE_MIN_STACK_SIZE) {
		return 0;
	} else {
		return length - EMPIRE_MIN_STACK_SIZE;
	}
#endif
}


__forceinline void* AllocateTempBuffer(u64 bytes) {
	u64 maxSize = GetStackSpace();
	if (maxSize == 0) {
		return nullptr;
	} else {
		return _alloca(bytes);
	}
}

}//namespace

