
#include <catch2/catch.hpp>
#include <initializer_list>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>

using namespace ES;

Error NoError { ErrorCode::NONE };

template<typename SrcType, typename DestType>
static void TestSuccess(const SrcType* src, std::initializer_list<DestType> expected, const Error& expectedError = NoError)
{
	DestType dest[2048];
	StringCodingData data;
	ErrorCode error = Conversions::Convert<SrcType, DestType>(src, std::strlen(src), dest, sizeof(dest), data);
	if (error)
	{
		DefaultFormatter formatter;
		ToString::PrintError(formatter, ES::GetError());
		printf("Error converting string %s", formatter.c_str());
	}
	REQUIRE(error == expectedError.Type);
	REQUIRE(data.Bytes == expected.size());
	REQUIRE(data.Characters  == strlen(src));
	
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

	for (int i = 0; i < expected.size(); i++)
	{
		REQUIRE(dest[i] == expected.begin()[i]);
	}
}

TEST_CASE("utf8->esc4 convert")
{
	TestSuccess<utf8, esc4>("test", {0x10, 0x61});
}
