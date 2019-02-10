#pragma once

#include "../EmpireException.h"
#include "../buffer/Output.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"
#include "../primitive/int128_t.h"
#include "../primitive/uint128_t.h"

#define DEFAULT_SERIALIZE_TABLE_SIZE 256

namespace Empire {

typedef void(*SerializeFunction)(void* value, Output& buf EMPIRE_ERROR_PARAMETER);

class Serializer {
public:
	Serializer();

	template<typename T>
	void Write(T value, Output& buf EMPIRE_ERROR_PARAMETER);

	void AddSerializer(EmpireType type, SerializeFunction function EMPIRE_ERROR_PARAMETER);

	inline void SerializeNamed(EmpireType type, void* value, Output& buf EMPIRE_ERROR_PARAMETER) {
		buf.WriteVLE(type);
		SerializeUnnamed(type, value, buf EMPIRE_ERROR_VAR);
	}
	inline void SerializeUnnamed(EmpireType type, void* value, Output& buf EMPIRE_ERROR_PARAMETER) {
		if (type >= serializersSize || serializers[type] == nullptr) {
			EMPIRE_ERROR(EMPIRE_UNKNOWN_TYPE, new UnknownTypeErrorData(type), EMPIRE_VOID_FUNCTION);
		}
		serializers[type](value, buf EMPIRE_ERROR_VAR);
	}

	~Serializer();
protected:
	void InitSerializers();

private:
	SerializeFunction* serializers;
	EmpireType serializersSize;
};

template<>
__forceinline void Serializer::Write<u8>(u8 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_8_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s8>(s8 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_8_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u16>(u16 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_16_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s16>(s16 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_16_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u32>(u32 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_32_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s32>(s32 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_32_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u64>(u64 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_64_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u128>(u128 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_128_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s128>(s128 value, Output& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_128_TYPE, &value, buf EMPIRE_ERROR_VAR);
}








}