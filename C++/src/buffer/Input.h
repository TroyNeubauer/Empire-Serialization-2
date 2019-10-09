#pragma once

#include "Buffer.h"
#include "Output.h"
#include "../EmpireException.h"
#include "../system/System.h"
#include "../system/FileSystem.h"

namespace Empire {

class BufferedInput : public Buffer {
public:
	BufferedInput() {}

	BufferedInput(u64 capacity) : Buffer(capacity), m_Limit(capacity) {}

	BufferedInput(u8* buffer, u64 capacity) :
		Buffer(buffer, capacity), m_Limit(capacity) {}

	BufferedInput(BufferedOutput& out) :
		Buffer(out.m_Buffer, out.m_Capacity), m_Limit(out.m_Offset) {}


	void Init(u64 capacity) {
		Buffer::Init(capacity);
		this->m_Limit = 0;//We are creating a new buffer so there we begin with 0 bytes being readable
	}
	void Init(u8* buffer, u64 capacity) {
		Buffer::Init(buffer, capacity);
		this->m_Limit = capacity;
	}

	/*
	* Returns true if there is enough space in the buffer to read x bytes, false if a buffer overflow will occur by reading x bytes
	*/
	inline bool EnsureCapacity(u64 bytes EMPIRE_ERROR_PARAMETER) {
		if (bytes + m_Offset > m_Limit) {
			EMPIRE_ERROR(BUFFER_OVERFLOW, false, "Limit: %llu, Size Needed: %llu, Chunk Size: %llu", m_Limit, bytes + m_Offset, bytes);
		}
		return true;
	}

	inline bool CanRead() {
		return m_Offset < m_Limit;
	}

	inline u64 BytesAvilable() {
		return m_Limit - m_Offset;
	}

	template<typename T>
	T ReadVLE(EMPIRE_ERROR_PARAMETER1) {
		T result = 0;
		u32 shift = 0;
		u8 byte;
		do {
			if (!EnsureCapacity(1 EMPIRE_ERROR_VAR)) return 0;
			byte = m_Buffer[m_Offset++];
			u8 data = byte & VLE_DATA_MASK;
			result |= static_cast<T>(data) << shift;
			shift += 7;
		} while (byte & VLE_STATUS_MASK);


		return result;
	}
	template<typename T>
	T Read(EMPIRE_ERROR_PARAMETER1) {
		EnsureCapacity(sizeof(T) EMPIRE_ERROR_VAR);
		T result;
		ReadRaw(&result, sizeof(T));
		return result;
	}

	template<typename T>
	void Read(T* dest, u64 bytes EMPIRE_ERROR_PARAMETER) {
		EnsureCapacity(bytes EMPIRE_ERROR_VAR);
		ReadRaw(dest, bytes);
	}

private:
	template<typename T>
	inline void ReadRaw(T* dest, u64 bytes) {
		std::memcpy(dest, Pointer(), bytes);
		m_Offset += bytes;
	}


protected:
	u64 m_Limit;

	friend class BufferedOutput;
	friend class FileInput;
};


enum FileOptions {
	NONE = 0, NORMAL = 1, MAPPED = 2, SEE_CHANGES = 4
};

enum class InputMode {
	NORMAL, MAPPED
};

class FileInput {
public:

	FileInput(std::string path EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : FileInput(path.c_str() EMPIRE_ERROR_VAR, options) {}

	FileInput(const char* path EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : FileInput(path, 0, (std::numeric_limits<u64>::max)() EMPIRE_ERROR_VAR, options) {}
	
	FileInput(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : m_Options(options) {
		m_Input.Init(GetInitalCapacity());
		if (options & FileOptions::MAPPED) OpenFileAsMapped(path, offset, length EMPIRE_ERROR_VAR);
		else OpenFileNormally(path, offset, length EMPIRE_ERROR_VAR);
	}

private:
	u64 GetInitalCapacity() { return 1024 * 1024; }

	void OpenFileAsMapped(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER) {
		u8* buf = reinterpret_cast<u8*>(FileSystem::MapFile(path, FileOpenOptions::READ, m_FileSize EMPIRE_ERROR_VAR));
		m_Input.Init(buf, m_FileSize);
		m_Mode = InputMode::MAPPED;
	}

	void OpenFileNormally(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER) {
		m_FileSize = FileSystem::FileSize(path EMPIRE_ERROR_VAR);
		m_FP = fopen(path, "rb");
		if (m_FP == nullptr)
			EMPIRE_ERROR(IO_ERROR, , "File not found");
		m_Mode = InputMode::NORMAL;
	}

	void CloseFile() {
		if (m_Mode == InputMode::NORMAL) {
			fclose(m_FP);
			m_FP = nullptr;
		} else if (m_Mode == InputMode::MAPPED) {
			FileSystem::UnmapFile(m_Input.Start());
		}
	}

public:

	~FileInput() {
		CloseFile();
	}

	template<typename T>
	T ReadVLE(EMPIRE_ERROR_PARAMETER1) {
		T result = 0;
		u32 shift = 0;
		u8 byte;
		do {
			Read(&byte, 1 EMPIRE_ERROR_VAR);
			u8 data = byte & VLE_DATA_MASK;
			result |= static_cast<T>(data) << shift;
			shift += 7;
		} while (byte & VLE_STATUS_MASK);


		return result;
	}
	template<typename T>
	T Read(EMPIRE_ERROR_PARAMETER1) {
		T result;
		Read(&result, sizeof(T) EMPIRE_ERROR_VAR);
		return result;
	}

	template<typename T>
	void Read(T* dest, u64 bytes EMPIRE_ERROR_PARAMETER) {
		u64 bytesRead = 0;
		if (m_Input.CanRead()) {
			bytesRead = min(bytes, m_Input.BytesAvilable());
			m_Input.Read(dest, bytesRead EMPIRE_ERROR_VAR);
			bytes -= bytesRead;
			m_TotalRead += bytesRead;
		} else if (m_Mode == InputMode::MAPPED) {//If we cant read in mapped mode then we have run out of data
			EMPIRE_ERROR(BUFFER_UNDERFLOW, , "No more data left to read. Consumed %llu bytes", m_TotalRead);
		}
		if (bytes && m_Mode == InputMode::NORMAL) {// Get new bytes from the file.
			u64 newBytes = fread(m_Input.Start(), 1, m_Input.m_Capacity, m_FP);
			m_Input.m_Limit = newBytes;
			Read(reinterpret_cast<char*>(dest) + bytesRead, bytes - bytesRead EMPIRE_ERROR_VAR);
		}
		
	}

	inline bool CanRead() {
		if (m_Mode == InputMode::MAPPED) return m_Input.CanRead();
		else m_Input.CanRead() || (m_TotalRead < m_FileSize);
	}


protected:
	BufferedInput m_Input;
	FileOptions m_Options;
	InputMode m_Mode;

	u64 m_TotalRead = 0, m_FileSize = -1;
	FILE* m_FP = nullptr;

	friend class BufferedOutput;
};


}//namespace
