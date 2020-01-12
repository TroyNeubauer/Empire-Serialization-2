
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
	s64 value = std::numeric_limits<s64>::min();
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
	s64 value = std::numeric_limits<s64>::min();
	REQUIRE(Internal::BottomBits<64>(value) == 0x8000000000000000L);
	value = -1;
	REQUIRE(Internal::BottomBits<64>(value) == 0xFFFFFFFFFFFFFFFFL);

}

TEST_CASE("Internal::MaxBitPlace")
{
	REQUIRE(Internal::MaxBitPlace<u8>(0xFF) == 7);
	REQUIRE((int) Internal::MaxBitPlace<u8>(0b11) == 1);
	REQUIRE(Internal::MaxBitPlace<u8>(0) == -1);

	REQUIRE(Internal::MaxBitPlace<u16>(0xFF) == 7);
	REQUIRE(Internal::MaxBitPlace<u16>(0xFFFF) == 15);
	REQUIRE(Internal::MaxBitPlace<u16>(0x0100) == 8);
	REQUIRE(Internal::MaxBitPlace<u16>(0b11) == 1);
	REQUIRE(Internal::MaxBitPlace<u16>(0) == -1);

	REQUIRE(Internal::MaxBitPlace<u32>(0xFF) == 7);
	REQUIRE(Internal::MaxBitPlace<u32>(0xFFFF) == 15);
	REQUIRE(Internal::MaxBitPlace<u32>(0x0100) == 8);
	REQUIRE(Internal::MaxBitPlace<u32>(0xFFFFFFFF) == 31);
	REQUIRE(Internal::MaxBitPlace<u32>(0x010000) == 16);
	REQUIRE(Internal::MaxBitPlace<u32>(0b11) == 1);
	REQUIRE(Internal::MaxBitPlace<u32>(0) == -1);

	REQUIRE(Internal::MaxBitPlace<u64>(0xFF) == 7);
	REQUIRE(Internal::MaxBitPlace<u64>(0xFFFF) == 15);
	REQUIRE(Internal::MaxBitPlace<u64>(0x0100) == 8);
	REQUIRE(Internal::MaxBitPlace<u64>(0xFFFFFFFF) == 31);
	REQUIRE(Internal::MaxBitPlace<u64>(0x010000) == 16);
	REQUIRE(Internal::MaxBitPlace<u64>(0xFFFFFFFFFFFFFFFF) == 63);
	REQUIRE(Internal::MaxBitPlace<u64>(0x0100000000) == 32);
	REQUIRE(Internal::MaxBitPlace<u64>(0b11) == 1);
	REQUIRE(Internal::MaxBitPlace<u64>(0) == -1);
}

static int x = 0;

static void NewAllocHandler()
{
	x = 5;
}

TEST_CASE("Internal Alloc handler functions")
{
	Internal::Log("Disregard the next message. It is being printed to test the error handler. No actual error happened");
	Internal::InvokeAllocErrorHandler();
	Internal::SetAllocErrorHandler(&NewAllocHandler);
	Internal::InvokeAllocErrorHandler();

	REQUIRE(x == 5);
}

