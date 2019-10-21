#pragma once

#include <stdint.h>
#include <stdio.h>

#include "../primitive/EmpirePrimitives.h"

#if defined(EMPIRE_PLATFORM_WINDOWS)
	#include <Windows.h>
	typedef LARGE_INTEGER TimeType;
#else
	#include <time.h>
	typedef timespec TimeType;
#endif

namespace Empire {

	class Timer {
	public:
		//Constructing a timer starts it
		inline Timer() { Start(); }

		Timer& Start();
		Timer& Stop();
		u64 Nanos();

		double Seconds()
		{
			return Nanos() / 1000000000.0;
		}
		double Millis()
		{
			return Nanos() / 1000000.0;
		}
		u64 Micros()
		{
			return Nanos() / 1000;
		}

		static inline void FormatNanos(char* buf, u64 bufSize, u64 nanos) { FormatNanos(buf, bufSize, "", nanos); }

		static inline void FormatNanos(char* buf, u64 bufSize, const char* message, u64 nanos)
		{
			if (nanos < 1000)
				snprintf(buf, bufSize, "%s %lu nanoseconds", message, nanos);
			else if (nanos < (1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f microseconds", message, (double)nanos / 1000.0);
			else if (nanos < (1000ULL * 1000ULL * 1000ULL))
				snprintf(buf, bufSize, "%s %.2f milliseconds", message, (double)nanos / 1000000.0);
			else
				snprintf(buf, bufSize, "%s %.3f seconds", message, (double)nanos / 1000000000.0);
		}

		inline Timer& Print(const char* message)
		{
			char buf[512];
			FormatNanos(buf, sizeof(buf), message, Nanos());
			puts(buf);
			return *this;
		}

	private:
		TimeType start, end;
	};

	//A timer that prints the time since construnction when the destructor is called.
	class ScopedTimer
	{
	public:
		inline ScopedTimer(const char* message = "[Scoped Timer] Time: ")
			: m_Message(message) {}

		inline ~ScopedTimer()
		{
			timer.Stop().Print(m_Message);
		}

	private:
		const char* m_Message;
		Timer timer;
	};

}
