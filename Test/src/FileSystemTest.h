#include <stdio.h>
#include <iostream>
#include <fstream>

TEST_CASE("FileSystem::CreateDirectory", "[filesystem]") {
	REQUIRE(Empire::FileSystem::CreateDirectory("./test/"));
}

TEST_CASE("FileSystem::CreateDirectories", "[filesystem]") {

	REQUIRE(Empire::FileSystem::CreateDirectories("test/test2/test3/"));
}

TEST_CASE("FileSystem::Delete", "[filesystem]") {
	char fileName[] = "./test/test.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	Empire::FileSystem::Delete(fileName);
	REQUIRE(fopen(fileName, "rb") == nullptr);
	
}

TEST_CASE("FileSystem::FileLength", "[filesystem]") {
	char fileName[] = "./test/length.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	REQUIRE(Empire::FileSystem::FileSize(fileName) == sizeof(fileName));
	Empire::FileSystem::Delete(fileName);
}

TEST_CASE("FileSystem::TruncateFile", "[filesystem]") {
	char fileName[] = "./test/length.dat";
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
	REQUIRE(fclose(f) == 0);
	REQUIRE(Empire::FileSystem::TruncateFile(fileName));
	REQUIRE(Empire::FileSystem::FileSize(fileName) == 0);
	Empire::FileSystem::Delete(fileName);
	
	//test TruncateFile creating non existing files
	REQUIRE(Empire::FileSystem::TruncateFile("./test/file_that_will_never_exist.dat"));
	REQUIRE(Empire::FileSystem::Delete("./test/file_that_will_never_exist.dat"));
}

TEST_CASE("FileSystem::CreateFile", "[filesystem]") {
	char fileName[] = "./test/test.dat";
	Empire::FileSystem::Delete(fileName);
	REQUIRE(Empire::FileSystem::CreateFile(fileName));
	FILE* f = fopen(fileName, "wb");
	REQUIRE(f != nullptr);
	fclose(f);
}


TEST_CASE("FileSystem::MapFile", "[filesystem]") {
	SECTION("Memory map test file") {
		char fileName[] = "./test/test.dat";
		FILE* f = fopen(fileName, "wb");
		REQUIRE(f != nullptr);
		REQUIRE(fwrite(fileName, 1, sizeof(fileName), f) == sizeof(fileName));
		REQUIRE(fclose(f) == 0);
		u64 length;
		Empire::EmpireError EMPIRE_ERROR_VAR1;
		void* data = Empire::FileSystem::MapFile(fileName, Empire::FileOpenOptions::READ, length EMPIRE_ERROR_VAR, 0, Empire::FileSystem::ENTIRE_FILE);
		REQUIRE(data != nullptr);
		REQUIRE(Empire::StringUtils::Equal(fileName, (const char*)data));
		Empire::FileSystem::UnmapFile(data);
	}
	SECTION("Memory map 0 length file") {
		Empire::FileSystem::CreateFile("./test/empty_file.dat");
		u64 length;
		Empire::EmpireError EMPIRE_ERROR_VAR1;
		REQUIRE_THROWS(Empire::FileSystem::MapFile("./test/empty_file.dat", Empire::FileOpenOptions::READ, length EMPIRE_ERROR_VAR, 0, Empire::FileSystem::ENTIRE_FILE));
		Empire::FileSystem::Delete("./test/empty_file.dat");
	}
}
