#include "Serializer.h"
#include "../type/EmpireTypes.h"
#include "../primitive/EmpirePrimitives.h"
#include "../primitive/int128_t.h"
#include "../primitive/uint128_t.h"

//These functions serve as entry points for serializing primitive values
namespace Empire {


template<>
__forceinline void Serializer::Write<u8>(u8 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_8_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s8>(s8 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_8_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u16>(u16 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_16_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s16>(s16 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_16_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u32>(u32 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_32_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s32>(s32 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_32_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u64>(u64 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_64_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s64>(s64 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_64_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<u128>(u128 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_UINT_128_TYPE, &value, buf EMPIRE_ERROR_VAR);
}
template<>
__forceinline void Serializer::Write<s128>(s128 value, Buffer& buf EMPIRE_ERROR_PARAMETER) {
	SerializeNamed(EMPIRE_SINT_128_TYPE, &value, buf EMPIRE_ERROR_VAR);
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