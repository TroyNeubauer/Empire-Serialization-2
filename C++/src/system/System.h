#pragma once

#include "../primitive/EmpirePrimitives.h"

#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
#else
#error Not supported!
#endif

#include <stdio.h>

namespace Empire {
	class System {
	public:

		static u64 PageSize();
		static u64 PerformanceCounterResulution();

		static void GetLastErrorMessage(char* buf, unsigned int capacity);


	};
}
