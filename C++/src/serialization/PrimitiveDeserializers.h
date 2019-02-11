#pragma once

#include "Deserializer.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"

//The template functions serve as entry points for serializing primitive dests
namespace Empire {


__forceinline void Deserialize_U8(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}
__forceinline void Deserialize_S8(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
	
}

__forceinline void Deserialize_U16(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}
__forceinline void Deserialize_S16(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}

__forceinline void Deserialize_U32(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}
__forceinline void Deserialize_S32(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}

__forceinline void Deserialize_U64(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}
__forceinline void Deserialize_S64(void* dest, Input& buf EMPIRE_ERROR_PARAMETER) {
		
}



//TODO
/*
template<>
__forceinline void Serializer::Write<u256>(u256 dest, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_256_TYPE, &dest, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s256>(s256 dest, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_256_TYPE, &dest, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<bigint>(bigint dest, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_BIG_INT_TYPE, &dest, buf EMPIRE_ERROR_VAR);
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