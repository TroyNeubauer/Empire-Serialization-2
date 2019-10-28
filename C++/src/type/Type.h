#pragma once

#include <variant>
#include <array>
#include <iostream>
#include <vector>
#include <limits>

#include "../primitive/EmpirePrimitives.h"
#undef max
#undef min

namespace Empire {

	struct TypeMember;
	struct SequenceData;

	class Type;

	class BuiltinTypes
	{
	public:
		static inline const u64
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

		static const Type
			VIRTUAL, INVALID,
			S8, U8, S16, U16, S32, U32, S64, U64, S128, U128, S256, U256, BIG_INTEGER,

	//F8, F16, F32, F32, F64, F128, BIG_FLOAT,

			UTF8_STRING, UTF16_STRING, UTF32_STRING, ESC4_STRING, ESC6_STRING, ESC8_PLUS_STRING;

		static const Type& FLOAT;
		static const Type& DOUBLE;
		static const Type& BYTE;
		static const Type& SHORT;
		static const Type& INT;
		static const Type& LONG_LONG;

		static const Type& UBYTE;
		static const Type& USHORT;
		static const Type& UINT;
		static const Type& ULONG_LONG;

		//Returns a reference to the primitive type with the desired ID. If id doesnt represent a primitive type this function returns BuiltinTypes::INVALID
		static const Type& Get(u64 id);

	};

	struct SequenceData
	{
		const Type* First;
		const Type* Second;
		u32 Size;

		SequenceData() = default;
		SequenceData(SequenceData& other) = default;
		SequenceData(SequenceData&& other) = default;
		
		SequenceData& operator=(const SequenceData& other) = default;
		SequenceData& operator=(SequenceData&& other) = default;

		SequenceData(const Type& listType);
		SequenceData(const Type& key, const Type& value);

		bool operator==(const SequenceData& other) const;
	};

	std::ostream& operator<<(std::ostream& out, const SequenceData& type);

	struct TypeMember
	{
		TypeMember();
		TypeMember(const Type& type, std::string name, u32 offset);

		inline static constexpr u32 UNKNOWN_OFFSET = std::numeric_limits<u32>::max();

		bool operator==(const TypeMember& other);
		
		const Type* MemberType;
		const std::string Name;
		const u32 Offset;

	};

	class Type
	{
	private:
		Type() : m_Name("Invalid"), m_Size(UNKNOWN_SIZE) {}
		Type(const char* name, u32 size, u64 id);
		Type(const char* name, u32 size, std::initializer_list<TypeMember> members);
		Type(const Type& listType, bool unused);
		Type(const Type& key, const Type& value);
	public:

		Type& operator=(const Type& other) = delete;
		//Type& operator=(const Type&& other);

		bool operator==(const Type& other) const;

		static Type CreatePrimitive(const char* name, u32 size, u64 id);

		static Type CreateClass(const char* name, std::initializer_list<TypeMember> members);
		static Type CreateList(const Type& listType);
		static Type CreateMap(const Type& key, const Type& value);

		inline bool IsPrimitive()		const { return m_Data.index() == 0; }
		inline bool IsNormalObject()	const { return m_Data.index() == 1; }
		inline bool IsSequence()		const { return m_Data.index() == 2; }

		inline const char* GetName() const { return m_Name; }

		inline u64 GetPrimitiveID() const { return std::get<u64>(m_Data); }
		inline const std::vector<TypeMember>& GetMembers() const { return std::get<std::vector<TypeMember>>(m_Data); }
		const inline SequenceData& GetSequence() const { return std::get<SequenceData>(m_Data); }

		inline static constexpr u32 UNKNOWN_SIZE = std::numeric_limits<u32>::max();

		inline bool IsConstantSize() const { return m_Size != UNKNOWN_SIZE; }
		inline u32 GetSize() const { return m_Size; }

		

	private:
		char m_Name[256];
		//Can either be primitive (and have an id), be a normal object (and have members), or be a sequence (and have 1 or more component types)
		std::variant<u64, std::vector<TypeMember>, SequenceData> m_Data;
		u32 m_Size;

		friend class BuiltinTypes;
	};

	std::ostream& operator<<(std::ostream& out, const Type& type);

}

