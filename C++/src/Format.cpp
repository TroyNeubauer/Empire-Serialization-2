#include "Format.h"

#include "Internal.h"

namespace ES {

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
