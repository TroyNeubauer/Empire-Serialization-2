#include <iostream>
#include <stdio.h>
#include <exception>
#include <vector>
#include <typeinfo>

#include <stdio.h>

#include "../Empire.h"
#include "../type/TypeManager.h"


using namespace Empire;

int main() {

	BufferedOutput out;

	using Type = int;
	Type value = 0;
	while (value != -1) {
		std::cin >> value;
		out.WriteVLE(value);
	}

	FILE* f = fopen("./test.dat", "wb");
	fwrite(out.Begin(), 1, out.Offset(), f);
	fclose(f);
	

}

