
#include <EmpireSerialization2.h>



#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>

int main(int argc, char* const argv[]) {
	int result = Catch::Session().run(argc, argv);

#ifdef _MSC_VER
	system("PAUSE");
#endif
}


#include "BufferTest.h"

