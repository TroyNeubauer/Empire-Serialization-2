#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <limits>
#include <algorithm>


namespace ES {

	static const char UPPER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const char LOWER_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };


	class Formatter
	{
	public:
		inline Formatter(char* buf, std::size_t capacity) : m_Buf(buf), m_Capacity(capacity) {}

		inline const char* c_str() { m_Buf[m_Offset] = '\0'; return m_Buf; }
		inline operator const char*() { return c_str(); }

		Formatter& operator<<(uint8_t value);
		Formatter& operator<<(uint16_t value);
		Formatter& operator<<(uint32_t value);
		Formatter& operator<<(uint64_t value);

		Formatter& operator<<(int8_t value);
		Formatter& operator<<(int16_t value);
		Formatter& operator<<(int32_t value);
		Formatter& operator<<(int64_t value);

		Formatter& operator<<(float value);
		Formatter& operator<<(double value);
		Formatter& operator<<(char value);
		Formatter& operator<<(const char* value);
		Formatter& operator<<(const std::string& value);

		template<typename T>
		Formatter& Base(T value, uint8_t base)
		{
			//Always treat value as unsigned
			typedef typename std::make_unsigned<T>::type UnsignedType;
			UnsignedType unsignedValue;

			std::memcpy(&unsignedValue, &value, sizeof(value));
			PrintUnsignedInteger(unsignedValue, base);
			return *this;
		}

		template<typename T>
		inline Formatter& Write(T value)
		{
			*this << value;
			return *this;
		}

		template<typename T>
		inline Formatter& W(T value)
		{
			return Write(value);
		}

	
	private:
		const static char UPPER_DIGITS[16];
		const static char LOWER_DIGITS[16];

		inline void PrintChar(char value)
		{
			//Save space for the null termination character
			if (m_Offset < m_Capacity - 1)
			{
				m_Buf[m_Offset++] = value;
			}
		}

		template<typename T>
		inline void PrintUnsignedInteger(T value, uint8_t base = 10)
		{
			if (value == static_cast<T>(0))
			{
				PrintChar('0');
			}
			else
			{
				char* start = m_Buf + m_Offset;
				while (value != static_cast<T>(0))
				{
					PrintChar(UPPER_DIGITS[value % base]);
					value /= base;
				}
				std::reverse(start, m_Buf + m_Offset);//The digits will appear in reverse order if we don't do this
			}
		}

		template<typename T>
		inline void PrintSignedInteger(T value, uint8_t base = 10)
		{
			if (value < static_cast<T>(0))
			{
				value = -value;
				PrintChar('-');
			}
			else
			{
				PrintUnsignedInteger(value, base);
			}
		}

	
	private:
		char* m_Buf;
		std::size_t m_Capacity;
		std::size_t m_Offset = 0;
	};


	template<std::size_t Capacity>
	class SizedFormatter
	{
	public:
		inline SizedFormatter() : m_Wrapper(m_Buf, Capacity) {}

		inline const char* c_str() { return m_Wrapper.c_str(); }
		inline operator const char*() { return c_str(); }
		inline operator Formatter&() const { return m_Wrapper; }

		template<typename T>
		inline Formatter& operator<<(T value) { return m_Wrapper << value; }

	private:
		char m_Buf[Capacity];
		Formatter m_Wrapper;
	};

	using DefaultFormatter = SizedFormatter<256>;

}

