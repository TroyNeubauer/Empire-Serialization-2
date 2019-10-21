
#ifdef EMPIRE_PLATFORM_UNIX

#include "Timer.h"
#include "System.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

namespace Empire {
	Timer::Timer()
	{
		Start();
	}

	Timer& Timer::Start()
	{
		QueryPerformanceCounter(&start);
		return *this;
	}

	Timer& Timer::Stop()
	{
		QueryPerformanceCounter(&end);
		return *this;
	}

	u64 Timer::Nanos()
	{
		return (end.QuadPart - start.QuadPart) * 1000000000ULL / System::PerformanceCounterResulution();
	}
	
	
	
	u64 System::PageSize() {
		return getpagesize();
	}

	u64 System::PerformanceCounterResulution() {
		return CLOCKS_PER_SEC;
	}

	void System::GetLastErrorMessage(char* buf, u32 capacity) {
		char* error = strerror(errno);
		StringUtils::Copy(error, buf, capacity);
	}

}

#endif
