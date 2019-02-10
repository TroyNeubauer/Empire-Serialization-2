#pragma once

#include "../EmpireException.h"
#include "../buffer/Buffer.h"
#include "../primitive/EmpirePrimitives.h"

#define DEFAULT_SERIALIZE_TABLE_SIZE 256

namespace Empire {

typedef void(*SerializeFunction)(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER);

class Serializer {
public:
	Serializer();

	template<typename T>
	void Write(T value, Buffer& buf EMPIRE_ERROR_PARAMETER);

	void AddSerializer(EmpireType type, SerializeFunction function EMPIRE_ERROR_PARAMETER);

	inline void SerializeNamed(EmpireType type, void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
		// Variable length encode name into buf
		SerializeUnnamed(type, value, buf EMPIRE_ERROR_VAR);
	}
	inline void SerializeUnnamed(EmpireType type, void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
		if (type >= serializersSize || serializers[type] == nullptr) {
			EMPIRE_ERROR(EMPIRE_UNKNOWN_TYPE, new UnknownTypeErrorData(type), EMPIRE_VOID_FUNCTION);
		}
		serializers[type](value, buf EMPIRE_ERROR_VAR);
	}

	~Serializer();

private:
	SerializeFunction* serializers;
	EmpireType serializersSize;
};



}