#include "Screenshot.hpp"

CLC_Screenshot_WriteToBufferFn OriginalCLC_Screenshot_WriteToBuffer = nullptr;
bool __fastcall HookedCLC_Screenshot_WriteToBuffer(__int64 a1, __int64 a2)
{
	printf("[+] Game called CLC_Screenshot::WriteToBuffer \n");
	return false;
}

CLC_Screenshot_ReadFromBufferFn OriginalCLC_Screenshot_ReadFromBuffer = nullptr;
bool __fastcall HookedCLC_Screenshot_ReadFromBuffer(__int64 a1, __int64 a2)
{
	printf("[+] Game called CLC_Screenshot::ReadFromBuffer \n");
	return false;
}