#pragma once

#include "Deserializer.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"
#include "../primitive/float16.h"

//The template functions serve as entry points for serializing primitive dests
namespace Empire {


__forceinline void Deserialize_U8(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(u8) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_S8(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(s8) EMPIRE_ERROR_VAR);
}

__forceinline void Deserialize_U16(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(u16) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_S16(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(s16) EMPIRE_ERROR_VAR);
}

__forceinline void Deserialize_U32(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(u32) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_S32(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(s32) EMPIRE_ERROR_VAR);
}

__forceinline void Deserialize_U64(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(u64) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_S64(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(s64) EMPIRE_ERROR_VAR);
}

__forceinline void Deserialize_F16(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(f16) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_F32(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(f32) EMPIRE_ERROR_VAR);
}
__forceinline void Deserialize_F64(void* dest, Input& input EMPIRE_ERROR_PARAMETER) {
	input.Read(dest, sizeof(f64) EMPIRE_ERROR_VAR);
}



//TODO
/*

template<>
__forceinline void Serializer::Write<bigint>(bigint dest, Buffer& input EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_BIG_INT_TYPE, &dest, input EMPIRE_ERROR_VAR);
}

*/


	//#################### FLOATS ####################
#define EMPIRE_FLOAT_16_TYPE	13	
#define EMPIRE_FLOAT_32_TYPE	14

#define EMPIRE_FLOAT_64_TYPE	15
#define EMPIRE_FLOAT_128_TYPE	16

#define EMPIRE_FLOAT_256_TYPE	17
#define EMPIRE_BIG_FLOAT_TYPE	18

}