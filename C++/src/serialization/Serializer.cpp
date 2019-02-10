#include "Serializer.h"

#include <memory>

#include "../EmpireErrorCodes.h"
#include "../type/EmpireTypes.h"
#include "PrimitiveSerializers.h"

namespace Empire {

Serializer::Serializer() {
	serializersSize = DEFAULT_SERIALIZE_TABLE_SIZE;
	serializers = new SerializeFunction[serializersSize]{};//Zero it out
	InitSerializers();
}

Serializer::~Serializer() {
	delete serializers;
}

void Serializer::AddSerializer(EmpireType type, SerializeFunction function EMPIRE_ERROR_PARAMETER) {
	if (type >= serializersSize) {
		EmpireType oldSize = serializersSize;
		EmpireType newSize = serializersSize;
		while (newSize < type) {
			newSize *= 2;//Double it until its big enough
		}
		SerializeFunction* newSerializers = new SerializeFunction[newSize];
		SerializeFunction* oldSerializers = serializers;
		std::memcpy(newSerializers, oldSerializers, sizeof(SerializeFunction) * oldSize);
		//Zero out the remaining indices
		std::memset(newSerializers + oldSize, 0x00, sizeof(SerializeFunction) * (newSize - oldSize));

		serializers = newSerializers;
		serializersSize = newSize;
		delete oldSerializers;
	}
	if (serializers[type] != nullptr) {
		EMPIRE_ERROR(EMPIRE_ALREADY_KNOWN_TYPE, new AlreadyKnownTypeData(type), EMPIRE_VOID_FUNCTION);
	} else {
		serializers[type] = function;
	}
}




void Serializer::InitSerializers() {
	EmpireError error;//Dont bother checking error since we know we will never duplicate these serializers
	AddSerializer(EMPIRE_UINT_8_TYPE, &Serialize_U8 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_UINT_16_TYPE, &Serialize_U16 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_UINT_32_TYPE, &Serialize_U32 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_UINT_64_TYPE, &Serialize_U64 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_UINT_128_TYPE, &Serialize_U128 EMPIRE_IF_ERROR_CODES(error));

	AddSerializer(EMPIRE_SINT_8_TYPE, &Serialize_S8 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_SINT_16_TYPE, &Serialize_S16 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_SINT_32_TYPE, &Serialize_S32 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_SINT_64_TYPE, &Serialize_S64 EMPIRE_IF_ERROR_CODES(error));
	AddSerializer(EMPIRE_SINT_128_TYPE, &Serialize_S128 EMPIRE_IF_ERROR_CODES(error));
}

}//namespace