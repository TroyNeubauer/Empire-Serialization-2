#pragma once

#include "Serializer.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"
#include "../primitive/int128_t.h"
#include "../primitive/uint128_t.h"

//The template functions serve as entry points for serializing primitive values
namespace Empire {


__forceinline void Serialize_U8(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<u8*>(value));
}


__forceinline void Serialize_S8(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<s8*>(value));
}


__forceinline void Serialize_U16(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<u16*>(value));
}


__forceinline void Serialize_S16(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<s16*>(value));
}


__forceinline void Serialize_U32(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<u32*>(value));
}


__forceinline void Serialize_S32(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<s32*>(value));
}


__forceinline void Serialize_U64(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<u64*>(value));
}


__forceinline void Serialize_S64(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<s64*>(value));
}


__forceinline void Serialize_U128(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<u128*>(value));
}


__forceinline void Serialize_S128(void* value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	buf.Write(*static_cast<s128*>(value));
}


//TODO
/*
template<>
__forceinline void Serializer::Write<u256>(u256 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_256_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s256>(s256 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_256_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<bigint>(bigint value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_BIG_INT_TYPE, &value, buf EMPIRE_ERROR_VAR);
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