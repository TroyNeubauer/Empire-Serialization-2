#include <iostream>
#include <stdio.h>

#include "../Empire.h"


int main() {
	Serializer s;
	Buffer buf(1);
	buf.WriteVLE(0);
	buf.WriteVLE(0x1F);
	buf.WriteVLE(0);
	buf.WriteVLE(0b101010101010101010101010UL);

	/*
	EmpireError error;
	int x = 0;
	while(x != -1) {
		std::cin >> x;
		EmpireError error;
		s.Write(x, buf, error);
		if (error) {
			throw ErrorToException(error);
		}
	}*/
	FILE* file = fopen("out.dat", "wb");
	if (file) {
		fwrite(buf.GetStart(), 1, buf.Offset(), file);
	}
	fclose(file);
	
	
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