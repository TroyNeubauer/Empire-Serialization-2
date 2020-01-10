#include "EmpireSerialization/Charsets.h"
#include "EmpireSerialization/ForwardTypes.h"

#include "Internal.h"

namespace ES {

#define CHARSET_DECODER_PUBLIC_METHODS_DEF(ClassName)										\
		CharsetDecoder(const ClassName* buf, std::size_t length)							\
			: m_Buf(reinterpret_cast<const ClassName::WordType*>(buf)), m_BufEnd(m_Buf + length), m_BufInital(m_Buf) {}						\
																							\
		ErrorCode Read(u32& unicode);														\
		inline std::size_t WordsRead() const { return m_Buf - m_BufInital; }				\
		inline std::size_t BytesRead() const { return WordsRead() * sizeof(ClassName); }	\
		inline std::size_t CharactersRead() const { return m_CharacterIndex; }				\
		inline bool HasChars() const { return m_Buf != m_BufEnd; }							\




#define CHARSET_DECODER_PRIVATE_METHODS(ClassName)							\
		ErrorCode ReadWord(ClassName::WordType& word)						\
		{																	\
			if (m_Buf == m_BufEnd)											\
			{																\
				Error& error = Internal::GetError();														\
				error.BufferUnderflow.BufferSize = BytesRead();												\
				error.BufferUnderflow.RequiredSize = BytesRead() + sizeof(ClassName);						\
																											\
				return error.Type = ErrorCode::BUFFER_UNDERFLOW;											\
																											\
			}																			\
			word = *m_Buf++;															\
			/*if (flip) Internal::ByteSwap(c);*/										\
			m_CharacterIndex++;															\
			return ErrorCode::NONE;														\
		}																				\
																						\
		ErrorCode InvalidCharacterError()												\
		{																				\
			/*Its the character last returned*/											\
			Error& error = Internal::GetError();										\
			error.InvalidCharacter.Char = m_Buf[-1];									\
			error.InvalidCharacter.CharacterSet = GetCharsetCode<ClassName>::Code;		\
																						\
			error.InvalidCharacter.Position.Word = WordsRead() - 1;						\
			error.InvalidCharacter.Position.Character = m_CharacterIndex;				\
																						\
			return error.Type = ErrorCode::INVALID_CHARACTER;							\
		}																				\
																						\



#define CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(ClassName)		\
		const ClassName::WordType* m_Buf;						\
		const ClassName::WordType* m_BufEnd;					\
		const ClassName::WordType* const m_BufInital;			\
		std::size_t m_CharacterIndex = 0;						\
	
	template<>
	struct CharsetDecoder<utf8>
	{

	public:  CHARSET_DECODER_PUBLIC_METHODS_DEF(utf8)

