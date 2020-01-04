
#include <catch2/catch.hpp>
#include <initializer_list>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>
#include <Internal.h>

using namespace ES;

TEST_CASE("Internal::BottomBits(value, bits)")
{
	REQUIRE(Internal::BottomBits(0xFF, 8) == 0xFF);
	REQUIRE(Internal::BottomBits(0xFF, 4) == 0x0F);
	REQUIRE(Internal::BottomBits(0xFF, 1) == 0x01);

	REQUIRE(Internal::BottomBits(0, 32) == 0);
	REQUIRE(Internal::BottomBits(0xFFFFFFFFFFFFFFFFL, 1) == 0x01);
	REQUIRE(Internal::BottomBits(0xFFFFFFFFFFFFFFFFL, 32) == 0xFFFFFFFFL);
	REQUIRE(Internal::BottomBits(0xFFFFFFFFFFFFFFFFL, 64) == 0xFFFFFFFFFFFFFFFFL);

	//Make sure negitive signed types are treated only as bits
	int64_t value = std::numeric_limits<int64_t>::min();
	REQUIRE(Internal::BottomBits(value, 64) == 0x8000000000000000L);
	value = -1;
	REQUIRE(Internal::BottomBits(value, 64) == 0xFFFFFFFFFFFFFFFFL);
	
}

TEST_CASE("Internal::BottomBits<n>(value)")
{
	REQUIRE(Internal::BottomBits<8>(0xFF) == 0xFF);
	REQUIRE(Internal::BottomBits<4>(0xFF) == 0x0F);
	REQUIRE(Internal::BottomBits<1>(0xFF) == 0x01);

	REQUIRE(Internal::BottomBits<32>(0) == 0);
	REQUIRE(Internal::BottomBits<1>(0xFFFFFFFFFFFFFFFFL) == 0x01);
	REQUIRE(Internal::BottomBits<32>(0xFFFFFFFFFFFFFFFFL) == 0xFFFFFFFFL);
	REQUIRE(Internal::BottomBits<64>(0xFFFFFFFFFFFFFFFFL) == 0xFFFFFFFFFFFFFFFFL);

	//Make sure negitive signed types are treated only as bits
	int64_t value = std::numeric_limits<int64_t>::min();
	REQUIRE(Internal::BottomBits<64>(value) == 0x8000000000000000L);
	value = -1;
	REQUIRE(Internal::BottomBits<64>(value) == 0xFFFFFFFFFFFFFFFFL);

}


