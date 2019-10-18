#include "Type.h"
#include "../util/StringUtils.h"

#include <limits>


namespace Empire {

	//const Type& BuiltinTypes::FLOAT = BuiltinTypes::F32;
	//const Type& BuiltinTypes::DOUBLE = BuiltinTypes::F64;
	const Type& BuiltinTypes::BYTE = BuiltinTypes::S8;
	const Type& BuiltinTypes::SHORT = BuiltinTypes::S16;
	const Type& BuiltinTypes::INT = BuiltinTypes::S32;
	const Type& BuiltinTypes::LONG_LONG = BuiltinTypes::S64;

	const Type& BuiltinTypes::UBYTE = BuiltinTypes::U8;
	const Type& BuiltinTypes::USHORT = BuiltinTypes::U16;
	const Type& BuiltinTypes::UINT = BuiltinTypes::U32;
	const Type& BuiltinTypes::ULONG_LONG = BuiltinTypes::U64;

	const Type BuiltinTypes::VIRTUAL = { "Virtual", Type::UNKNOWN_SIZE, std::numeric_limits<u64>::max() },//-1 to differentiate itself in the code. Never written to the stream
		BuiltinTypes::INVALID = { "Invalid", Type::UNKNOWN_SIZE, INVALID_ID },
		BuiltinTypes::S8 = { "S8", 1, S8_ID },
		BuiltinTypes::U8 = { "U8", 1, U8_ID },
		BuiltinTypes::S16 = { "S16", 2, S16_ID },
		BuiltinTypes::U16 = { "U16", 2, U16_ID },
		BuiltinTypes::S32 = { "S32", 4, S32_ID },
		BuiltinTypes::U32 = { "U32", 4, U32_ID },
		BuiltinTypes::S64 = { "S64", 8, S64_ID },
		BuiltinTypes::U64 = { "U64", 8, U64_ID },
		BuiltinTypes::S128 = { "S128", 16, S128_ID },
		BuiltinTypes::U128 = { "U128", 16, U128_ID },
		BuiltinTypes::S256 = { "S256", 32, S256_ID },
		BuiltinTypes::U256 = { "U256", 32, U256_ID },
		BuiltinTypes::BIG_INTEGER = { "BigInteger", Type::UNKNOWN_SIZE, BIG_INTEGER_ID },

		/*BuiltinTypes::F8 = { "F8", F8_ID },
		BuiltinTypes::F16 = { "F16", F16_ID },
		BuiltinTypes::F32 = { "F32", F32_ID },
		BuiltinTypes::F32 = { "F32", F32_ID },
		BuiltinTypes::F64 = { "F64", F64_ID },
		BuiltinTypes::F128 = { "F128", F128_ID },
		BuiltinTypes::BIG_FLOAT = { "BigFloat", BIG_FLOAT_ID },*/

		BuiltinTypes::UTF8_STRING = { "UTF8-String", Type::UNKNOWN_SIZE, UTF8_STRING_ID },
		BuiltinTypes::UTF16_STRING = { "UTF16-String", Type::UNKNOWN_SIZE, UTF16_STRING_ID },
		BuiltinTypes::UTF32_STRING = { "UTF32-String", Type::UNKNOWN_SIZE, UTF32_STRING_ID },
		BuiltinTypes::ESC4_STRING = { "ESC4-String", Type::UNKNOWN_SIZE, ESC4_STRING_ID },
		BuiltinTypes::ESC6_STRING = { "ESC6-String", Type::UNKNOWN_SIZE, ESC6_STRING_ID },
		BuiltinTypes::ESC8_PLUS_STRING = { "ESC8+-String", Type::UNKNOWN_SIZE, ESC8_PLUS_STRING_ID };

	SequenceData::SequenceData(const Type& listType) : First(&listType), Size(1) {}
	SequenceData::SequenceData(const Type& key, const Type& value) : First(&key), Second(&value), Size(2) {}

	bool SequenceData::operator==(const SequenceData& other) const
	{
		return this == &other || 
			this->First->operator==(*other.First) && this->Second->operator==(*other.Second) && this->Size == other.Size;
	}

	Type::Type(const char* name, u32 size, u64 id) : m_Name(), m_Size(size) {
		StringUtils::Copy(name, m_Name, sizeof(m_Name));
	}

	Type::Type(const char* name, u32 size, std::initializer_list<TypeMember> members) : m_Data(members), m_Name(), m_Size(size) {
		StringUtils::Copy(name, m_Name, sizeof(m_Name));
	}

	Type::Type(const Type& listType, bool unused) : m_Data(listType), m_Name(), m_Size(Type::UNKNOWN_SIZE) {
		m_Name[0] = '[';
		StringUtils::Copy(GetSequence().First->GetName(), m_Name + 1, sizeof(m_Name) - 1);
	}

