#pragma once


#include "../EmpireException.h"
#include "../buffer/Input.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"

#define DEFAULT_DESERIALIZE_TABLE_SIZE 256

namespace Empire {

typedef void(*DeserializeFunction)(void* dest, Input& input EMPIRE_ERROR_PARAMETER);

class Deserializer {
public:
	Deserializer();

	template<typename T>
	T Read(Input& input EMPIRE_ERROR_PARAMETER);

	void AddDeserializer(EmpireType type, DeserializeFunction function EMPIRE_ERROR_PARAMETER);

	inline void DeserializeNamed(EmpireType type, void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
		EmpireType storedType = input.ReadVLE<EmpireType>(EMPIRE_ERROR_VAR1);
		DeserializeUnnamed(type, dest, input EMPIRE_ERROR_VAR);
	}
	inline void DeserializeUnnamed(EmpireType type, void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
		if (type >= deserializersSize || deserializers[type] == nullptr) {
			EMPIRE_ERROR(EMPIRE_UNKNOWN_TYPE, new UnknownTypeErrorData(type), EMPIRE_VOID_FUNCTION);
		}
		deserializers[type](dest, input EMPIRE_ERROR_VAR);
	}

	~Deserializer();
protected:
	void InitDeserializers();

private:
	DeserializeFunction* deserializers;
	EmpireType deserializersSize;
};

template<>
__forceinline u8 Deserializer::Read<u8>(Input& input EMPIRE_ERROR_PARAMETER) {
	u8 result;
	DeserializeNamed(EMPIRE_UINT_8_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline s8 Deserializer::Read<s8>(Input& input EMPIRE_ERROR_PARAMETER) {
	u8 result;
	DeserializeNamed(EMPIRE_SINT_8_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline u16 Deserializer::Read<u16>(Input& input EMPIRE_ERROR_PARAMETER) {
	u16 result;
	DeserializeNamed(EMPIRE_UINT_16_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline s16 Deserializer::Read<s16>(Input& input EMPIRE_ERROR_PARAMETER) {
	s16 result;
	DeserializeNamed(EMPIRE_SINT_16_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline u32 Deserializer::Read<u32>(Input& input EMPIRE_ERROR_PARAMETER) {
	u32 result;
	DeserializeNamed(EMPIRE_UINT_32_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline s32 Deserializer::Read<s32>(Input& input EMPIRE_ERROR_PARAMETER) {
	s32 result;
	DeserializeNamed(EMPIRE_SINT_32_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline u64 Deserializer::Read<u64>(Input& input EMPIRE_ERROR_PARAMETER) {
	u64 result;
	DeserializeNamed(EMPIRE_UINT_64_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}
template<>
__forceinline s64 Deserializer::Read<s64>(Input& input EMPIRE_ERROR_PARAMETER) {
	s64 result;
	DeserializeNamed(EMPIRE_SINT_64_TYPE, &result, input EMPIRE_ERROR_VAR);
	return result;
}


}//namespace