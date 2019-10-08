#pragma once

#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include <stdio.h>

namespace Hazel {
	class System {
	public:

		inline static void GetLastErrorMessage(char* buf, unsigned int capacity) {
			GetLastErrorMessageImpl(buf, capacity);
		}


#ifdef EMPIRE_PLATFORM_WINDOWS
		inline static void GetLastErrorMessageImpl(char* buf, unsigned int capacity) {
			DWORD error = GetLastError();
			DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_ENGLISH, buf, capacity, NULL);
			if (!result) {
				fprintf(stderr, "Failed to get information log from error code: %i, the error from FormatMessageA was %i", error, GetLastError());
				if (capacity)
					buf[0] = 0x00;//Make sure to terminate the string
			}
		}
#else
	#error Not supported!
#endif
	};
}