	private: CHARSET_DECODER_PRIVATE_METHODS(utf8)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf8)

	};

	ErrorCode CharsetDecoder<utf8>::Read(u32& codepoint)
	{
		ErrorCode code;

		u8 surrogateCount = 0;
		u8 c;
		if (code = ReadWord(c)) return code;
		if (Internal::TopNBitsAre<1, 0b0>(c))// 7bit ASCII
		{
			codepoint = static_cast<u32>(c);//Easy handling for single byte utf8 characters
		}
		else if (Internal::TopNBitsAre<3, 0b110>(c))// 11 bits
		{
			surrogateCount = 1;
		}
		else if (Internal::TopNBitsAre<4, 0b1110>(c))// 16 bits
		{
			surrogateCount = 2;
		}
		else if (Internal::TopNBitsAre<5, 0b11110>(c))// 21 bits
		{
			surrogateCount = 3;
		}
		//Handle invalid bit patterns
		else if (Internal::TopNBitsAre<2, 0b10>(c))// Invalid. Middle of character
		{
			Internal::GetError().AdditionalMessage.Write("Mis-aligned UTF8 character: 0b").Base(c, 2)
				.Write(" The high bits of a new character must start with with 0, or 110, or 1110, or 11110 per the utf8 standard");
			return InvalidCharacterError();
		}
		else // Unsupported bit format Could be a 26 or 31 or 36 bit encoding which are disallowed
		{
			Internal::GetError().AdditionalMessage.Write("Invalid UTF8 bit format: 0b").Base(c, 2);
			return InvalidCharacterError();
		}

		//We know how many bytes need to be read for multi byte encoding
		if (surrogateCount != 0)
		{
			//Save bottom X bytes of the first byte according to the utf8 pattern
			codepoint = Internal::BottomBits(c, 6 - surrogateCount);
			int i = 0;
			while (surrogateCount)
			{
				//Look for buffer overflow, read char and check for errors in the second byte in this character
				if (code = ReadWord(c)) return code;
				if (!Internal::TopNBitsAre<2, 0b10>(c))
				{
					(Internal::GetError().AdditionalMessage << "The UTF8 character surrogate #" << i << " has invaid top bits. Expected 0b11... Got 0b").Base(c, 2);
					return InvalidCharacterError();
				}
				//Append aurrogate to unicode
				codepoint = (codepoint << 6) | Internal::BottomBits<6>(c);

				surrogateCount--;
				i++;
			}
		}
		m_CharacterIndex++;
		return ErrorCode::NONE;
	}

	template<>
	struct CharsetDecoder<utf16>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(utf16)

	private: CHARSET_DECODER_PRIVATE_METHODS(utf16)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf16)
	};

	ErrorCode CharsetDecoder<utf16>::Read(u32& codepoint)
	{
		char16_t c;
		ErrorCode code;
		if (code = ReadWord(c)) return code;
		if (c < 0xD800)
		{
			codepoint = c;
		}
		else
		{
			char16_t& W1 = c;
			char16_t W2;
			if (code = ReadWord(W2)) return code;
			//FIXME
		}
		
		m_CharacterIndex++;
		return ErrorCode::NONE;
	}


	template<>
	struct CharsetDecoder<utf32>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(utf32)

	private: CHARSET_DECODER_PRIVATE_METHODS(utf32)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf32)
	};


	ErrorCode CharsetDecoder<utf32>::Read(u32& codepoint)
	{
		char32_t c;
		ErrorCode code;
		if (code = ReadWord(c)) return code;
		if (c < 0xFFFFFFFF)//FIXME actually check for word being out of the unicode range
		{
			codepoint = static_cast<u32>(c);
		}
		else
		{
			return InvalidCharacterError();
		}
		
		m_CharacterIndex++;
		return ErrorCode::NONE;
	}

	template<>
	struct CharsetDecoder<esc4>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc4)

	private: CHARSET_DECODER_PRIVATE_METHODS(esc4)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc4)

		bool m_Stale = true;
		esc4::WordType m_CurrentWord;
	};

	ErrorCode CharsetDecoder<esc4>::Read(u32& codepoint)
	{
		ErrorCode code;
		if (m_Stale)
		{
			//Every other time read a new value and give either the top 4 bits or the bottom 4 bits
			if (code = ReadWord(m_CurrentWord)) return code;
			codepoint = (static_cast<u32>(m_CurrentWord) >> 4) & 0b1111;
		}
		else
		{
			codepoint = (static_cast<u32>(m_CurrentWord) >> 0) & 0b1111;
		}
		m_Stale = !m_Stale;

		return ErrorCode::NONE;
	}

	template<>
	struct CharsetDecoder<esc6>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc6)

	private: CHARSET_DECODER_PRIVATE_METHODS(esc6)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc6)

		union
		{
			esc6::WordType m_Words[4];
			u32 m_PackedWords;
		};



	};

	ErrorCode CharsetDecoder<esc6>::Read(u32& codepoint)
	{
		if (m_CharacterIndex % 4 == 0)
		{
			ReadWord(m_Words[2]);
			ReadWord(m_Words[1]);
			ReadWord(m_Words[0]);

			codepoint = (m_PackedWords >> 18) & 0b111111;
		}
		else if (m_CharacterIndex % 4 == 1)
		{
			codepoint = (m_PackedWords >> 12) & 0b111111;
		}
		else if (m_CharacterIndex % 4 == 2)
		{
			codepoint = (m_PackedWords >> 6) & 0b111111;
		}
		else if (m_CharacterIndex % 4 == 3)
		{
			codepoint = (m_PackedWords >> 0) & 0b111111;
		}
		return ErrorCode::NONE;
	}

	template<>
	struct CharsetDecoder<esc8>
	{

	public: CHARSET_DECODER_PUBLIC_METHODS_DEF(esc8)

	private: CHARSET_DECODER_PRIVATE_METHODS(esc8)

	private: CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc8)


	};

	ErrorCode CharsetDecoder<esc8>::Read(u32& codepoint)
	{
		return ErrorCode::NONE;
	}


	// ==================== CharsetEncoders ====================


