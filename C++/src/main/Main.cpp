#include <iostream>
#include <stdio.h>
#include <exception>
#include <vector>
#include <typeinfo>

#include "../Empire.h"
#include "../type/TypeManager.h"

using namespace Empire;

int main() {

	using s = std::make_signed<u128>::type;
	s a = 10;

	/*f128 f;
	double d = 0.12378;
	f = d;
	float128 b = 23784234.23478234;
	f += b;
	//std::cout << "lf " << b << std::endl;*/
	system("PAUSE");
}

