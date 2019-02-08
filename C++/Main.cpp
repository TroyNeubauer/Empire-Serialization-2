#include <iostream>
#include <time.h>
#include <Windows.h>

#include "Empire.h"
#include "Test.h"
#include "half.h"

#include "TempBuffer.h"


#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <malloc.h>
	#define EMPIRE_MIN_STACK_SIZE 30000// Its usually around 25K so 30K should be safe
#else
	#error Not Implemented!
#endif


namespace Empire {

inline void* AllocateTempBuffer(u64 bytes) {
	u64 maxSize = GetStackSpace();
	if (maxSize == 0) {
		return nullptr;
	} else {
		return _alloca(bytes);
	}
}

inline u64 GetStackSpace() {
#ifdef EMPIRE_PLATFORM_WINDOWS
	MEMORY_BASIC_INFORMATION mbi;
	// get range
	VirtualQuery((PVOID)&mbi, &mbi, sizeof(mbi));
	// subtract from top (stack grows downward on win)
	u64 length = (UINT_PTR)&mbi - (UINT_PTR)mbi.AllocationBase;
	if (length < EMPIRE_MIN_STACK_SIZE) {
		return 0;
	} else {
		return length - EMPIRE_MIN_STACK_SIZE;
	}
#endif

}

}

u64 x(u64 a) {
	char* buffer = (char*) Empire::AllocateTempBuffer(128);
	a = ((u64*) buffer)[10];
	return (u64) buffer + x(a);
}


using namespace Empire;


//Full length string - fills 128 bits
//ABCDEFABCDEFABCD1122334455667788
int main() {
	u64 c = x(0);
	return 0;
	float array[] = { 0.0f, 292.23f, 2.2323f, 13.2387f, 278.2f, 27.5f, -1.24f, -45.46f, -0.4578134f, 0.0000001f};
	const int elements = sizeof(array) / sizeof(float);
	half result[elements];

	FloatsToHalves(result, array, elements);
	for (int i = 0; i < elements; i++) {
		half h = result[i];
		for (int j = 8 * sizeof(half) - 1; j >= 0; j--) {
			std::cout << ((h.value >> j) & 0x01);
		}
		std::cout << std::endl;
	}//0001110111
	float dest[elements];
	HalvesToFloats(dest, result, elements);
	


	system("PAUSE");
}
