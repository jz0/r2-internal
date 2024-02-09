#include "CreateMove.hpp"
#include "../Features/Aim.hpp"
#include "../Features/Vars.hpp"

ClientmodeCreateMoveFn OriginalClientmodeCreateMove = nullptr;
bool __fastcall HookedClientmodeCreateMove(__int64 ThisPtr, float a2, CUserCmd* cmd, unsigned __int8 a4)
{
	auto Ret = OriginalClientmodeCreateMove(ThisPtr, a2, cmd, a4);

	if (!cmd->command_number)
		return Ret;

	if (Vars::Airstuck && InputSystem->IsButtonDown(KEY_B))
		cmd->command_number = 0x7FFFFFFF;

	if (Vars::Aimbot)
		Aim::Run(cmd);

	if (Vars::Speedhack && InputSystem->IsButtonDown(MOUSE_4))
		*(float*)((uintptr_t)cmd + 0xAC) += 0.025f;

	return Ret;
}