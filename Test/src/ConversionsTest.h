
#include <catch2/catch.hpp>
#include <initializer_list>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>

using namespace ES;

Error NoError { ErrorCode::NONE };

template<typename SrcType, typename DestType>
static void Test(const SrcType* src, std::initializer_list<uint8_t> expectedBytes, const Error& expectedError = NoError)
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
	REQUIRE(data.Bytes == expectedBytes.size());
	REQUIRE(data.Characters  == strlen(src));
	
	//Make sure the expected error information matches the generated error information
	switch (expectedError.Type)
	{
		case NONE: break;//Nothing to check
		case INVALID_CHARACTER:
		{
			REQUIRE(GetError().InvalidCharacter.Char == expectedError.InvalidCharacter.Char);
			REQUIRE(GetError().InvalidCharacter.CharacterSet == GetCharsetCode<DestType>::Code);
			REQUIRE(GetError().InvalidCharacter.Position.Bytes == expectedError.InvalidCharacter.Position.Bytes);
			REQUIRE(GetError().InvalidCharacter.Position.Characters == expectedError.InvalidCharacter.Position.Characters);
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

	uint8_t destBytes[2048];
	memcpy(destBytes, dest, data.Bytes);
	for (int i = 0; i < expectedBytes.size(); i++)
	{
		REQUIRE(destBytes[i] == expectedBytes.begin()[i]);
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


/*TEST_CASE("utf8->esc6 \"test\"")
{
	Test<utf8, esc6>("test", {0x1F, 0x61});
}

TEST_CASE("utf8->esc6 \"etaoinshrdlcumwf\"")
{
	Test<utf8, esc4>("etaoinshrdlcumwf", { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef });
}*/

