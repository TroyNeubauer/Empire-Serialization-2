#include "TypeManager.h"

#include <sstream>
namespace Empire {

std::string TypeData::ToString() {
	std::stringstream ss;
	ss << "Type Data ";
	if (offset == UINT32_MAX) {
		ss << "Class";
	} else {
		ss << "Field";
	}
	ss << " name " << name;
	ss << ", size: " << size;
	if (offset == UINT32_MAX) {
		ss << ", fields: " << std::endl;
		for (TypeData& data : fields) {
			ss << "\t" << data.name << ", offset: " << data.offset << ", size: " << data.size << std::endl;
		}
	} else {
		ss << ", offset: " << offset;
	}

	return ss.str();
}



}//namespace
