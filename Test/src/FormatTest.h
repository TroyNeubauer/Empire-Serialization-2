

#include <EmpireSerialization/Format.h>

using namespace ES;

template<typename T>
void TestIntegers(T& obj, uint64_t value)
{
	obj << static_cast<uint64_t>(value); obj << ":";
	obj << static_cast<int64_t>(value); obj << ' ';

	obj << static_cast<uint32_t>(value); obj << ":";
	obj << static_cast<int32_t>(value);	obj << ' ';

	obj << static_cast<uint16_t>(value); obj << ":";
	obj << static_cast<int16_t>(value); obj << ' ';

	obj << static_cast<uint8_t>(value); obj << ":";
	obj << static_cast<int8_t>(value);
}

template<typename T>
void TestBase(T& obj, uint64_t value, uint8_t base)
{
	obj.Base(static_cast<uint64_t>(value), base).Write(":");
	obj.Base(static_cast<int64_t>(value), base).Write(' ');

	obj.Base(static_cast<uint32_t>(value), base).Write(":");
	obj.Base(static_cast<int32_t>(value), base).Write(' ');

	obj.Base(static_cast<uint16_t>(value), base).Write(":");
	obj.Base(static_cast<int16_t>(value), base).Write(' ');

	obj.Base(static_cast<uint8_t>(value), base).Write(":");
	obj.Base(static_cast<int8_t>(value), base);
}



TEST_CASE("Print::*", "[formatting]")
{

	TestIntegers(Print::OUT, -1);
	TestIntegers(Print::OUT, 5);
	TestIntegers(Print::OUT, 11);
	Print::OUT << '\n' << '\n';
	Print::OUT.Flush();

	TestIntegers(Print::ERR, -1);
	TestIntegers(Print::ERR, 5);
	TestIntegers(Print::ERR, 11);
	Print::ERR << '\n' << '\n';
	Print::ERR.Flush();

	REQUIRE(true);
}


TEST_CASE("DefualtFormatter #0", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, 0);
	REQUIRE(std::string("0:0 0:0 0:0 0:0") == std::string(formatter.c_str()));
}

TEST_CASE("DefualtFormatter #1", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, 1);
	REQUIRE(std::string("1:1 1:1 1:1 1:1") == std::string(formatter.c_str()));
}

TEST_CASE("DefualtFormatter #2", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, -1);
	REQUIRE(std::string("18446744073709551615:-1 4294967295:-1 65535:-1 255:-1") == std::string(formatter.c_str()));
}

TEST_CASE("DefualtFormatter #3", "[formatting]")
{
	DefaultFormatter formatter;
	TestBase(formatter, 7, 2);
	REQUIRE(std::string("111:111 111:111 111:111 111:111") == std::string(formatter.c_str()));
}

TEST_CASE("DefualtFormatter #4", "[formatting]")
{
	DefaultFormatter formatter;
	const char* string = "abc test test. Print me please";
	formatter.Write(string);
	REQUIRE(std::string(string) == std::string(formatter.c_str()));
}

