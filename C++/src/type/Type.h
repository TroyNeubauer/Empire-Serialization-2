#pragma once

#include <vector>
#include <string>

#include "../primitive/EmpirePrimitives.h"

namespace Empire {

	class Type;

	struct TypeMember {
		const std::string Name;
		const Type& Type;
	};

	class Type {
	public:
		Type(std::string name, std::initializer_list<TypeMember> members) : m_Primitive(false), m_Name(name), m_Members(members) {}
		Type(std::string name, u64 id) : m_Primitive(true), m_Name(name), m_ID(id), m_Members(0) {}

		inline bool IsPrimitive() const { return m_Primitive; }
		inline u64 GetPrimitiveID() const { return m_ID; }
		inline const std::vector<TypeMember>& GetMembers() const { return m_Members; }
		inline const std::string& GetName() const { return m_Name; }

	private:
		const bool m_Primitive;
		const u64 m_ID;//The id of the type. Only known if m_Primitive is true

		const std::string m_Name;
		const std::vector<TypeMember> m_Members;

	};

	class BuiltinTypes {
		static const u64
			INVALID_ID = 0,
			BIT_FIELD_ID = 0,
			S8_ID = 1,
			U8_ID = 2,
			S16_ID = 3,
			U16_ID = 4,
			S32_ID = 5,
			U32_ID = 6,
			S64_ID = 7,
			U64_ID = 8,
			S128_ID = 9,
			U128_ID = 10,
			S256_ID = 11,
			U256_ID = 12,
			BIG_INTEGER_ID = 13,

			F8_ID = 16,
			F16_ID = 17,
			F32_ID = 18,
			F64_ID = 19,
			F128_ID = 20,
			BIG_FLOAT_ID = 21,

			UTF8_STRING_ID = 24,
			UTF16_STRING_ID = 25,
			UTF32_STRING_ID = 26,
			ESC4_STRING_ID = 27,
			ESC6_STRING_ID = 28,
			ESC8_PLUS_STRING_ID = 29;

		inline static const Type 
			INVALID = { "Invalid", INVALID_ID },
			S8 = {"S8", S8_ID},
			U8 = { "U8", U8_ID },
			S16 = { "S16", S16_ID },
			U16 = { "U16", U16_ID },
			S32 = { "S32", S32_ID },
			U32 = { "U32", U32_ID },
			S64 = { "S64", S64_ID },
			U64 = { "U64", U64_ID },
			S128 = { "S128", S128_ID },
			U128 = { "U128", U128_ID },
			S256 = { "S256", S256_ID },
			U256 = { "U256", U256_ID },
			BIG_INTEGER = { "BigInteger", BIG_INTEGER_ID },

			F8 = { "F8", F8_ID },
			F16 = { "F16", F16_ID },
			F32 = { "F32", F32_ID },
			F64 = { "F64", F64_ID },
			F128 = { "F128", F128_ID },
			BIG_FLOAT = { "BigFloat", BIG_FLOAT_ID },

			UTF8_STRING = { "UTF8-String", UTF8_STRING_ID},
			UTF16_STRING = { "UTF16-String", UTF16_STRING_ID},
			UTF32_STRING = { "UTF32-String", UTF32_STRING_ID},
			ESC4_STRING = { "ESC4-String", ESC4_STRING_ID },
			ESC6_STRING = { "ESC6-String", ESC6_STRING_ID},
			ESC8_PLUS_STRING = { "ESC8+-String", ESC8_PLUS_STRING_ID };

		//Returns a reference to the primitive type with the desired ID. If id doesnt represent a primitive type this function returns BuiltinTypes::INVALID
		static const Type& Get(u64 id);

	};



}

