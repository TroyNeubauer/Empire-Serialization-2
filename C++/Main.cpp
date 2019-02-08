#include <iostream>
#include <time.h>
#include <Windows.h>

#include "Empire.h"
#include "Test.h"
#include "half.h"

#include "TempBuffer.h"


using namespace Empire;


//Full length string - fills 128 bits
//ABCDEFABCDEFABCD1122334455667788
int main() {
	system("PAUSE");
	return 0;
	float array[] = { 0.0f, 292.23f, 2.2323f, 13.2387f, 278.2f, 27.5f, -1.24f, -45.46f, -0.4578134f, 0.0000001f};
	const int elements = sizeof(array) / sizeof(float);
	half result[elements];

	FloatsToHalves(result, array, elements);
	for (int i = 0; i < elements; i++) {
		half h = result[i];
		for (int j = 8 * sizeof(half) - 1; j >= 0; j--) {
			std::cout << ((h.value >> j) & 0x01);
		}
		std::cout << std::endl;
	}//0001110111
	float dest[elements];
	HalvesToFloats(dest, result, elements);
	


	system("PAUSE");
}
