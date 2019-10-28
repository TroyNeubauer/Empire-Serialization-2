#pragma once

#include "../primitive/EmpirePrimitives.h"

#include <malloc.h>
#ifdef EMPIRE_PLATFORM_WINDOWS
	#include <Windows.h>
#elif defined(EMPIRE_COMPILER_GCC)
	#include <alloca.h>
#endif


namespace Empire {

    namespace TempBuffer {

#if defined(EMPIRE_COMPILER_MSVC)
        template<typename T>
        __forceinline T* Alloc(u64 elements)
		{
            return reinterpret_cast<T*>(_malloca(sizeof(T) * elements));
        }

        template<typename T>
        __forceinline void Free(T* ptr)
		{
			_freea(ptr);
        }
        
#elif defined(EMPIRE_COMPILER_GCC)
        //Returns the address of the "top of the stack". This intuitively is at a lower address than GetBottomOfStack
        inline void* GetTopOfStack()
		{
            void* result;
            asm("mov %0, rsp;"
				:"=r"(result)
			);
            return result;
        }
        
        inline void* GetBottomOfStack()
		{
            void* result;
            asm("mov %0, rbp;"
				:"=r"(result)
			);
            return result;
        }
        

        const u64 MAX_STACK_ALLOC_SIZE = 8192;
        
        template<typename T>
        __attribute__((always_inline)) T* Alloc(u64 elements)
		{
            u64 bytes = sizeof(T) * elements;
            if (bytes > MAX_STACK_ALLOC_SIZE)
				return malloc(bytes);
            else
                return static_cast<T*>(alloca(bytes));

        }
        

        template<typename T>
        __attribute__((always_inline)) void Free(T* ptr)
		{
            void* cmp = ptr;
            if (cmp <= GetTopOfStack() || cmp >= GetBottomOfStack())
                free(ptr);

        }
#endif
        
    }

}//namespace

