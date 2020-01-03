#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <limits>
#include <algorithm>


namespace ES {

	class Formatter
	{
	public:
		inline Formatter(char* buf, std::size_t capacity) : m_Buf(buf), m_Capacity(capacity) {}

		const char* c_str();
		inline operator const char*() { return c_str(); }

		inline std::size_t Capacity() const { return m_Capacity; }
		inline std::size_t Size() const { return m_Offset; }

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

			//Convert to the unsigned type using memcpy instead of a cast to make the compiler happy
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


	template<std::size_t Cap>
	class SizedFormatter
	{
	public:
		inline SizedFormatter() : m_Wrapper(m_Buf, Cap) {}

		//Proxy all functions to the wrapper
		inline const char* c_str() { return m_Wrapper.c_str(); }
		inline operator const char*() { return c_str(); }
		inline operator Formatter&() { return m_Wrapper; }

		inline std::size_t Capacity() const { return m_Wrapper.Capacity(); }
		inline std::size_t Size() const { return m_Wrapper.Size(); }


		template<typename T> inline Formatter& operator<<(T value) { return m_Wrapper << value; }
		template<typename T> inline Formatter& Base(T value, uint8_t base) { return m_Wrapper.Base(value, base); }
		template<typename T> inline Formatter& Write(T value) { return m_Wrapper.Write(value); }
		template<typename T> inline Formatter& W(T value) { return m_Wrapper.W(value); }

	private:
		char m_Buf[Cap];
		Formatter m_Wrapper;
	};

	using DefaultFormatter = SizedFormatter<256>;

}

