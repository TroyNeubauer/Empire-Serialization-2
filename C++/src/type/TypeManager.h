#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <stdint.h>

#include "../primitive/EmpirePrimitives.h"

#define CreatePointerField(type, parent, name) TypeData(sizeof(type), offsetof(parent, name), #name, PutTypeEnum(POINTER))
#define CreatePODField(type, parent, name) TypeData(sizeof(type), offsetof(parent, name), #name, PutTypeEnum(POD))

#define CreateClass(type, ...) TypeData(sizeof(type), UINT32_MAX, #type, std::vector<TypeData>{ ##__VA_ARGS__}, PutTypeEnum(CLASS) )

#define TYPE_FLAGS_POINTER 0b01
#define TYPE_FLAGS_ 0b01


namespace Empire {

enum TypeEnum {
	POD = 0, POINTER = 1, REFERENCE = 2, CLASS = 3
};
#define TYPE_ENUM_MASK 0b11
#define TYPE_ENUM_SHIFT 0

#define GetTypeEnum(flags) ( (TypeEnum) ((flags >> TYPE_ENUM_SHIFT) & TYPE_ENUM_MASK) )
#define PutTypeEnum(value) (value << TYPE_ENUM_SHIFT)

struct TypeData {
public:

	u64 size;
	u32 offset;// Only used for fields
	u32 flags;
	std::string name;
	std::vector<TypeData> fields;
	inline bool IsField() { return offset != UINT32_MAX; };

	TypeData(u64 size, u32 offset, std::string name, u32 flags) : size(size), offset(offset), name(name), flags(flags) {}
	TypeData(u64 size, u32 offset, std::string name, std::vector<TypeData> fields, u32 flags) : size(size), offset(offset), name(name), fields(fields), flags(flags) {}
	std::string ToString();
};



}//namespace
