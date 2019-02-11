#include "Deserializer.h"

#include <memory>

#include "../EmpireErrorCodes.h"
#include "../type/EmpireTypes.h"
#include "PrimitiveDeserializers.h"

namespace Empire {

Deserializer::Deserializer() {
	deserializersSize = DEFAULT_DESERIALIZE_TABLE_SIZE;
	deserializers = new DeserializeFunction[deserializersSize]{};//Zero it out
	InitDeserializers();
}

Deserializer::~Deserializer() {
	delete deserializers;
}

void Deserializer::AddDeserializer(EmpireType type, DeserializeFunction function EMPIRE_ERROR_PARAMETER) {
	if (type >= deserializersSize) {
		EmpireType oldSize = deserializersSize;
		EmpireType newSize = deserializersSize;
		while (newSize < type) {
			newSize *= 2;//Double it until its big enough
		}
		DeserializeFunction* newSerializers = new DeserializeFunction[newSize];
		DeserializeFunction* oldSerializers = deserializers;
		std::memcpy(newSerializers, oldSerializers, sizeof(DeserializeFunction) * oldSize);
		//Zero out the remaining indices
		std::memset(newSerializers + oldSize, 0x00, sizeof(DeserializeFunction) * (newSize - oldSize));

		deserializers = newSerializers;
		deserializersSize = newSize;
		delete oldSerializers;
	}
	if (deserializers[type] != nullptr) {
		EMPIRE_ERROR(EMPIRE_ALREADY_KNOWN_TYPE, new AlreadyKnownTypeData(type), EMPIRE_VOID_FUNCTION);
	} else {
		deserializers[type] = function;
	}
}


void Deserializer::InitDeserializers() {
	EmpireError error;//Dont bother checking error since we know we will never duplicate these serializers
	AddDeserializer(EMPIRE_UINT_8_TYPE, &Deserialize_U8 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_UINT_16_TYPE, &Deserialize_U16 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_UINT_32_TYPE, &Deserialize_U32 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_UINT_64_TYPE, &Deserialize_U64 EMPIRE_IF_ERROR_CODES(error));

	AddDeserializer(EMPIRE_SINT_8_TYPE, &Deserialize_S8 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_SINT_16_TYPE, &Deserialize_S16 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_SINT_32_TYPE, &Deserialize_S32 EMPIRE_IF_ERROR_CODES(error));
	AddDeserializer(EMPIRE_SINT_64_TYPE, &Deserialize_S64 EMPIRE_IF_ERROR_CODES(error));
}

}//namespace