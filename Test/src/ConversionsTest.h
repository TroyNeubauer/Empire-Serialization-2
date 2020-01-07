
#include <catch2/catch.hpp>
#include <initializer_list>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>
#include <EmpireSerialization/String.h>

using namespace ES;

Error NoError { ErrorCode::NONE };

constexpr std::size_t FILLER = static_cast<std::size_t>(-1);

template<typename SrcType, typename DestType>
static void Test(const SrcType* src, std::initializer_list<DestType> expectedWords, const Error& expectedError = NoError,
	std::size_t wordsReadOverride = FILLER, std::size_t charactersOverride = FILLER)
{
	if (wordsReadOverride == FILLER) wordsReadOverride = String::WordCount(src);
	if (charactersOverride == FILLER) charactersOverride = String::CharacterCount(src);


	DestType dest[2048];
	StringCodingData data;
	ErrorCode error = Conversions::Convert<SrcType, DestType>(src, String::Bytes(src), dest, sizeof(dest), data);
	if (error && expectedError.Type == ErrorCode::NONE)
	{
		DefaultFormatter formatter;
		String::PrintError(formatter, ES::GetError());
		printf("Error converting string %s", formatter.c_str());
	}

	REQUIRE(error == expectedError.Type);
	REQUIRE(data.SrcCharacterSet == GetCharsetCode<SrcType>::Code);
	REQUIRE(data.DestCharacterSet == GetCharsetCode<DestType>::Code);
	REQUIRE(data.WordsRead == wordsReadOverride);
	REQUIRE(data.WordsWritten == expectedWords.size());
	REQUIRE(data.Characters  == charactersOverride);
	
	//Make sure the expected error information matches the generated error information
	switch (expectedError.Type)
	{
		case NONE: break;//Nothing to check
		case BUFFER_OVERFLOW:
		{
			REQUIRE(GetError().BufferOverflow.BufferSize == expectedError.BufferOverflow.BufferSize);
			REQUIRE(GetError().BufferOverflow.RequiredSize == expectedError.BufferOverflow.RequiredSize);
			break;
		}
		case BUFFER_UNDERFLOW:
		{
			REQUIRE(GetError().BufferUnderflow.BufferSize == expectedError.BufferUnderflow.BufferSize);
			REQUIRE(GetError().BufferUnderflow.RequiredSize == expectedError.BufferUnderflow.RequiredSize);
			break;
		}
		case INVALID_CHARACTER:
		{
			REQUIRE(GetError().InvalidCharacter.Char == expectedError.InvalidCharacter.Char);
			REQUIRE(GetError().InvalidCharacter.CharacterSet == expectedError.InvalidCharacter.CharacterSet);
			REQUIRE(GetError().InvalidCharacter.Position.Character == expectedError.InvalidCharacter.Position.Character);
			REQUIRE(GetError().InvalidCharacter.Position.Word == expectedError.InvalidCharacter.Position.Word);
			break;
		}
		case UNSUPPORTED_CHARACTER:
		{
			REQUIRE(GetError().UnsupportedCharacter.Char == expectedError.UnsupportedCharacter.Char);
			REQUIRE(GetError().UnsupportedCharacter.CharacterSet == GetCharsetCode<DestType>::Code);
			REQUIRE(GetError().UnsupportedCharacter.Position.Word == expectedError.UnsupportedCharacter.Position.Word);
			REQUIRE(GetError().UnsupportedCharacter.Position.Character == expectedError.UnsupportedCharacter.Position.Character);
			break;
		}

		default: REQUIRE(false); break;//Fail if we dont reconogize the error
	}

	for (int i = 0; i < expectedWords.size(); i++)
	{
		REQUIRE(dest[i] == expectedWords.begin()[i]);
	}
}

template<typename SrcCharset, typename CharsetA, typename FinalCharset>
static void ConversionLoopHelper(CharsetA* src, std::size_t wordCount)
{
	Internal::TempBuffer<FinalCharset> temp(Conversions::RequiredCapacity<CharsetA, FinalCharset>(wordCount));
}


template<typename SrcCharset, typename CharsetA, typename CharsetB, typename... Args>
static void ConversionLoopHelper(CharsetA* src, std::size_t wordCount)
{

}

template<typename SrcCharset, typename... Args>
static void ConversionLoop(SrcCharset* src, std::size_t wordCount)
{
	return ConversionLoopHelper<SrcCharset, Args...>()
}


TEST_CASE("utf8->esc4 \"test\"", "[conversions]")
{
	Test<utf8, esc4>("test", { 0x10, 0x61 });
}

TEST_CASE("utf8->esc4 \"etaoinshrdlucmwf\"", "[conversions]")
{
	Test<utf8, esc4>("etaoinshrdlucmwf", { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF });
}

TEST_CASE("utf8->esc4 \"fwmculdrhsnioate\"", "[conversions]")
{
	Test<utf8, esc4>("fwmculdrhsnioate", { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 });
}


TEST_CASE("utf8->esc4 (empty)", "[conversions]")
{
	Test<utf8, esc4>("", {});
}