#define CHARSET_ENCODER_PUBLIC_METHODS_DEF(ClassName)											\
		CharsetEncoder(ClassName* buf, std::size_t length)										\
			: m_Buf(reinterpret_cast<ClassName::WordType*>(buf)), m_BufEnd(m_Buf + length), m_BufInital(m_Buf) {}						\
																								\
		ErrorCode Write(u32 unicode);															\
		inline std::size_t WordsWritten() const { return m_Buf - m_BufInital; }					\
		inline std::size_t BytesWritten() const { return WordsWritten() * sizeof(ClassName); }	\
		inline std::size_t CharactersWritten() const { return m_CharacterIndex; }				\




#define CHARSET_ENCODER_PRIVATE_METHODS(ClassName)										\
		ErrorCode WriteWord(ClassName::WordType word)									\
		{																				\
			if (m_Buf == m_BufEnd)														\
			{																			\
				Error& error = Internal::GetError();									\
				error.BufferOverflow.BufferSize = BytesWritten();						\
				error.BufferOverflow.RequiredSize = BytesWritten() + sizeof(ClassName);	\
																						\
				return error.Type = ErrorCode::BUFFER_UNDERFLOW;						\
																						\
			}																			\
			*m_Buf++ = word;															\
			/*if (flip) Internal::ByteSwap(c);*/										\
			m_CharacterIndex++;															\
			return ErrorCode::NONE;														\
		}																				\
																						\
		ErrorCode UnsupportedCharacterError(u32 unicode)								\
		{																				\
			/*Its the character last returned*/											\
			Error& error = Internal::GetError();										\
			error.UnsupportedCharacter.Char = unicode;									\
			error.UnsupportedCharacter.CharacterSet = GetCharsetCode<ClassName>::Code;	\
																						\
			error.UnsupportedCharacter.Position.Word = WordsWritten();					\
			error.UnsupportedCharacter.Position.Character = m_CharacterIndex;			\
																						\
			return error.Type = ErrorCode::UNSUPPORTED_CHARACTER;						\
		}																				\
																						\



