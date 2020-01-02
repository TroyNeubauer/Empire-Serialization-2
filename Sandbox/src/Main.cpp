#include <iostream>
#include <cstdio>
#include <exception>
#include <vector>
#include <typeinfo>
#include <random>
#include <map>
#include <unordered_map>
#include <cstdio>

#include <EmpireSerialization2.h>
#include <EmpireSerialization/Conversions.h>
#include <Internal.h>

struct Point {
	int x, y;
};

struct BigClass
{
	std::unordered_map<int, std::vector<Point>> m_BigMap;
	int m_RandInt;
	Point m_DefaultPoint;
};

int main()
{
	char in[] = "testa";
	ES::esc6 dest[100];
	ES::StringCodingData data;
	if (ES::Conversions::Convert<ES::utf8, ES::esc6>(in, std::strlen(in) + 1, dest, sizeof(dest), data))
	{
		ES::DefaultFormatter formatter;
		ES::ToString::PrintError(formatter, ES::GetError());
		printf("Error converting string %s", formatter.c_str());
	}
	for (int i = 0; i < data.Bytes; i++)
	{
		printf("%d (%c)\n", dest[i], dest[i]);
	}

	/*Type point = Type::CreateClass("Point",
	{ 
		{ BuiltinTypes::INT, "x", offsetof(Point, x) }, 
		{ BuiltinTypes::INT, "y", offsetof(Point, y) },
	});

	Type pointVector = Type::CreateList(point);
	Type myMap = Type::CreateMap(BuiltinTypes::INT, pointVector);
	
	Type bigClass = Type::CreateClass("BigClass", { 
		{ myMap, "m_BigMap", offsetof(BigClass, m_BigMap) } , 
		{ point, "m_DefaultPoint", offsetof(BigClass, m_DefaultPoint) },
		{ BuiltinTypes::INT, "m_RandInt", offsetof(BigClass, m_RandInt) },
	});
	std::cout << bigClass;
	std::cin.get();*/
}



void VLETest() {
	/*BufferedOutput out(1024);
	const int SIZE = 1000000;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist((std::numeric_limits<int>::min)(), (std::numeric_limits<int>::max)());

	int* array = new int[SIZE];
	for (int i = 0; i < SIZE; i++) {
		array[i] = dist(rng);
	}

	Timer timer;
	for (int i = 0; i < SIZE; i++) {
		out.WriteVLE(array[i]);
	}
	timer.Stop().Print("Printing 1000000 vle ints took");

	FILE* f = fopen("test.dat", "wb");
	fwrite(out.Begin(), 1, out.Offset(), f);
	fclose(f);

	EmpireError empireError;
	FileInput in("test.dat" EMPIRE_ERROR_VAR, FileOptions::MAPPED);
	//if (empireError) DebugBreak();

	while (in.CanRead()) {
		int value = in.ReadVLE<int>(empireError);
		std::cout << value << std::endl;
	}
	int x = in.ReadVLE<int>(empireError);*/
}
