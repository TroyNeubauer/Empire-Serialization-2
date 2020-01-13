#include "Internal.h"

#include <cstdio>

namespace ES {

	namespace Internal {

		static thread_local Error s_Error;

		Error& GetError()
		{
			return s_Error;
		}

		void Init()
		{

		}

		void DefaultAllocErrorHandler()
		{
			Log("Critical Error! Failed to allocate memory! Program will terminate");
		}

		
		AllocErrorHandler s_AllocErrorHandler = &DefaultAllocErrorHandler;
		
		void SetAllocErrorHandler(AllocErrorHandler handler)
		{
			s_AllocErrorHandler = handler;
		}

		void InvokeAllocErrorHandler()
		{
			s_AllocErrorHandler();
		}

		void Log(const char* message)
		{
			printf("[Empire Serialization 2]: %s\n", message);
		}

	}

}