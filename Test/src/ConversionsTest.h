
#include <catch2/catch.hpp>
#include <initializer_list>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>

using namespace ES;

Error NoError { ErrorCode::NONE };

template<typename SrcType, typename DestType>
static void Test(const SrcType* src, std::initializer_list<DestType> expectedWords, const Error& expectedError = NoError)
{
	DestType dest[2048];
	StringCodingData data;
	ErrorCode error = Conversions::Convert<SrcType, DestType>(src, std::strlen(src), dest, sizeof(dest), data);
	if (error && expectedError.Type == ErrorCode::NONE)
	{
		DefaultFormatter formatter;
		ToString::PrintError(formatter, ES::GetError());
		printf("Error converting string %s", formatter.c_str());
	}

	REQUIRE(error == expectedError.Type);
	REQUIRE(data.Words / sizeof(expectedWords.begin()[0]) == expectedWords.size());
	REQUIRE(data.Characters  == strlen(src));
	
	//Make sure the expected error information matches the generated error information
	switch (expectedError.Type)
	{
		case NONE: break;//Nothing to check
		case UNSUPPORTED_CHARACTER:
		{
			REQUIRE(GetError().UnsupportedCharacter.Char == expectedError.UnsupportedCharacter.Char);
			REQUIRE(GetError().UnsupportedCharacter.CharacterSet == GetCharsetCode<DestType>::Code);
			REQUIRE(GetError().UnsupportedCharacter.Position.Word == expectedError.UnsupportedCharacter.Position.Word);
			REQUIRE(GetError().UnsupportedCharacter.Position.Character == expectedError.UnsupportedCharacter.Position.Character);
			break;
		}
		case BUFFER_OVERFLOW:
		{
			REQUIRE(GetError().BufferOverflow.BufferSize == expectedError.BufferOverflow.BufferSize);
			REQUIRE(GetError().BufferOverflow.RequiredSize == expectedError.BufferOverflow.RequiredSize);
			break;
		}

		default: REQUIRE(false); break;//Fail if we dont reconogize the error
	}

	for (int i = 0; i < expectedWords.size(); i++)
	{
		REQUIRE(dest[i] == expectedWords.begin()[i]);
	}
}

TEST_CASE("utf8->esc4 \"test\"")
{
	Test<utf8, esc4>("test", { 0x10, 0x61 });
}

TEST_CASE("utf8->esc4 \"etaoinshrdlucmwf\"")
{
	Test<utf8, esc4>("etaoinshrdlucmwf", { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF });
}

TEST_CASE("utf8->esc4 \"fwmculdrhsnioate\"")
{
	Test<utf8, esc4>("fwmculdrhsnioate", { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 });
}


TEST_CASE("utf8->esc4 (empty)")
{
	Test<utf8, esc4>("", {  });
}


//JSON:=0x0000004a, 0x00000053, 0x0000004f, 0x0000004e, 0x0000003a

//https://onlineutf8tools.com/convert-utf8-to-utf32
TEST_CASE("utf8->utf32 (emojis)")
{
	Test<utf8, utf32>("😂I❤EmpireSerialization\n",
		{
			0x0001f602, 0x00000049, 0x00002764,
			0x00000045, 0x0000006d, 0x00000070, 0x00000069,
			0x00000072, 0x00000065, 0x00000053, 0x00000065,
			0x00000072, 0x00000069, 0x00000061, 0x0000006c,
			0x00000069, 0x0000007a, 0x00000061, 0x00000074,
			0x00000069, 0x0000006f, 0x0000006e, 0x0000000a
 		}
	);
}


/*TEST_CASE("utf8->esc6 \"test\"")
{
	Test<utf8, esc6>("test", {0x1F, 0x61});
}

TEST_CASE("utf8->esc6 \"etaoinshrdlcumwf\"")
{
	Test<utf8, esc4>("etaoinshrdlcumwf", { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef });
}*/

