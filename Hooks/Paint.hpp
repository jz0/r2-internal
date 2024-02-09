#pragma once
#include "../Include.hpp"

typedef __int64(__fastcall* PaintTraverseFn)(void*, __int64, bool, bool);
extern PaintTraverseFn OriginalPaintTraverse;
__int64 __fastcall HookedPaintTraverse(void* thisptr, __int64 panel, bool paintPopups, bool allowForce);