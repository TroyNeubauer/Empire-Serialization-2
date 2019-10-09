#include <iostream>
#include <stdio.h>
#include <exception>
#include <vector>
#include <typeinfo>
#include <random>

#include <stdio.h>

#include "../Empire.h"

using namespace Empire;

#undef min
#undef max

int main() {

	BufferedOutput out(1024);
	const int SIZE = 1000000;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

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
	if (empireError) DebugBreak();
	
	while (in.CanRead()) {
		int value = in.ReadVLE<int>(empireError);
		std::cout << value << std::endl;
	}
	int x = in.ReadVLE<int>(empireError);
	

}

