#include "Format.h"

#include "Internal.h"

namespace ES {

	const char Formatter::UPPER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	const char Formatter::LOWER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };


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


}
