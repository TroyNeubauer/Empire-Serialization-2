#include "System.h"


namespace Empire {

#ifdef EMPIRE_PLATFORM_WINDOWS

	static SYSTEM_INFO info = {};
	static u64 timerResulution = 0;

	u64 System::PageSize() {
		if(info.dwPageSize == 0)
			GetSystemInfo(&info);
		return info.dwPageSize;
	}

	u64 System::PerformanceCounterResulution()
	{
		if (timerResulution == 0) {
			LARGE_INTEGER result;
			if (QueryPerformanceFrequency(&result) == 0) {
				char buf[1024];
				GetLastErrorMessage(buf, sizeof(buf));
				printf("QueryPerformanceFrequency Returned 0! Error: %s\n", buf);
			}
			timerResulution = result.QuadPart;
		}
		return timerResulution;
	}

	void System::GetLastErrorMessage(char* buf, unsigned int capacity) {
		DWORD error = GetLastError();
		DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_ENGLISH, buf, capacity, NULL);
		if (!result) {
			fprintf(stderr, "Failed to get information log from error code: %i, the error from FormatMessageA was %i", error, GetLastError());
			if (capacity)
				buf[0] = 0x00;//Make sure to terminate the string
		}
	}



#endif


}

