#pragma once

#include "../EmpireException.h"
#include "../buffer/Buffer.h"
#include "../primitive/EmpirePrimitives.h"

typedef void(*SerializeFunction)(void* value, Empire::Buffer& buf EMPIRE_ERROR_CODE_PARAMETER);
#define DEFAULT_SERIALIZE_TABLE_SIZE 256

namespace Empire {

class Serializer {
public:
	Serializer();

	void AddSerializer(EmpireType type, SerializeFunction function EMPIRE_ERROR_CODE_PARAMETER);

	inline void SerializeNamed(EmpireType type, void* value, Buffer& buf EMPIRE_ERROR_CODE_PARAMETER) {
		// Variable length encode name into buf
		SerializeUnnamed(type, value, buf EMPIRE_ERROR_CODE_VAR);
	}
	inline void SerializeUnnamed(EmpireType type, void* value, Buffer& buf EMPIRE_ERROR_CODE_PARAMETER) {
		if (type >= serializersSize) {
			EMPIRE_ERROR(EMPIRE_UNKNOWN_TYPE, new UnknownTypeErrorData(type), EMPIRE_VOID_FUNCTION);
		}
		serializers[type](value, buf EMPIRE_ERROR_CODE_PARAMETER);
	}

	~Serializer();

private:
	SerializeFunction* serializers;
	EmpireType serializersSize;
};



}