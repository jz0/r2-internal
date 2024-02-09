#pragma once
#include "../Include.hpp"

typedef bool(__fastcall* ClientmodeCreateMoveFn)(__int64, float, CUserCmd*, unsigned __int8);
extern ClientmodeCreateMoveFn OriginalClientmodeCreateMove;
bool __fastcall HookedClientmodeCreateMove(__int64 ThisPtr, float a2, CUserCmd* cmd, unsigned __int8 a4);