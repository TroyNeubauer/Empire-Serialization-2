//===================================================================================
// MIT Liscense
// Copyright (c) 2020, Troy Neubauer
//
//	File: Format.h
//	Contains definitions Empier Serialization's high performanace string formatting and
//		logging sub-library
//

#pragma once

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <climits>

#include <string>
#include <limits>
#include <algorithm>

#include "ForwardTypes.h"

#if CHAR_MIN == 0
	#define ES_UNSIGNED_CHAR

#elif CHAR_MIN == SCHAR_MIN
	#define ES_SIGNED_CHAR

#else
	#error Unknown char type
#endif

namespace ES {

	class Formatter
	{
	public:
		inline Formatter(char* buf, std::size_t capacity) : m_Buf(buf), m_Capacity(capacity) {}

		const char* c_str();
		inline operator const char*() { return c_str(); }

		inline std::size_t Capacity() const { return m_Capacity; }
		inline std::size_t Size() const { return m_Offset; }
		inline void Clear() { m_Offset = 0; }

		Formatter& operator<<(char value);
//If char is signed we can provide an operator that prints an byte
#ifdef ES_SIGNED_CHAR
		Formatter& operator<<(u8 value);
#endif

		Formatter& operator<<(u16 value);
		Formatter& operator<<(s16 value);

		Formatter& operator<<(u32 value);
		Formatter& operator<<(s32 value);

		Formatter& operator<<(u64 value);
		Formatter& operator<<(s64 value);

		Formatter& operator<<(float value);
		Formatter& operator<<(double value);

		Formatter& operator<<(const char* value);
		Formatter& operator<<(const std::string& value);

		template<typename T>
		Formatter& Base(T value, u8 base)
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
		inline Formatter& Write(const T& value)
		{
			*this << value;
			return *this;
		}

		template<typename T>
		inline Formatter& W(const T& value)
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
		inline void PrintUnsignedInteger(T value, u8 base = 10)
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
		inline void PrintSignedInteger(T value, u8 base = 10)
		{
			if (value < static_cast<T>(0))
			{
				PrintChar('-');
				value = -value;
			}
			PrintUnsignedInteger(value, base);
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
		inline void Clear() { m_Wrapper.Clear(); }


		template<typename T> inline Formatter& operator<<(const T& value) { return m_Wrapper << value; }
		template<typename T> inline Formatter& Base(const T& value, u8 base) { return m_Wrapper.Base(value, base); }
		template<typename T> inline Formatter& Write(const T& value) { return m_Wrapper.Write(value); }
		template<typename T> inline Formatter& W(const T& value) { return m_Wrapper.W(value); }

	private:
		char m_Buf[Cap];
		Formatter m_Wrapper;
	};

	using DefaultFormatter = SizedFormatter<256>;

#ifndef ES_FORMAT_FILE_BUF_SIZE
	#define ES_FORMAT_FILE_BUF_SIZE 512
#endif

	template<typename T>
	static std::size_t MaxDigits(T value, u8 base = 10)
	{
		return log(value) / log(base)+ 2;
	}

	class FormatFile
	{
	public:
		FormatFile(FILE* file) : m_Wrapper(m_Buf, sizeof(m_Buf)), m_File(file) {}

		template<typename T> FormatFile& operator<<(const T& value) { return Write(value); }
		template<typename T> FormatFile& Base(const T& value, u8 base) { TryFlush(MaxDigits<T>(value, base)); m_Wrapper.Base(value, base); return *this; }
		template<typename T> FormatFile& W(const T& value) { return Write(value); }
		template<typename T> FormatFile& Write(const T& value) { WriteImpl(value); return *this; }

		FormatFile& WriteImpl(char);
#ifdef ES_SIGNED_CHAR
		FormatFile& WriteImpl(u8);
#endif
		FormatFile& WriteImpl(u16);
		FormatFile& WriteImpl(s16);
		FormatFile& WriteImpl(u32);
		FormatFile& WriteImpl(s32);
		FormatFile& WriteImpl(u64);
		FormatFile& WriteImpl(s64);

		FormatFile& WriteImpl(const std::string&);
		FormatFile& WriteImpl(const char*);

		inline void F() { Flush(); }
		inline void Flush() { TryFlush(m_Wrapper.Capacity()); }
		//This will force it to flush
		~FormatFile() { Flush(); }
	private:

		void TryFlush(std::size_t requiredBytes);
		void WriteStringImpl(const char*, std::size_t length = 0);

		template<typename T> void WriteIntegral(T value)
		{
			TryFlush(std::numeric_limits<T>::digits10); m_Wrapper.Write(value);
		}

	private:

		char m_Buf[ES_FORMAT_FILE_BUF_SIZE];
		Formatter m_Wrapper;
		FILE* m_File;
	};
	class Print
	{
	public:
		static FormatFile OUT;
		static FormatFile ERR;

	};

}

