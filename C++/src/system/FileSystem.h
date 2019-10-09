#pragma once

#include "../util/StringUtils.h"
#include "../EmpireException.h"

#include <functional>
#include <utility>


namespace Hazel {

	enum FileOpenOptions
	{
		READ =			0b0001,
		WRITE =			0b0010,
		APPEND =		0b0100,
		RANDOM_ACCESS =	0b1000,
		READ_WRITE = READ | WRITE,
	};

	class FileSystem
	{
	public:
		static const uint64_t ENTIRE_FILE = (uint64_t)-1;
		//Maps a section of a file into virtual memory
		//This function returns the pointer to the memory and the length of the file
		static void* MapFile(const char* file, FileOpenOptions options, uint64_t& fileLength EMPIRE_ERROR_PARAMETER, uint64_t offset = 0, uint64_t bytes = ENTIRE_FILE);
		static void UnmapFile(void* file);

		//The following functions return true if their operation succeeded and false otherwise.
		static bool Exists(const char* path);
		static bool IsDirectory(const char* path);

		//Creates a new file with the name and location specified by path
		//The return value indicates weather or not a new file was created. 
		//Therefore asking CreateFile to create a file that already exists will return false indicating that the operation failed since no file was actually created. 
		//if the requesite parent directories dont exist, CreateFile will return false and no further action will be taken
		static bool CreateFile(const char* path);

		//Creates a new file with the name and location specified by path
		//The difference between this function and CreateFile is that CreateFileWithParents will create any non existant parent directories before trying to create the file
		static bool CreateFileWithParents(const char* path);

		//CreateDirectory and CreateDirectories are the directory versions of CreateFile and CreateFileWithParents
		//They function identically except these functions create directories instead
		static bool CreateDirectory(const char* path);
		static bool CreateDirectories(const char* path);

		//Truncates a file to have a length of 0. 
		//If the file doesnt exist, this method creates a new empty file. 
		//Returns true if the specified file is now a length of 0
		static bool TruncateFile(const char* path);

		//Copies the absloute path of file into buf
		static void AbsloutePath(const char* file, char* buf, size_t bufLength);

		//Deletes a file or directory
		//If path points to a directory that contains files, each file will be removed before the directory is deleted
		//If Delete returns false (indicating that the operation failed), some child files may have been removed even if removing their parent directory failed
		static bool Delete(const char* path);

		static void NormalizePath(const char* src, char* dest, uint64_t destSize);

		//Calls onPath() for each component in path with the name of the file or directory. This path must be normalized. ../ results in undefined behavior
		//The boolean return value in the lambda is used to determine weather to continue or not. True means break and iterate no more, false means continue
		//This is marked always inline because onPath will almost always be a lambda, therefore inlining this function will usually
		//lead the compiler to inline the lambda too
		template<typename F>
		inline static void PathNameIterator(const char* path, F onPath);
		template<typename F>
		inline static void PathNameIterator(char* path, F onPath);

		//Tests if the extension of fileName equals extension. For example IsExtension("test.png", "png"); would return true
		static bool IsExtension(const char* fileName, const char* extension);

		static inline bool IsSlash(char c) { return (c == '/' || c == '\\'); }

		static inline char NormalizeSlash(char c) { if (IsSlash(c)) { return GetSlashCharacter(); } else { return c; } }

		static inline char GetSlashCharacter()
		{
#ifdef EMPIRE_PLATFORM_WINDOWS
			return '\\';
#else
			return '/';
#endif
		}

		static inline bool HasArchiveExtension(const char* fileName) { return StringUtils::ContainsAny(fileName, ".zip", ".pax", ".cpio", ".tar", "mtree", "ar"); }
	};

	template<typename F>
	void FileSystem::PathNameIterator(const char* path, F onPath)
	{
		size_t bytes = StringUtils::Capacity(path);
		char* mutablePath = _malloca(bytes);
		memcpy(mutablePath, path, bytes);

		PathNameIterator<F>(mutablePath, onPath);
		_freea(mutablePath);
	}

	template<typename F>
	void FileSystem::PathNameIterator(char* path, F onPath)
	{
		if (*path == 0x00)
			return;//Return for empty strings
		if (IsSlash(*path))
			path++;
		char* start = path;//Skip the first slash and have start point to the first char of this file/dir name
		char* end = path;//Begin searching for the end starting at the first character of this file/dir name
		while (*end)
		{
			StringUtils::Until(end, IsSlash);
			if (*end == 0x00) break;
			if (start != end)//We found somthing
			{
				char origional = *end;//Put in a temporary null byte so they we dont have to copy it
				*end = 0x00;
				if (onPath(start, origional ? (end + 1) : end)) {
					*end = origional;
					return;
				}
				*end = origional;
			}
			start = ++end;//Have the next start be the char after this trailing slash
		}
		if (start != end)
			onPath(start, end);
	}


}

