
#include <fstream>
#include <iostream>

#include <EmpireSerialization/Conversions.h>

using namespace ES;

//Assume the source file is always utf8
template<typename SrcType, typename DestType>
void TestFromFile(const char* benchName, const char* fileName)
{
	std::ifstream file(fileName);
	REQUIRE(file.good());
	
	std::vector<utf8> fileChars((std::istreambuf_iterator<utf8>(file)), std::istreambuf_iterator<utf8>());
	std::size_t characters = fileChars.size();
	std::cout << "chars " << characters << std::endl;

	std::vector<SrcType> src(characters);
	std::vector<DestType> dest(characters);
	
	//We can use the file directly
	if (std::is_same<SrcType, utf8>::value)
	{
		std::copy(fileChars.begin(), fileChars.end(), src.data());
	}
	else
	{
		//We need to convert from the utf8 file to the source type
		StringCodingData data;
		REQUIRE(Conversions::Convert<utf8, SrcType>(fileChars.data(), characters, src.data(), characters, data) == ErrorCode::NONE);
		REQUIRE(data.Characters == characters);
	}
	
    BENCHMARK(benchName)
	{
		StringCodingData data;
		ErrorCode error = Conversions::Convert<SrcType, DestType>(src.data(), characters, dest.data(), characters, data);
		REQUIRE(error == ErrorCode::NONE);
		REQUIRE(data.WordsRead == characters);
		REQUIRE(data.Characters == characters);
		return error;
    };

}

TEST_CASE("to utf16") {

	TestFromFile<utf8, utf32>("25000 utf8->utf16", "res/conversion/25000_ASCII.txt");

}

