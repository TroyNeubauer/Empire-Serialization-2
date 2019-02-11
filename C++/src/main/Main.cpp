#include <iostream>
#include <stdio.h>
#include <exception>

#include "../Empire.h"

using namespace Empire;

int main() {
	Serializer s;
	EmpireError error;
	Output out;
	std::cout  << std::hex << reinterpret_cast<u64>(out.GetPointer()) << std::endl;

	s.Write(6, out, error);
	
	FILE* file = fopen("out.dat", "wb");
	if (file) {
		fwrite(out.GetStart(), 1, out.Offset(), file);
	}
	fclose(file);


	system("PAUSE");
}




















void floatTest() {
	float array[] = { 0.0f, 292.23f, 2.2323f, 13.2387f, 278.2f, 27.5f, -1.24f, -45.46f, -0.4578134f, 0.0000001f };
	const int elements = sizeof(array) / sizeof(float);
	half result[elements];

	FloatsToHalves(result, array, elements);
	for (int i = 0; i < elements; i++) {
		half h = result[i];
		for (int j = 8 * sizeof(half) - 1; j >= 0; j--) {
			std::cout << ((h.m_Value >> j) & 0x01);
		}
		std::cout << std::endl;
	}//0001110111
	float dest[elements];
	HalvesToFloats(dest, result, elements);
	std::cout << std::endl << std::endl;
	for (int i = 0; i < elements; i++) {
		std::cout << dest[i] << ", ";
	}

	system("PAUSE");
}