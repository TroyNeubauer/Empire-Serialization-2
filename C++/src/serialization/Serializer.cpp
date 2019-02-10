#include "Serializer.h"

#include <memory>

#include "../EmpireErrorCodes.h"

namespace Empire {

Serializer::Serializer() {
	serializersSize = DEFAULT_SERIALIZE_TABLE_SIZE;
	serializers = new SerializeFunction[serializersSize]{};//Zero it out
}

Serializer::~Serializer() {
	delete serializers;
}

void Serializer::AddSerializer(EmpireType type, SerializeFunction function EMPIRE_ERROR_CODE_PARAMETER) {
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


}