	Type::Type(const Type& key, const Type& value) : m_Data(SequenceData(key, value)), m_Name(), m_Size(key.GetSize() + value.GetSize()) {
		s64 length = sizeof(m_Name);
		const char* keyName = key.GetName();
		const char* valueName = value.GetName();
		char* name = m_Name;
		*name++ = '<'; length--;

		size_t bytes = StringUtils::Copy(keyName, name, length);
		length -= bytes; name += bytes;
		if (length <= 2) { *name++ = 0x00; return; }

		*name++ = ','; length--;
		*name++ = ' '; length--;

		bytes = StringUtils::Copy(valueName, name, length);
		length -= bytes; name += bytes;
		if (length <= 1) { *name++ = 0x00; return; }

		*name++ = '>'; length--;
		*name = 0x00; length--;
	}

	bool Type::operator==(const Type& other) const {
		if (this->m_Data.index() != other.m_Data.index())// They must be the same kind of Type in order to be the same
			return false;
		if (IsPrimitive()) {
			return this->GetPrimitiveID() == other.GetPrimitiveID();
		} else if (IsNormalObject()) {
			return StringUtils::Equal(this->m_Name, other.m_Name) /*&& this->GetMembers() == other.GetMembers()*/;
		} else if (IsSequence()) {
			return this->GetSequence() == other.GetSequence();
		}
	}

	Type Type::CreatePrimitive(const char* name, u32 size, u64 id) {
		return Type(name, size, id);
	}

	u32 GetSizeOfMembers(std::initializer_list<TypeMember> members) {
		u32 size = 0;
		for (const auto& member : members) {
			if (member.Type->IsConstantSize())
				size += member.Type->GetSize();
			else
				return Type::UNKNOWN_SIZE;
		}
		return size;
	}

	Type Type::CreateClass(const char* name, std::initializer_list<TypeMember> members) {
		return Type(name, GetSizeOfMembers(members), members);
	}

	Type Type::CreateList(const Type& listType) {
		return Type(listType, false);
	}

	Type Type::CreateMap(const Type& key, const Type& value) {
		return Type(key, value);
	}

	thread_local int indentation = -1;

	void Indent(std::ostream& out) {
		for (int i = 0; i < indentation; i++) out << "  ";
	}

	std::ostream& operator<<(std::ostream& out, const Type& type) {
		indentation++;
		Indent(out);
		out << "Type (";
		if (type.IsPrimitive()) {
			out << "Primitive) " << type.GetName() << " ID: " << type.GetPrimitiveID() << std::endl;
		} else if (type.IsNormalObject()) {
			out << "Object) " << type.GetName() << std::endl;
			indentation++;
			for (auto& member : type.GetMembers()) {
				if (member.Type->IsPrimitive() || type == *member.Type) {
					Indent(out);
					out << member.Type->GetName() << ": " << member.Name << std::endl;
				} else {
					out << *member.Type;
					
				}
			}
			indentation--;
			out << std::endl;
		} else if (type.IsSequence()) {
			out << "Sequence)" << std::endl;
			out << type.GetSequence() << std::endl;
		}
		indentation--;
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const SequenceData& type) {
		indentation++;
		Indent(out);
		if (type.Size == 1) {
			out << "List of:" << std::endl;
			out << *type.First;
		} else {
			out << "Map. Key: " << std::endl;
			out << *type.First;
			Indent(out);
			out << "Value: " << std::endl;
			out << *type.Second;
		}
		indentation--;
		return out;
	}


	const Type& BuiltinTypes::Get(u64 id)
	{
		switch (id) {
			default:
			case INVALID_ID: return INVALID;
			case S8_ID: return S8;
			case U8_ID: return U8;
			case S16_ID: return U16;
			case U16_ID: return U16;
			case S32_ID: return S32;
			case U32_ID: return U32;
			case S64_ID: return S64;
			case U64_ID: return U64;
			case S128_ID: return S128;
			case U128_ID: return U128;
			case S256_ID: return S256;
			case U256_ID: return U256;
			case BIG_INTEGER_ID: return BIG_INTEGER;
			
			/*case F8_ID: return F8;
			case F16_ID: return F16;
			case F32_ID: return F32;
			case F64_ID: return F64;
			case F128_ID: return F128;
			case BIG_FLOAT_ID: return BIG_FLOAT;*/

			case UTF8_STRING_ID: return UTF8_STRING;
			case UTF16_STRING_ID: return UTF16_STRING;
			case UTF32_STRING_ID: return UTF32_STRING;
			case ESC4_STRING_ID: return ESC4_STRING;
			case ESC6_STRING_ID: return ESC6_STRING;
			case ESC8_PLUS_STRING_ID: return ESC8_PLUS_STRING;
		}
	}

	bool TypeMember::operator==(const TypeMember & other) {
		return Type->operator==(*other.Type) && (Name == other.Name) && (Offset == other.Offset);
	}

}