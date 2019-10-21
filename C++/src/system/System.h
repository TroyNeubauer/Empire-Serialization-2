#pragma once

#include "../primitive/EmpirePrimitives.h"

#include <stdio.h>

namespace Empire {
	class System {
	public:

		static u64 PageSize();
		static u64 PerformanceCounterResulution();

		static void GetLastErrorMessage(char* buf, u32 capacity);


	};
}
