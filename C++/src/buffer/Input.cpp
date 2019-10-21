
#include "Input.h"



namespace Empire {
		inline bool BufferedInput::EnsureCapacity(u64 bytes EMPIRE_ERROR_PARAMETER) {
		if (bytes + m_Offset > m_Limit) {
			EMPIRE_ERROR(BUFFER_OVERFLOW, false, "Limit: %lu, Size Needed: %lu, Chunk Size: %lu", m_Limit, bytes + m_Offset, bytes);
		}
		return true;
	}
	
	
	
	
	
}
