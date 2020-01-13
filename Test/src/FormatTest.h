

#include <EmpireSerialization/Format.h>

#include <fstream>

using namespace ES;

template<typename T>
void TestIntegers(T& obj, s64 value)
{
	obj << static_cast<u64>(value); obj << ":";
	obj << static_cast<s64>(value); obj << ' ';

	obj << static_cast<u32>(value); obj << ":";
	obj << static_cast<s32>(value);	obj << ' ';

	obj << static_cast<u16>(value); obj << ":";
	obj << static_cast<s16>(value); obj << ' ';

	obj << static_cast<u8>(value); obj;

}

template<typename T>
void TestBase(T& obj, s64 value, u8 base)
{
	obj.Base(static_cast<u64>(value), base); obj.Write(":");
	obj.Base(static_cast<s64>(value), base); obj.Write(' ');

	obj.Base(static_cast<u32>(value), base); obj.Write(":");
	obj.Base(static_cast<s32>(value), base); obj.Write(' ');

	obj.Base(static_cast<u16>(value), base); obj.Write(":");
	obj.Base(static_cast<s16>(value), base); obj.Write(' ');

	obj.Base(static_cast<u8>(value), base);
}


static const char* testFileName = "TestFile.txt";

TEST_CASE("Print::*", "[formatting]")
{
	FILE* file = fopen(testFileName, "wb");
	REQUIRE(file);
	FormatFile formatter(file);
	
	TestIntegers(formatter, -1); formatter.Write('\n');
	TestIntegers(formatter, 5); formatter.Write('\n');
	TestIntegers(formatter, 11); formatter.Write('\n');
	formatter.Flush();
	fclose(file);

	std::ifstream infile(testFileName);
	REQUIRE(infile.good());

	std::string expected[] = 
	{
		"18446744073709551615:-1 4294967295:-1 65535:-1 255",
		"5:5 5:5 5:5 5", 
		"11:11 11:11 11:11 11",
	};

	std::string line;
	int i = 0;
	while (std::getline(infile, line))
	{
		REQUIRE(line == expected[i++]);
	}

}


TEST_CASE("DefualtFormatter #0", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, 0);
	REQUIRE(std::string("0:0 0:0 0:0 0") == formatter.c_str());
}

TEST_CASE("DefualtFormatter #1", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, 1);
	REQUIRE(std::string("1:1 1:1 1:1 1") == formatter.c_str());
}

TEST_CASE("DefualtFormatter #2", "[formatting]")
{
	DefaultFormatter formatter;
	TestIntegers(formatter, -1);
	REQUIRE(std::string("18446744073709551615:-1 4294967295:-1 65535:-1 255") == formatter.c_str());
}

TEST_CASE("DefualtFormatter #3", "[formatting]")
{
	DefaultFormatter formatter;
	TestBase(formatter, 7, 2);
	REQUIRE(std::string("111:111 111:111 111:111 111") == formatter.c_str());
}

TEST_CASE("DefualtFormatter #4", "[formatting]")
{
	DefaultFormatter formatter;
	const char* string = "abc test test. Print me please";
	formatter.Write(string);
	REQUIRE(std::string(string) == formatter.c_str());
}

TEST_CASE("DefualtFormatter #5", "[formatting]")
{
	DefaultFormatter formatter;
	std::string string = "abc test test. Print me please. This is an std::string";
	formatter.Write(string);
	REQUIRE(string == formatter.c_str());
}

TEST_CASE("File formatter flush", "[formatting]")
{
	FILE* testFile = fopen("test.txt", "w");
	REQUIRE(testFile);
	FormatFile formatter(testFile);

	//Each iteration prints way more than one byte so we will trigger a flush
	for (int i = 0; i < formatter.BufferCapacity(); i++)
	{
		formatter << "Print me test string" << '\n' << 5;
	}

	std::vector<char> str(formatter.BufferCapacity());
	std::fill(str.begin(), str.begin() + str.capacity(), 'A');
	str.push_back('\0');
	formatter << str.data();

}

TEST_CASE("DefualtFormatter Code coverage", "[formatting]")
{
	DefaultFormatter formatter;
	formatter << 0.5f << 1.0;

	REQUIRE(true);
}

