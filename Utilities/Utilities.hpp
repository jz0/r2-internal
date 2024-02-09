#pragma once
#include "../Include.hpp"

#define RVA(Instr, InstrSize) ((uintptr_t)Instr + InstrSize + *(long*)((uintptr_t)Instr + (InstrSize - sizeof(long))))

namespace Utilities
{
	uintptr_t FindPattern(uintptr_t Base, const char* Sig);
}