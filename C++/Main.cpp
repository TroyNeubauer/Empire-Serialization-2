#include <iostream>
#include <time.h>

#include "Empire.h"
#include "Test.h"

using namespace Empire;

//Full length string - fills 128 bits
//ABCDEFABCDEFABCD1122334455667788
int main() {
	//RunAllTests<u128, 1000>();

	half h = FloatToHalf(0.0f);
	for (int i = sizeof(h) * 8 - 1; i >= 0; i--) {
		int bit = (h.value >> i) & 0x01;
		std::cout << bit;
	
	}

	/*
	u128 a(1, 0);
	u128 b(1, 0);
	std::cout << "a " << a.ToString() << "  b " << b.ToString() << std::endl;
	u128 r = a * b;
	std::cout << " = " << r.ToString() << std::endl;
	*/
	system("PAUSE");
}
