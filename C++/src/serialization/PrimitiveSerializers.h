#pragma once

#include "Serializer.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"
#include "../primitive/float16.h"

//The template functions serve as entry points for serializing primitive values
namespace Empire {


__forceinline void Serialize_U8(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<u8*>(value));
}
__forceinline void Serialize_S8(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<s8*>(value));
}

__forceinline void Serialize_U16(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<u16*>(value));
}
__forceinline void Serialize_S16(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<s16*>(value));
}

__forceinline void Serialize_U32(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<u32*>(value));
}
__forceinline void Serialize_S32(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<s32*>(value));
}

__forceinline void Serialize_U64(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<u64*>(value));
}
__forceinline void Serialize_S64(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<s64*>(value));
}




//TODO
/*
template<>
__forceinline void Serializer::Write<bigint>(bigint& value, Buffer& output EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_BIG_INT_TYPE, &value, output EMPIRE_ERROR_VAR);
}

*/

__forceinline void Serialize_F16(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(static_cast<f16*>(value)->m_Value);
}

__forceinline void Serialize_F32(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<f32*>(value));
}

__forceinline void Serialize_F64(void* value, Output& output EMPIRE_ERROR_PARAMETER) {
	output.Write(*static_cast<f64*>(value));
}

//TODO
/*
template<>
__forceinline void Serializer::Write<bigfloat>(bigfloat& value, Buffer& output EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_BIG_INT_TYPE, &value, output EMPIRE_ERROR_VAR);
}

*/


}