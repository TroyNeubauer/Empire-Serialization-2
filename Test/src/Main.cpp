
#include <EmpireSerialization2.h>


#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

int main(int argc, char* const argv[]) {
	int result = Catch::Session().run(argc, argv);

#ifdef _MSC_VER
	system("PAUSE");
#endif

	return result;
}


#include "FormatTest.h"
#include "BufferTest.h"
#include "ConversionsTest.h"
#include "InternalTest.h"

