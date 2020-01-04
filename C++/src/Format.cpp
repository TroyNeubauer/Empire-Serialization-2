#include "Format.h"

#include "Internal.h"

namespace ES {

	const char Formatter::UPPER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	const char Formatter::LOWER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	FormatFile Print::OUT = STDOUT_FILENO;
	FormatFile Print::ERR = STDERR_FILENO;


	const char* Formatter::c_str()
	{
		ES_ASSERT(m_Offset < m_Capacity, "Formatter has no space for the null termination character! "
			"m_Offset should always save space for the null byte!");
		m_Buf[m_Offset] = '\0';
		return m_Buf;
	}

	Formatter& Formatter::operator<<(uint8_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(uint16_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(uint32_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(uint64_t value)
	{
		PrintUnsignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(int8_t value)
	{
		PrintSignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(int16_t value)
	{
		PrintSignedInteger(value);

		return *this;
	}

	Formatter& Formatter::operator<<(int32_t value)
	{
		PrintSignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(int64_t value)
	{
		PrintSignedInteger(value);
		return *this;
	}

	Formatter& Formatter::operator<<(float value)
	{
		*this << "Cannot print floats yet...";
		return *this;
	}

	Formatter& Formatter::operator<<(double value)
	{
		*this << "Cannot print doubles yet...";
		return *this;
	}

	Formatter& Formatter::operator<<(char value)
	{
		PrintChar(value);
		return *this;
	}

	Formatter& Formatter::operator<<(const char* value)
	{
		size_t bytes = ES_MIN(std::strlen(value), m_Capacity - m_Offset - 1);
		std::memcpy(m_Buf + m_Offset, value, bytes);
		m_Offset += bytes;
		return *this;
	}

	Formatter& Formatter::operator<<(const std::string& value)
	{
		*this << value.c_str();
		return *this;
	}

	FormatFile& FormatFile::WriteImpl(uint8_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(uint16_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(uint32_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(uint64_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(int8_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(int16_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(int32_t value)
	{
		WriteIntegral(value);
		return *this;
	}
	FormatFile& FormatFile::WriteImpl(int64_t value)
	{
		WriteIntegral(value);
		return *this;
	}

	FormatFile& FormatFile::WriteImpl(const std::string& value)
	{
		WriteStringImpl(value.c_str(), value.size());
		return *this;
	}
	
	FormatFile& FormatFile::WriteImpl(const char* value)
	{
		WriteStringImpl(value, strlen(value));
		return *this;
	}

	void FormatFile::WriteStringImpl(const char* value, std::size_t length)
	{
		if (length >= m_Wrapper.Capacity() / 2)
		{
			//flush the current data and then print the string directly
			Flush();
			std::size_t bytesWritten = write(m_FD, value, length);
			ES_ASSERT(bytesWritten == length, "Failed to write to STDOut");
			Flush();//fsync
		}
		else
		{
			//If it is a small string make sure we have enough space then buffer it
			TryFlush(length);
			m_Wrapper.Write(value);
		}
	}

	void FormatFile::TryFlush(std::size_t requiredBytes)
	{
		if (requiredBytes + m_Wrapper.Size() >= m_Wrapper.Capacity())
		{
			if (m_Wrapper.Size())
			{
				std::size_t bytesWritten = write(m_FD, m_Buf, m_Wrapper.Size());
				ES_ASSERT(bytesWritten == m_Wrapper.Size(), "Failed to write to STDOut");
				m_Wrapper.Clear();
			}
			
			fsync(m_FD);
		}
	}

}
