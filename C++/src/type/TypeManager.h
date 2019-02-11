#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <stdint.h>

#include "../primitive/EmpirePrimitives.h"

#define CreateField(type, parent, name) TypeData(sizeof(type), offsetof(parent, name), #name)
#define CreateClass(type, ...) TypeData(sizeof(type), UINT64_MAX, #type, std::vector<TypeData>{ ##__VA_ARGS__} )

namespace Empire {

struct TypeData {
public:
	

	u64 size;
	u64 offset;// Only used for fields
	std::string name;
	std::vector<TypeData> fields;

	TypeData(u64 size, u64 offset, std::string name) : size(size), offset(offset), name(name) {}
	TypeData(u64 size, u64 offset, std::string name, std::vector<TypeData> fields) : size(size), offset(offset), name(name), fields(fields) {}
	std::string ToString();
};



}//namespace
