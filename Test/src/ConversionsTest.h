
#include <catch2/catch.hpp>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>

using namespace ES;

TEST_CASE("utf8->esc4 convert")
{
	char in[] = "test";
	esc4 dest[100];
	StringCodingData data;
	ErrorCode error = Conversions::Convert<utf8, esc4>(in, std::strlen(in), dest, sizeof(dest), data);
	REQUIRE(error == ErrorCode::NONE);
	if (error) {
		DefaultFormatter formatter;
		ToString::PrintError(formatter, ES::GetError());
		printf("Error converting string %s", formatter.c_str());
	}
	REQUIRE(data.Bytes == 2);
	REQUIRE(data.Characters == 4);


}