#define CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(ClassName)		\
		ClassName::WordType* m_Buf;								\
		ClassName::WordType* const m_BufEnd;					\
		ClassName::WordType* const m_BufInital;					\
		std::size_t m_CharacterIndex = 0;						\



	template<>
	struct CharsetEncoder<utf8>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf8)

	private: CHARSET_ENCODER_PRIVATE_METHODS(utf8)
				
	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf8)

	};

	ErrorCode CharsetEncoder<utf8>::Write(u32 codepoint)
	{
		ErrorCode code;

		s8 bits = Internal::MaxBitPlace(codepoint);
		if (bits == 0 || bits <= 7)
		{
			//ASCII
			utf8::WordType utf8Char = static_cast<utf8::WordType>(codepoint);
			if (code = WriteWord(utf8Char)) return code;
		}
		else
		{
			utf8::WordType byte;
			//Fix bits to specific size based on the utf8 template
			if (bits <= 11)
			{
				byte = (0b110 << 5) | (codepoint >> 6);
				bits = 6;//6 more bits to encode
			}
			else if (bits <= 16)
			{
				byte = (0b1110 << 4) | (codepoint >> 12);
				bits = 12;//12 more bits to encode
			}
			else if (bits <= 21)
			{
				byte = (0b11110 << 3) | (codepoint >> 18);
				bits = 18;//18 more bits to encode
			}
			else
			{
				Internal::GetError().AdditionalMessage.Write("Character is outside the allowed range for a unicode codepoint");
				return UnsupportedCharacterError(codepoint);
			}
			//Write the first byte
			if (code = WriteWord(byte)) return code;
			//Encode the remaining 6-18 bits
			while (bits > 0)
			{
				byte = static_cast<utf8::WordType>((codepoint >> (bits - 6)) & 0b00111111);
				byte |= 0b10000000;
				if (code = WriteWord(byte)) return code;

				bits -= 6;
			}
			ES_ASSERT(bits == 0, "utf32 codepoint was not fully encoded!");
		}

		m_CharacterIndex++;
		
		return ErrorCode::NONE;
	}

	template<>
	struct CharsetEncoder<utf16>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf16)

	private: CHARSET_ENCODER_PRIVATE_METHODS(utf16)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf16)

	};

	ErrorCode CharsetEncoder<utf16>::Write(u32 codepoint)
	{
		ErrorCode code;
		if (codepoint < 0xD800)
		{
			//Single byte
			if (code = WriteWord(static_cast<utf16::WordType>(codepoint))) return code;
		}
		else
		{
			utf16::WordType W1 = 0xD800 | static_cast<utf16::WordType>((codepoint >> 10) & 0b1111111111);
			utf16::WordType W2 = 0xDC00 | static_cast<utf16::WordType>((codepoint >>  0) & 0b1111111111);
			if (code = WriteWord(W1)) return code;
			if (code = WriteWord(W2)) return code;

		}

		m_CharacterIndex++;

		return ErrorCode::NONE;
	}




	template<>
	struct CharsetEncoder<utf32>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(utf32)

	private: CHARSET_ENCODER_PRIVATE_METHODS(utf32)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(utf32)

	};

	ErrorCode CharsetEncoder<utf32>::Write(u32 codepoint)
	{
		ErrorCode code;
		utf32::WordType word = codepoint;
		if (code = WriteWord(word)) return code;
		
		return ErrorCode::NONE;
	}





	template<>
	struct CharsetEncoder<esc4>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc4)

	private: CHARSET_ENCODER_PRIVATE_METHODS(esc4)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc4)
		esc4::WordType m_CurrentWord;
	};

	ErrorCode CharsetEncoder<esc4>::Write(u32 codepoint)
	{
		ErrorCode code;
		esc4::WordType encoded;
		if (codepoint >= ESC4_ENCODE.size() || (encoded = ESC4_ENCODE[codepoint]) == -1) return UnsupportedCharacterError(codepoint);

		if (m_CharacterIndex % 2 == 0)
		{
			m_CurrentWord = encoded << 4;
		}
		else
		{
			m_CurrentWord |= encoded;//Encoded is guarnteed to only have the lowest 4 bits set
			if (code = WriteWord(m_CurrentWord)) return code;
		}
		m_CharacterIndex++;

		return ErrorCode::NONE;
	}




	template<>
	struct CharsetEncoder<esc6>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc6)

	private: CHARSET_ENCODER_PRIVATE_METHODS(esc6)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc6)
		u32 m_State;
	};

	ErrorCode CharsetEncoder<esc6>::Write(u32 codepoint)
	{
		ErrorCode code;

		esc6::WordType encoded;
		if (codepoint >= ESC6_ENCODE.size() || (encoded = ESC6_ENCODE[codepoint]) == -1) return UnsupportedCharacterError(codepoint);

		if (m_CharacterIndex % 2 == 0)
		{
			//top 6 bits of 24 bit word
			m_State = (encoded << 18);
		}
		else if (m_CharacterIndex % 2 == 1)
		{
			m_State |= (encoded << 12);
		}
		else if (m_CharacterIndex % 2 == 2)
		{
			m_State |= (encoded << 6);
		}
		else if (m_CharacterIndex % 2 == 3)
		{
			m_State |= (encoded << 6);
			//Write all three bytes
			if (code = WriteWord(static_cast<esc6::WordType>((m_State >> 16) & 0xFF))) return code;
			if (code = WriteWord(static_cast<esc6::WordType>((m_State >> 8) & 0xFF))) return code;
			if (code = WriteWord(static_cast<esc6::WordType>((m_State >> 0) & 0xFF))) return code;
		}
		m_CharacterIndex++;

		return ErrorCode::NONE;
	}



	template<>
	struct CharsetEncoder<esc8>
	{

	public:	 CHARSET_ENCODER_PUBLIC_METHODS_DEF(esc8)

	private: CHARSET_ENCODER_PRIVATE_METHODS(esc8)

	private: CHARSET_ENCODER_PRIVATE_MEMBER_VARIABLES(esc8)

	};

	ErrorCode CharsetEncoder<esc8>::Write(u32 codepoint)
	{


		return ErrorCode::NONE;
	}

	const CharsetInfo& GetCharsetInfo(Charset charset)
	{
		switch (charset)
		{
		case Charset::UTF8:
		{
			static CharsetInfo UTF8Info;
			UTF8Info.Name = "UTF-8";
			UTF8Info.WordSize = sizeof(utf8);
			UTF8Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;
			UTF8Info.CharsetCode = 3;

			return UTF8Info;
		}
		case Charset::UTF16:
		{
			static CharsetInfo UTF16Info;
			UTF16Info.Name = "UTF-16";
			UTF16Info.WordSize = sizeof(utf8);
			UTF16Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;
			UTF16Info.CharsetCode = 4;

			return UTF16Info;
		}
		case Charset::UTF32:
		{
			static CharsetInfo UTF32Info;
			UTF32Info.Name = "UTF-8";
			UTF32Info.WordSize = sizeof(utf8);
			UTF32Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;
			UTF32Info.CharsetCode = 5;

			return UTF32Info;
		}
		case Charset::ESC4:
		{
			static CharsetInfo ESC4Info;
			ESC4Info.Name = "ESC4";
			ESC4Info.WordSize = sizeof(esc4);
			ESC4Info.CharactersPerWord = 2.0;
			ESC4Info.CharsetCode = 0;

			return ESC4Info;
		}
		case Charset::ESC6:
		{
			static CharsetInfo ESC6Info;
			ESC6Info.Name = "ESC6";
			ESC6Info.WordSize = sizeof(esc6);
			ESC6Info.CharactersPerWord = 4.0 / 3.0;//4 characters per every three words
			ESC6Info.CharsetCode = 1;

			return ESC6Info;
		}
		case Charset::ESC8:
		{
			static CharsetInfo ESC8Info;
			ESC8Info.Name = "ESC8+";
			ESC8Info.WordSize = sizeof(esc8);
			ESC8Info.CharactersPerWord = CharsetInfo::VARIABLE_CHARACTERS_PER_WORD;
			ESC8Info.CharsetCode = 2;

			return ESC8Info;
		}
		case Charset::ANY_CHARSET:
		{
			static CharsetInfo AnyCharsetInfo;
			AnyCharsetInfo.Name = "Any Charset";
			AnyCharsetInfo.WordSize = 0;
			AnyCharsetInfo.CharactersPerWord = 0.0;
			AnyCharsetInfo.CharsetCode = 6;

			return AnyCharsetInfo;
		}
		default: ES_ABORT("Invalid charset!");
		}
	}

	const utf8* SL(const char* literal)
	{
		return reinterpret_cast<const utf8*>(literal);
	}

	const utf16* SL(const char16_t* literal)
	{
		return reinterpret_cast<const utf16*>(literal);
	}

	const utf32* SL(const char32_t* literal)
	{
		return reinterpret_cast<const utf32*>(literal);
	}

}


