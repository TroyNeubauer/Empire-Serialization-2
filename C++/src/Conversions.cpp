#include "EmpireSerialization/Conversions.h"

#include "EmpireSerialization2.h"
#include "Internal.h"
#include "ESString.h"
#include "Charsets.h"

namespace ES {
	namespace Conversions {


		//Explicitally instantiate src->utf32->dest conversions

		//utf32 can store any codepoint the smaller encodings can store in 1 word
		template<> std::size_t RequiredCapacity <utf8, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf16, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc4, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc6, utf32>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity <esc8, utf32>(std::size_t words) { return words; }

		//1 word in utf32 can take more words in utr8/utf16 for the worst case
		template<> std::size_t RequiredCapacity<utf32,  utf8>(std::size_t words) { return words * 4; }
		template<> std::size_t RequiredCapacity<utf32, utf16>(std::size_t words) { return words * 2; }

		//If esc4/esc6 can't store things they cant support. So compilant utf32 strings -> esc4/esc6 will always be 1-1
		template<> std::size_t RequiredCapacity<utf32, esc4>(std::size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf32, esc6>(std::size_t words) { return words; }

		//esc8 uses VLE which can be bloated to up to 3 words per utf32 codepoint if it has a high unicode value
		template<> std::size_t RequiredCapacity<utf32, esc8>(std::size_t words) { return words * 3; }

		//Converting a type to itself is always 1-1
		template<> std::size_t RequiredCapacity<utf8,   utf8>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf16, utf16>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<utf32, utf32>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc4,   esc4>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc6,   esc6>(size_t words) { return words; }
		template<> std::size_t RequiredCapacity<esc8,   esc8>(size_t words) { return words; }

	}
}
