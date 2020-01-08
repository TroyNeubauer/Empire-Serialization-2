#include "EmpireSerialization/Charsets.h"
#include "EmpireSerialization/ForwardTypes.h"

#include "Internal.h"

namespace ES {

#define CHARSET_DECODER_PUBLIC_METHODS_DEF(ClassName)										\
		CharsetDecoder(const ClassName::WordType* buf, std::size_t length)					\
			: m_Buf(buf), m_BufEnd(buf + length), m_BufInital(buf) {}						\
																							\
		ErrorCode Read(u32& unicode);														\
		inline std::size_t WordsRead() const { return m_Buf - m_BufInital; }				\
		inline std::size_t BytesRead() const { return WordsRead() * sizeof(ClassName); }	\
		inline std::size_t CharactersRead() const { return m_CharacterIndex; }				\




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
		std::size_t m_CharacterIndex;							\


	struct CharsetDecoder<utf8>
	{
	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(utf8)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(utf8)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf8)

	};

	ErrorCode CharsetDecoder<utf8>::Read(u32& codepoint)
	{
		ErrorCode code;

		std::size_t surrogateCount = 0;
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

	struct CharsetDecoder<utf16>
	{
	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(utf16)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(utf16)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf16)
	};

	ErrorCode CharsetDecoder<utf16>::Read(u32& codepoint)
	{
		u16 c;
		ErrorCode code;
		if (code = ReadWord(c)) return code;
		if (c < 0xD800)
		{
			codepoint = c;
		}
		else
		{
			u16& W1 = c;
			u16 W2;
			if (code = ReadWord(W2)) return code;
			//FIXME
		}
		
		m_CharacterIndex++;
		return ErrorCode::NONE;
	}


	struct CharsetDecoder<utf32>
	{

	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(utf32)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(utf32)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(utf32)
	};


	ErrorCode CharsetDecoder<utf32>::Read(u32& codepoint)
	{
		u32 c;
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

	struct CharsetDecoder<esc4>
	{
	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(esc4)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(esc4)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc4)
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
	}

	struct CharsetDecoder<esc6>
	{
	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(esc6)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(esc6)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc6)
		union
		{
			esc6::WordType m_Words[3];
			u32 m_PackedWords;
		};


	};

	ErrorCode CharsetDecoder<esc6>::Read(u32& codepoint)
	{
		if (m_CharacterIndex % 4 == 0)
		{
#ifdef ES_LITTLE_ENDIAN
			ReadWord(m_Words[0]);
			ReadWord(m_Words[1]);
			ReadWord(m_Words[2]);
#else
			ReadWord(m_Words[2]);
			ReadWord(m_Words[1]);
			ReadWord(m_Words[0]);
#endif
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

	struct CharsetDecoder<esc8>
	{
	public:
		CHARSET_DECODER_PUBLIC_METHODS_DEF(esc8)

	private:
		CHARSET_DECODER_PRIVATE_METHODS(esc8)

	private:
		CHARSET_DECODER_PRIVATE_MEMBER_VARIABLES(esc8)


	};

	ErrorCode CharsetDecoder<esc8>::Read(u32& codepoint)
	{
		return ErrorCode::NONE;
	}




}