//https://onlineutf8tools.com/convert-utf8-to-utf32
TEST_CASE("utf8->utf32 (emojis)", "[conversions]")
{
	Test<utf8, utf32>("JSON:😂I❤EmpireSerialization\n",
		{
			0x0000004a, 0x00000053, 0x0000004f, 0x0000004e,
			0x0000003a, 0x0001f602, 0x00000049, 0x00002764,
			0x00000045, 0x0000006d, 0x00000070, 0x00000069,
			0x00000072, 0x00000065, 0x00000053, 0x00000065,
			0x00000072, 0x00000069, 0x00000061, 0x0000006c,
			0x00000069, 0x0000007a, 0x00000061, 0x00000074,
			0x00000069, 0x0000006f, 0x0000006e, 0x0000000a
 		}
	);
}

TEST_CASE("utf8->utf32 (empty)", "[conversions]")
{
	Test<utf8, utf32>("", {});
}

//UTF8 errors

TEST_CASE("utf8->utf32 misaligned character #1", "[conversions]")
{
	char badChar = 0b10000000;
	char str[2];
	str[0] = badChar;
	str[1] = 0x00;
	Error error;
	error.Type = INVALID_CHARACTER;
	error.InvalidCharacter.Char = badChar;
	error.InvalidCharacter.CharacterSet = Charset::UTF8;
	error.InvalidCharacter.Position.Character = 0;
	error.InvalidCharacter.Position.Word = 0;

	std::size_t readCount = 1, characterCount = 0;
	Test<utf8, utf32>(str, {}, error, readCount, characterCount);
}

TEST_CASE("utf8->utf32 misaligned character #2", "[conversions]")
{
	char badChar = 0b10000000;
	char str[4];
	str[0] = 'A';
	str[1] = 'B';
	str[2] = badChar;
	str[3] = 0x00;
	Error error;
	error.Type = INVALID_CHARACTER;
	error.InvalidCharacter.Char = badChar;
	error.InvalidCharacter.CharacterSet = Charset::UTF8;
	error.InvalidCharacter.Position.Character = 2;
	error.InvalidCharacter.Position.Word = 2;

	std::size_t wordsReadCount = 3, characterCount = 2;
	Test<utf8, utf32>(str, { 'A', 'B' }, error, wordsReadCount, characterCount);
}

constexpr std::size_t StrLen(const char* str)
{
	if (*str == 0x00) return 0;
	else return 1 + StrLen(str + 1);
}

void AppendChar(char* buf, const char* c, std::size_t& index)
{
	while (*c)
	{
		buf[index++] = *c;
		c++;
	}
}

TEST_CASE("utf8->utf32 misaligned character #3", "[conversions]")
{
	char buf[128];
	char badChar = 0b10000000;
	const char* emojiA = "🔥"; utf32 aCode = 0x1F525;
	const char* emojiB = "💪"; utf32 bCode = 0x1F4AA;
	const char* emojiC = "😎"; utf32 cCode = 0x1F60E;
	const char* emojiD = "🎉"; utf32 dCode = 0x1F389;
	const char* emojiE = "©";  utf32 eCode = 0x000A9;
	const char* emojiF = "Ꮌ";  utf32 fCode = 0x013BC;

	std::size_t index = 0;
	AppendChar(buf, emojiA, index);
	AppendChar(buf, emojiA, index);
	AppendChar(buf, emojiA, index);

	AppendChar(buf, emojiB, index);
	AppendChar(buf, emojiC, index);
	AppendChar(buf, emojiD, index);
	AppendChar(buf, emojiE, index);
	AppendChar(buf, emojiF, index);
	buf[index] = badChar;


	Error error;
	error.Type = INVALID_CHARACTER;
	error.InvalidCharacter.Char = badChar;
	error.InvalidCharacter.CharacterSet = Charset::UTF8;
	error.InvalidCharacter.Position.Character = 8;
	error.InvalidCharacter.Position.Word = index;

	//+1 because the erroneous character will be read
	std::size_t wordsReadCount = index + 1, characterCount = 8;
	Test<utf8, utf32>(buf, { aCode, aCode, aCode, bCode, cCode, dCode, eCode, fCode }, error, wordsReadCount, characterCount);
}

TEST_CASE("utf8->utf32 invalid utf32 header bit format", "[conversions]")
{
	char badChar = 0b11111000;
	char str[4];
	str[0] = 'A';
	str[1] = 'B';
	str[2] = badChar;
	str[3] = 0x00;
	Error error;
	error.Type = INVALID_CHARACTER;
	error.InvalidCharacter.Char = badChar;
	error.InvalidCharacter.CharacterSet = Charset::UTF8;
	error.InvalidCharacter.Position.Character = 2;
	error.InvalidCharacter.Position.Word = 2;

	std::size_t wordsReadCount = 3, characterCount = 2;
	Test<utf8, utf32>(str, { 'A', 'B' }, error, wordsReadCount, characterCount);
}

TEST_CASE("utf8->utf32 invalid utf32 surrogate", "[conversions]")
{
	char badChar = 0b11111000;
	char str[4];
	str[0] = 'A';
	str[1] = 'B';
	str[2] = 0b11100000;
	str[3] = badChar;
	Error error;
	error.Type = INVALID_CHARACTER;
	error.InvalidCharacter.Char = badChar;
	error.InvalidCharacter.CharacterSet = Charset::UTF8;
	error.InvalidCharacter.Position.Character = 2;
	error.InvalidCharacter.Position.Word = 3;

	std::size_t wordsReadCount = 4, characterCount = 2;
	Test<utf8, utf32>(str, { 'A', 'B' }, error, wordsReadCount, characterCount);
}
