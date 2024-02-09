#include "Include.hpp"

typedef void* (*CreateInterfaceFn)(const char* Name, int* ReturnCode);
void* CreateInterface(const char* ModuleName, const char* InterfaceName)
{
	const auto Address = LI_FN(GetProcAddress)(LI_FN(GetModuleHandleA)(ModuleName), "CreateInterface");
	const auto CreateInterface = reinterpret_cast<CreateInterfaceFn>(Address);

	auto Ret = CreateInterface(InterfaceName, nullptr);

	//printf("Module: %s : %s : %p \n", ModuleName, InterfaceName, Ret);

	return Ret;
}

void Init()
{
	AllocConsole();
	freopen_s(&IO, "CONOUT$", "w", stdout);
	printf("r2\n");

	BaseAddress = (uintptr_t)LI_MODULE(("Titanfall2.exe")).get();

	//Wait for all modules to load
	while (!InputSystemPtr)
	{
		ClientPtr = (uintptr_t)LI_MODULE(("client.dll")).get();
		EnginePtr = (uintptr_t)LI_MODULE(("engine.dll")).get();
		MatSystemSurfacePtr = (uintptr_t)LI_MODULE(("vguimatsurface.dll")).get();
		VGUI2Ptr = (uintptr_t)LI_MODULE(("vgui2.dll")).get();
		InputSystemPtr = (uintptr_t)LI_MODULE(("inputsystem.dll")).get();
	}

	auto GameMovementPtr = RVA(Utilities::FindPattern(ClientPtr, "48 89 05 ? ? ? ? 48 85 C9 74 10 48 8B 01 48 8B"), 7);
	LocalPlayerPtr = (uintptr_t)(GameMovementPtr + 0x8);
	Globals = *(CGlobalVarsBase**)RVA(Utilities::FindPattern(ClientPtr, "48 8B 05 ? ? ? ? 48 8B D9 F3 0F 10 48 ? E8 ? ? ? ? 48 8B"), 7);
	EntityListPtr = RVA(Utilities::FindPattern(ClientPtr, "48 8D 0D ? ? ? ? 0F 1F 00 4C 8B 10 4C 3B D2 75 21 48 83 C0 08 41 FF"), 7);
	ViewRenderPtr = RVA(Utilities::FindPattern(ClientPtr, "48 8B 0D ? ? ? ? 83 CA FF 48 8B 01 FF 50 60 48 8D 15 ? ? ? ? 48 8D 88"), 7);
	ClientState = (CClientState*)(EnginePtr + 0x7A64C0);
	Input = (CInput*)RVA(Utilities::FindPattern(ClientPtr, "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 50 50 83 BB"), 7);
	Client = (CHLClient*)RVA(Utilities::FindPattern(ClientPtr, "48 8D 0D ? ? ? ? 49 8B D1 E8 ? ? ? ? C6 05 ? ? ? ? ? 48 83 C4 28 C3"), 7);
	EngineVGUI = (CEngineVGui*)RVA(Utilities::FindPattern(EnginePtr, "48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? FF 50 18"), 7);

	//Interfaces
	MatSystemSurface = (CMatSystemSurface*)CreateInterface("vguimatsurface.dll", "VGUI_Surface031");
	PanelWrapper = (VPanelWrapper*)CreateInterface("vgui2.dll", "VGUI_Panel009");
	EntityList = (CClientEntityList*)CreateInterface("client.dll", "VClientEntityList003");
	EngineTrace = (CEngineTrace*)CreateInterface("engine.dll", "EngineTraceClient004");
	InputSystem = (CInputSystem*)CreateInterface("inputsystem.dll", "InputSystemVersion001");

	//Detours
	MH_Initialize();

	auto CLC_Screenshot_WriteToBuffer = Utilities::FindPattern(EnginePtr, "48 89 5C 24 ? 57 48 83 EC 20 8B 42 10 48 8B DA 48 8B F9 25 ? ? ? ? 7D 07 FF C8 83 C8");
	if (MH_CreateHook((void*)CLC_Screenshot_WriteToBuffer, &HookedCLC_Screenshot_WriteToBuffer, (void**)&OriginalCLC_Screenshot_WriteToBuffer) == MH_OK)
	{
		printf("[+] CLC_Screenshot_WriteToBuffer\n");
		MH_EnableHook((void*)CLC_Screenshot_WriteToBuffer);
	}

	auto CLC_Screenshot_ReadFromBuffer = Utilities::FindPattern(EnginePtr, "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B DA 48 8B 52");
	if (MH_CreateHook((void*)CLC_Screenshot_ReadFromBuffer, &HookedCLC_Screenshot_ReadFromBuffer, (void**)&OriginalCLC_Screenshot_ReadFromBuffer) == MH_OK)
	{
		printf("[+] CLC_Screenshot_ReadFromBuffer\n");
		MH_EnableHook((void*)CLC_Screenshot_ReadFromBuffer);
	}

	auto ClientmodeCreateMovePtr = Utilities::FindPattern(ClientPtr, "48 89 5C 24 ? 57 48 83 EC 30 48 8B 0D ? ? ? ? 0F 29 74 24 ? 41 0F");
	if (MH_CreateHook((void*)ClientmodeCreateMovePtr, &HookedClientmodeCreateMove, (void**)&OriginalClientmodeCreateMove) == MH_OK)
	{
		printf("[+] CreateMove\n");
		MH_EnableHook((void*)ClientmodeCreateMovePtr);
	}	
	
	auto PaintTraversePtr = Utilities::FindPattern(VGUI2Ptr, "48 89 5C 24 ? 57 48 83 EC 20 48 8B 01 41 0F B6 D9 41 0F B6 F8 FF 90");
	if (MH_CreateHook((void*)PaintTraversePtr, &HookedPaintTraverse, (void**)&OriginalPaintTraverse) == MH_OK)
	{
		printf("[+] PaintTraverse\n");
		MH_EnableHook((void*)PaintTraversePtr);
	}
}

bool __stdcall DllMain(void* Instance, unsigned long Reason, void* Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH)
		Init();

	return true;
}