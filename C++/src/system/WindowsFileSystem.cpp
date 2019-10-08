#ifdef EMPIRE_PLATFORM_WINDOWS

#include "FileSystem.h"
#include <Windows.h>

#undef CreateFile
#undef CreateDirectory
#undef DeleteFile

namespace Hazel {

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
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
	}

	bool FileSystem::CreateDirectories(const char* path)
	{
		HANDLE handle = CreateFileA(path, GENERIC_READ, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(handle);//A bit wasteful. Looking into assigning the handle to a path might be more efficent since Paths are usually opened anyway
		return true;
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
		return false;
	}


	//Use of p_ to denote parameters from local vars in this long function
	void* FileSystem::MapFile(const char* p_File, FileOpenOptions p_Options, uint64_t& p_FileLength, FileError* p_Error, uint64_t p_Offset, uint64_t p_Bytes)
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
			if (p_Error) {
				DWORD lastError = GetLastError();
				switch (lastError) {
				case ERROR_FILE_NOT_FOUND:
				case ERROR_PATH_NOT_FOUND:
					*p_Error = FileError::FILE_NOT_FOUND;
					break;
				case ERROR_ACCESS_DENIED:
					*p_Error = FileError::ACCESS_DENIED;
					break;
				case ERROR_TOO_MANY_OPEN_FILES:
					*p_Error = FileError::TOO_MANY_FILES;
					break;
				default:
					*p_Error = FileError::UNKNOWN;
					break;
				}
			}
			return nullptr;
		}
#ifdef HZ_DEBUG
		const char* foAccess;
		if ((p_Options & READ) && p_Options & WRITE) foAccess = "rw";
		else if (p_Options & READ) foAccess = "r";
		else if (p_Options & WRITE) foAccess = "w";
		else foAccess = "";
		Log_fopen((FILE*)handle, p_File, foAccess, __FILE__, __LINE__);
#elif HZ_RELEASE
		Log_fopen((FILE*)handle, p_File);
#endif
		LARGE_INTEGER size;
		if (!GetFileSizeEx(handle, &size))
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			//HZ_CORE_ERROR("Failed to get file length for file \"{0}\", error from GetFileSizeEx is: {1}", path, errorMessage);
			if (p_Error)
				*p_Error = FileError::UNKNOWN;
			goto fail;
		}
		p_FileLength = size.QuadPart;
		if (p_FileLength == 0)
		{
			HZ_CORE_ASSERT(false, "Unable to memory map file of length 0!");
		}

		DWORD protect = 0;
		if (p_Options & WRITE) protect = PAGE_READWRITE;//There is no write only memory mapping, so if they went to write they must have read write access
		else if (p_Options & READ) protect = PAGE_READONLY;
		else//They didnt specify a valid access mode
		{
			if (p_Error)
				* p_Error = FileError::INVALID_PARAMETER;
			goto fail;
		}
		HANDLE viewHandle = CreateFileMappingA(handle, nullptr, protect, (p_Bytes >> 32) & 0xFFFFFFFFULL, p_Bytes & 0xFFFFFFFFULL, nullptr);

		if (viewHandle == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			HZ_CORE_ERROR("Failed to create file mapping for file: \"{0}\", error from CreateFileMappingA is: {1}", p_File, errorMessage);
			if (p_Error)
				*p_Error = FileError::UNKNOWN;
			goto fail;
		}

		DWORD access = 0;
		if (p_Options & READ) access |= FILE_MAP_READ;
		if (p_Options & WRITE) access |= FILE_MAP_WRITE;

		void* data = MapViewOfFile(viewHandle, access, (p_Offset & 0xFFFFFFFF00000000ULL) >> 32, (p_Offset & 0xFFFFFFFFULL), p_Bytes);
		if (data == nullptr)
		{
			char errorMessage[1024];
			WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
			HZ_CORE_ERROR("Failed to map view of file: \"{0}\", error from MapViewOfFile2 is: {1}", p_File, errorMessage);
			if (p_Error)
				* p_Error = FileError::UNKNOWN;//FIXME: report proper errors
			CloseHandle(viewHandle);
			goto fail;
		}
		CloseHandle(viewHandle);//Clean up the other resources
		CloseHandle(handle);
		return data;
	fail:
		CloseHandle(handle);
#ifdef HZ_DEBUG
		Log_fclose((FILE*)handle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
		Log_fclose((FILE*)handle, 0);
#endif
		return nullptr;
		/*
		//Try to reserve another page for the null terminator to avoid copying the data on ToString()
		if ((options & READ_ONLY) && !(options & WRITE_ONLY)) {
			void* neededAddress = (char*)data + length;
			if (length % pageSize == 0)//There is no null byte since the string takes up an entire page
			{
				void* otherPage = VirtualAlloc(neededAddress, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READONLY);
				if (neededAddress != otherPage)
				{
					if (otherPage == nullptr) {
						char errorMessage[1024];
						WindowsUtils::GetLastErrorMessage(errorMessage, sizeof(errorMessage));
						HZ_CORE_ERROR("Failed to allocate page for null terminator: error from VirtualAlloc is: {0}", errorMessage);
					}
					//The page couldn't be allocated where it needed to be
					//So allocate a buffer big enough, release all the file handles, and then give the user the buffer
					char* newData = (char*)malloc(length + 1);
					memcpy(newData, data, length);
					newData[length] = 0x00;//Ensure there is a null byte at the end

					CloseHandle(handle);//We no longer need the file, all the data is safely in newData
					UnmapViewOfFile(data);
				}
			}
		}*/
	}

	void FileSystem::UnmapFile(void* file)
	{
		UnmapViewOfFile(file);
	}

	/*File::File(File&& other) : m_Path(other.GetPath())
	{
		other.m_Length = this->m_Length;
		other.m_FileHandle = this->m_FileHandle;
		other.m_Data = this->m_Data;
		other.m_FreeData = this->m_FreeData;

		this->m_FileHandle = INVALID_FILE_HANDLE;
		this->m_Data = nullptr;
	}

	File::~File()
	{
		if (!m_CompletedDeInit)
		{
			m_CompletedDeInit = true;
#ifdef HZ_DEBUG
			Log_fclose((FILE*)m_FileHandle, 0, __FILE__, __LINE__);
#elif HZ_RELEASE
			Log_fclose((FILE*)m_FileHandle, 0);
#endif
			if (m_FreeData)
			{	//In this case m_Data is the block we malloc'ed
				//We only need to free m_Data because the file handle and view were destroyed in the constructor
				free(m_Data);
			}
			else
			{
				CloseHandle(m_FileHandle);
				UnmapViewOfFile(m_Data);
				if (m_OtherPage)//We allocated another page to get space for the null byte
				{
					VirtualFree(m_OtherPage, 0, MEM_RELEASE);
				}
			}
		}

	}*/

}

#endif