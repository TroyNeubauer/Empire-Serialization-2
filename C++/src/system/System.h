#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
#else
#error Not supported!
#endif

#include <stdio.h>

namespace Hazel {
	class System {
	public:

		static u64 PageSize();

		static void GetLastErrorMessage(char* buf, unsigned int capacity);


	};
}
