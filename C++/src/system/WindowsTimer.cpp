#ifdef EMPIRE_PLATFORM_WINDOWS

#include "Timer.h"
#include "System.h"

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

}

#endif
