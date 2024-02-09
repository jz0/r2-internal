#pragma once 
#include "../Include.hpp"

typedef bool(__fastcall* CLC_Screenshot_WriteToBufferFn)(__int64, __int64);
extern CLC_Screenshot_WriteToBufferFn OriginalCLC_Screenshot_WriteToBuffer;
bool __fastcall HookedCLC_Screenshot_WriteToBuffer(__int64 a1, __int64 a2);

typedef bool(__fastcall* CLC_Screenshot_ReadFromBufferFn)(__int64, __int64);
extern CLC_Screenshot_ReadFromBufferFn OriginalCLC_Screenshot_ReadFromBuffer;
bool __fastcall HookedCLC_Screenshot_ReadFromBuffer(__int64 a1, __int64 a2);