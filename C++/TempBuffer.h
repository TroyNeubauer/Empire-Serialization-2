#pragma once

#include "EmpirePrimitives.h"

namespace Empire {

//This needs to be inline since it is allowed to allocate memory on the stack
void* AllocateTempBuffer(u64 bytes);

u64 GetStackSpace();

}//namespace

