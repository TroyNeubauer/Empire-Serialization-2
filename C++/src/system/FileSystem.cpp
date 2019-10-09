#include "FileSystem.h"

#include <stdint.h>

namespace Empire {

	const uint32_t MAX_SLASHES = 128;

	void FileSystem::NormalizePath(const char* src, char* dest, uint64_t destSize)
	{
		bool valid = true;
		uint32_t index = 0;

		uint32_t slashes[MAX_SLASHES];
		uint32_t slashCount = 0;
		while (*src)
		{
			if (IsSlash(src[0]))
			{
				if (src[1] == '.' && src[2] == '.')
				{
					if (slashCount)
					{
						index = slashes[slashCount - 1];
						slashCount--;
					}
					else
					{
						dest[index++] = FileSystem::GetSlashCharacter();
						valid = false;
					}
					src += 3;
				}
				else if (src[1] == '.')
				{
					if (slashCount == 0)
					{
						dest[index++] = FileSystem::GetSlashCharacter();
						valid = false;
					}
					src += 2; // just skip \.
				}
				else
				{
					if (slashCount < MAX_SLASHES)
					{
						slashes[slashCount++] = index;
					}
					else
					{
						valid = false;
					}
					dest[index++] = FileSystem::NormalizeSlash(*src++);
				}
			}
			else
			{
				dest[index++] = FileSystem::NormalizeSlash(*src++);
			}
		}
		dest[index] = 0;
	}

	bool FileSystem::IsExtension(const char* fileName, const char* extension)
	{
		const char* lastDot = fileName;
		while (*fileName)
		{
			StringUtils::FirstOf(fileName, '.');
			fileName++;
			if (*fileName)
				lastDot = fileName;
		}
		return StringUtils::Equal(lastDot, extension);
	}

}
