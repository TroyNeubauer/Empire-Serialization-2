
#ifdef EMPIRE_PLATFORM_WINDOWS

#include "FileSystem.h"
#include "System.h"
#include "Timer.h"

#include "../util/StringUtils.h"

#include <Windows.h>
#include <Shlobj.h>

namespace Empire {

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
	
	
	
	static SYSTEM_INFO info = {};
	static u64 timerResulution = 0;

	u64 System::PageSize() {
		if(info.dwPageSize == 0)
			GetSystemInfo(&info);
		return info.dwPageSize;
	}

	u64 System::PerformanceCounterResulution() {
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

	void System::GetLastErrorMessage(char* buf, u32 capacity) {
		DWORD error = GetLastError();
		DWORD result = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, LANG_ENGLISH, buf, capacity, NULL);
		if (!result) {
			fprintf(stderr, "Failed to get information log from error code: %i, the error from FormatMessageA was %i", error, GetLastError());
			if (capacity)
				buf[0] = 0x00;//Make sure to terminate the string
		}
	}
	
	

	bool FileSystem::Exists(const char* path)
	{
		DWORD atts = GetFileAttributesA(path);
		return atts != INVALID_FILE_ATTRIBUTES;
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		DWORD atts = GetFileAttributesA(path);
		return atts & FILE_ATTRIBUTE_DIRECTORY;
	}

	uint64_t FileSystem::FileSize(const char *path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);

		if (handle == INVALID_HANDLE_VALUE) return INVALID_FILE;

		LARGE_INTEGER size;
		if (!GetFileSizeEx(handle, &size))
			return INVALID_FILE;
		
		CloseHandle(handle);
		return size.QuadPart;
	}


	bool FileSystem::CreateFile(const char* path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	bool FileSystem::CreateFileWithParents(const char* path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	bool FileSystem::CreateDirectory(const char* path)
	{
		return CreateDirectoryA(path, nullptr) || FileSystem::Exists(path);
	}

	bool FileSystem::CreateDirectories(const char* path)
	{
		bool success = true;
		FileSystem::PathNameIterator(path, [&success](const char* dirName, const char* total, const char* rest) -> bool {
			if (!FileSystem::CreateDirectory(dirName)) {
				success = false;
				return true;
			}			
			return false;
		});
		return success;
	}

	bool FileSystem::TruncateFile(const char* path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_WRITE, 0, nullptr, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
				return FileSystem::CreateFile(path);//Try to create it if it didnt exist
			else
				return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	void FileSystem::AbsloutePath(const char* file, char* buf, size_t bufLength)
	{
		GetFullPathNameA(file, bufLength, buf, nullptr);
	}

	bool FileSystem::Delete(const char* path)
	{
		DWORD atts = GetFileAttributesA(path);
		if (atts & FILE_ATTRIBUTE_DIRECTORY) {

			//HANDLE h = FindFirstFileA()
		} else {
			return DeleteFileA(path);

		}
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, uint64_t& p_FileLength EMPIRE_ERROR_PARAMETER, uint64_t p_Offset, uint64_t p_Bytes)
	{
		if (p_Bytes == ENTIRE_FILE)
		{
			p_Bytes = 0;
			p_Offset = 0;
		}
		uint64_t pageSize = System::PageSize();

		DWORD flags = (p_Options & FileOpenOptions::RANDOM_ACCESS) ? FILE_FLAG_RANDOM_ACCESS : FILE_FLAG_SEQUENTIAL_SCAN;
		DWORD desiredAccess = 0;
		if (p_Options & READ) desiredAccess |= GENERIC_READ;
		if (p_Options & WRITE) desiredAccess |= GENERIC_WRITE;

		HANDLE handle = CreateFileA(p_File, desiredAccess, FILE_SHARE_READ, NULL, OPEN_EXISTING, flags, NULL);

		if (handle == INVALID_HANDLE_VALUE)
		{
			char errorMessage[1024];
			System::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			EMPIRE_ERROR(Empire::ErrorCodes::IO_ERROR, nullptr, "Error opening file. Error from CreateFileA is: %s", errorMessage);
		}

		LARGE_INTEGER size;
		//Locals for later on (so the compiler does not complain about uninitialized variables in the goto)
		void* data;
		DWORD access, protect;
		HANDLE viewHandle;

		if (!GetFileSizeEx(handle, &size))
		{
			char errorMessage[1024];
			System::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			EMPIRE_ERROR(Empire::ErrorCodes::IO_ERROR, nullptr, "Failed to get file length. Error from GetFileSizeEx is: %s", errorMessage);
			goto fail;
		}
		p_FileLength = size.QuadPart;
		if (p_FileLength == 0)
		{
			EMPIRE_ERROR(Empire::ErrorCodes::IO_ERROR, nullptr, "Cannot map a file with a length of 0 bytes");
			goto fail;
		}

		protect = 0;
		if (p_Options & WRITE) protect = PAGE_READWRITE;//There is no write only memory mapping, so if they went to write they must have read write access
		else if (p_Options & READ) protect = PAGE_READONLY;
		else//They didnt specify a valid access mode
		{
			EMPIRE_ERROR(Empire::ErrorCodes::INVALID_ARGUMENT, nullptr, "File must be opened for either reading, writing or both!");
			goto fail;
		}
		viewHandle = CreateFileMappingA(handle, nullptr, protect, (p_Bytes >> 32) & 0xFFFFFFFFULL, p_Bytes & 0xFFFFFFFFULL, nullptr);

		if (viewHandle == nullptr)
		{
			char errorMessage[1024];
			System::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			EMPIRE_ERROR(Empire::ErrorCodes::IO_ERROR, nullptr, "Failed to create file mapping. Error from CreateFileMappingA is: %s", errorMessage);
			goto fail;
		}

		access = 0;
		if (p_Options & READ) access |= FILE_MAP_READ;
		if (p_Options & WRITE) access |= FILE_MAP_WRITE;

		data = MapViewOfFile(viewHandle, access, (p_Offset & 0xFFFFFFFF00000000ULL) >> 32, (p_Offset & 0xFFFFFFFFULL), p_Bytes);
		if (data == nullptr)
		{
			char errorMessage[1024];
			System::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			EMPIRE_ERROR(Empire::ErrorCodes::IO_ERROR, nullptr, "Failed to map view of file. Error from MapViewOfFile2 is: %s", errorMessage);
			CloseHandle(viewHandle);
			goto fail;
		}
		CloseHandle(viewHandle);//Clean up the other resources
		CloseHandle(handle);
		return data;
	fail:
		CloseHandle(handle);
		return nullptr;
	}

	void FileSystem::UnmapFile(void* file)
	{
		UnmapViewOfFile(file);
	}

}

#endif
