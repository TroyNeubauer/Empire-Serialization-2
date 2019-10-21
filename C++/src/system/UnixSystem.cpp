
//#ifdef EMPIRE_PLATFORM_UNIX

#include "FileSystem.h"
#include "System.h"
#include "Timer.h"

#include "../util/StringUtils.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

namespace Empire {

	Timer& Timer::Start()
	{
		clock_gettime(CLOCK_REALTIME, &start);
		return *this;
	}

	Timer& Timer::Stop()
	{
		clock_gettime(CLOCK_REALTIME, &end);
		return *this;
	}

	uint64_t Timer::Nanos()
	{
		uint64_t seconds = end.tv_sec - start.tv_sec;
		return 1000000000ULL * seconds + end.tv_nsec - start.tv_nsec;
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
	
	
	
	bool FileSystem::Exists(const char* path)
	{
		return false;
	}

	bool FileSystem::IsDirectory(const char* path)
	{
		return false;
	}

	uint64_t FileSystem::FileSize(const char *path EMPIRE_ERROR_PARAMETER) {
		return 0;
	}


	bool FileSystem::CreateFile(const char* path) {
		return false;
	}

	bool FileSystem::CreateFileWithParents(const char* path) {
		return false;
	}

	bool FileSystem::CreateDirectory(const char* path) {
		return false;
	}

	bool FileSystem::CreateDirectories(const char* path) {
		return false;
	}

	bool FileSystem::TruncateFile(const char* path) {
		return false;
	}

	void FileSystem::AbsloutePath(const char* file, char* buf, size_t bufLength) {
		
	}

	bool FileSystem::Delete(const char* path) {
		return false;
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, uint64_t& p_FileLength EMPIRE_ERROR_PARAMETER, uint64_t p_Offset, uint64_t p_Bytes) {
		return nullptr;
	}

	void FileSystem::UnmapFile(void* file) {
		
	}

}

//#endif
