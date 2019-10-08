#pragma once

#include "Buffer.h"
#include "Output.h"
#include "../EmpireException.h"

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
		this->m_Limit = capacity;
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
		std::memcpy(dest, GetPointer(), bytes);
		m_Offset += bytes;
	}


protected:
	u64 m_Limit;

	friend class BufferedOutput;
};


enum FileOptions {
	NONE = 0, NORMAL = 1, MAPPED = 2, SEE_CHANGES = 4
};

class FileInput {
public:

	FileInput(std::string path EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : FileInput(path.c_str() EMPIRE_ERROR_VAR, options) {}

	FileInput(const char* path EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : FileInput(path, 0, (std::numeric_limits<u64>::max)() EMPIRE_ERROR_VAR, options) {}
	
	FileInput(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER, FileOptions options = FileOptions::NONE) : m_Options(options) {
		if (options & FileOptions::MAPPED) OpenFileAsMapped(path, offset, length EMPIRE_ERROR_VAR);
		else OpenFileNormally(path, offset, length EMPIRE_ERROR_VAR);
	}

private:
	u64 GetInitalCapacity() { return 1024 * 1024; }

	void OpenFileAsMapped(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER) {

	}

	void OpenFileNormally(const char* path, u64 offset, u64 length EMPIRE_ERROR_PARAMETER) {
		m_FP = fopen(path, "rb");
	}

	void CloseFile() {
		if (m_FP) {
			fclose(m_FP);
			m_FP = nullptr;
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
			if (!EnsureCapacity(1 EMPIRE_ERROR_VAR)) return 0;
			Read(&byte, 1 EMPIRE_ERROR_VAR);
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
	}


protected:
	BufferedInput m_Input;
	FileOptions m_Options;
	FILE* m_FP = nullptr;

	friend class BufferedOutput;
};


}//namespace
