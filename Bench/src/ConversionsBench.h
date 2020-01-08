
#include <fstream>
#include <iostream>
#include <random>

#include <EmpireSerialization/Conversions.h>
#include <EmpireSerialization/Charsets.h>
#include <Internal.h>

using namespace ES;

//Assume the source file is always utf8
template<typename SrcType, typename DestType>
void TestFromFile(const char* benchName, const char* fileName, double fileMultiplier = 1.0)
{
	std::ifstream file(fileName);
	REQUIRE(file.good());
	
	std::vector<utf8> rawFile((std::istreambuf_iterator<utf8>(file)), std::istreambuf_iterator<utf8>());
	std::vector<utf8> finalChars;
	const std::size_t characters = static_cast<std::size_t>(rawFile.size() * fileMultiplier);
	std::size_t i = characters;
	while (i > 0)
	{
		std::size_t copyCount = ES_MIN(rawFile.size(), i);
		finalChars.insert(finalChars.begin(), rawFile.begin(), rawFile.begin() + copyCount);
		i -= copyCount;
	}

	std::vector<SrcType> src(characters);
	std::vector<DestType> dest(characters);
	
	//We can use the file directly
	if (std::is_same<SrcType, utf8>::value)
	{
		std::copy(finalChars.begin(), finalChars.begin() + characters, src.data());
	}
	else
	{
		//We need to convert from the utf8 file to the source type
		StringCodingData data;
		REQUIRE(Conversions::Convert<utf8, SrcType>(finalChars.data(), characters, src.data(), characters, data) == ErrorCode::NONE);
		REQUIRE(data.Characters == characters);
	}
	
    BENCHMARK(benchName)
	{
		StringCodingData data;
		ErrorCode error = Conversions::Convert<SrcType, DestType>(src.data(), characters, dest.data(), characters, data);
		if (error) REQUIRE(false);//Use this format for performanace
		return error;
    };

}


//Using the ASCII values provided, this methoc creates a string of length characters and then tests the performanace of converting
//It to the desired charset

template<typename SrcType, typename DestType, std::size_t Len>
void TestFromRandomCharset(const char* benchName, const std::array<char, Len>& chars, std::size_t wordCount)
{

	std::vector<utf8> srcUtf8;
	std::vector<SrcType> src(Conversions::RequiredCapacity<utf8, SrcType>(wordCount));
	std::vector<DestType> dest(Conversions::RequiredCapacity<utf8, DestType>(wordCount));

	std::random_device device;
	std::default_random_engine rng(device());
	int min = 0;

	if (chars[0] == 0x00) min = 1;//Dont insert random null characters if this character set supports null characters
	std::uniform_int_distribution<std::size_t> range(0, Len);
	
	for (std::size_t i = 0; i < wordCount; i++)
	{
		srcUtf8.push_back(static_cast<utf8>(chars[range(rng)]));
	}
	StringCodingData data;
	REQUIRE(Conversions::Convert(srcUtf8.data(), srcUtf8.size(), src.data(), src.size(), data) == ErrorCode::NONE);
	std::size_t srcWords = data.WordsWritten;
	BENCHMARK(benchName)
	{
		StringCodingData data;
		ErrorCode error = Conversions::Convert<SrcType, DestType>(src.data(), srcWords, dest.data(), dest.size(), data);
		if (error) REQUIRE(false);//Use this format for performanace
		return error;
    };
}

//Origional about 2ns per character for direct. Double for things like utf16->esc4
TEST_CASE("utf8->esc4") {

	TestFromRandomCharset<utf8, esc4>("100B utf8->esc4", ESC4_DECODE, 100);
	TestFromRandomCharset<utf8, esc4>(  "1K utf8->esc4", ESC4_DECODE, 1000);
	TestFromRandomCharset<utf8, esc4>( "10K utf8->esc4", ESC4_DECODE, 10000);
	TestFromRandomCharset<utf8, esc4>("100K utf8->esc4", ESC4_DECODE, 100000);
	TestFromRandomCharset<utf8, esc4>( "1MB utf8->esc4", ESC4_DECODE, 1000000);
	TestFromRandomCharset<utf8, esc4>("10MB utf8->esc4", ESC4_DECODE, 10000000);
}

TEST_CASE("utf8->utf32") {

	TestFromFile<utf8, utf32>("100B utf8->utf32", "res/conversion/1k_ascii.txt", 0.1);
	TestFromFile<utf8, utf32>("1K utf8->utf32", "res/conversion/1k_ascii.txt", 1.0);
	TestFromFile<utf8, utf32>("10K utf8->utf32", "res/conversion/1k_ascii.txt", 10);
	TestFromFile<utf8, utf32>("100K utf8->utf32", "res/conversion/100k_ascii.txt");

	TestFromFile<utf8, utf32>("1MB utf8->utf32", "res/conversion/100k_ascii.txt", 10);
	TestFromFile<utf8, utf32>("10MB utf8->utf32", "res/conversion/100k_ascii.txt", 100);
}

TEST_CASE("utf16->utf32") {

	TestFromFile<utf16, utf32>("100B utf16->utf32", "res/conversion/1k_ascii.txt", 0.1);
	TestFromFile<utf16, utf32>("1K utf16->utf32", "res/conversion/1k_ascii.txt", 1.0);
	TestFromFile<utf16, utf32>("10K utf16->utf32", "res/conversion/1k_ascii.txt", 10);
	TestFromFile<utf16, utf32>("100K utf16->utf32", "res/conversion/100k_ascii.txt");

	TestFromFile<utf16, utf32>("1MB utf16->utf32", "res/conversion/100k_ascii.txt", 10);
	TestFromFile<utf16, utf32>("10MB utf16->utf32", "res/conversion/100k_ascii.txt", 100);

}

TEST_CASE("utf8->utf16") {

	TestFromFile<utf8, utf16>("100B utf8->utf16", "res/conversion/1k_ascii.txt", 0.1);
	TestFromFile<utf8, utf16>("1K utf8->utf16", "res/conversion/1k_ascii.txt", 1.0);
	TestFromFile<utf8, utf16>("10K utf8->utf16", "res/conversion/1k_ascii.txt", 10);
	TestFromFile<utf8, utf16>("100K utf8->utf16", "res/conversion/100k_ascii.txt");

	TestFromFile<utf8, utf16>("1MB utf8->utf16", "res/conversion/100k_ascii.txt", 10);
	TestFromFile<utf8, utf16>("10MB utf8->utf16", "res/conversion/100k_ascii.txt", 100);
}


TEST_CASE("utf8->esc6") {


}

