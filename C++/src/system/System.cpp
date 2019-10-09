#include "System.h"


namespace Hazel {

#ifdef EMPIRE_PLATFORM_WINDOWS

	static SYSTEM_INFO info = {};

	u64 System::PageSize() {
		if(info.dwPageSize == 0)
			GetSystemInfo(&info);
		return info.dwPageSize;
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

