#pragma once

#include <functional>

#include "../primitive/EmpirePrimitives.h"
#include "../buffer/Buffer.h"


namespace Empire {


template<typename T>
class Field {

private:
	u32 offset, length;
	const char* name;
	u32 nameLength;


	std::function<T(Buffer)> deserialize;
	std::function<void(T, Buffer)> serialize;


};

}