#include "Paint.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Features/ESP.hpp"
#include "../Features/Vars.hpp"

static bool MenuOpen = false;
Color Enabled(bool Variable) { return Variable ? Color(65, 255, 65, 255) : Color(255, 65, 65, 255); }

PaintTraverseFn OriginalPaintTraverse = nullptr;
__int64 __fastcall HookedPaintTraverse(void* thisptr, __int64 panel, bool paintPopups, bool allowForce)
{
	auto Ret = OriginalPaintTraverse(thisptr, panel, paintPopups, allowForce);

	if (panel == MatSystemSurface->GetEmbeddedPanel())
	{
		MatSystemSurface->PushMakeCurrent(panel, true);
		MatSystemSurface->DrawColoredTextWrapper(5, 2, 2, 255, 255, 255, 255, "titanfall 2");

		int w, h;
		MatSystemSurface->GetScreenSize(&w, &h);

		if (!MenuOpen && InputSystem->KeyPressed(KEY_HOME))
			MenuOpen = true;

		if (MenuOpen && InputSystem->KeyPressed(KEY_HOME))
			MenuOpen = false;

		if (MenuOpen && InputSystem->KeyPressed(KEY_F2))
			Vars::Aimbot = !Vars::Aimbot;

		if (MenuOpen && InputSystem->KeyPressed(KEY_F3))
			Vars::RageMode = !Vars::RageMode;

		if (MenuOpen && InputSystem->KeyPressed(KEY_F4))
			Vars::Speedhack = !Vars::Speedhack;

		if (MenuOpen && InputSystem->KeyPressed(KEY_F5))
			Vars::Airstuck = !Vars::Airstuck;

		int Spacer = 24;
		int Elements = 1;

		if (MenuOpen)
		{
			Renderer::DrawString(w / 2, h / 2 + 2 + Spacer * Elements, Enabled(Vars::Aimbot), "F2 - pSilent [MOUSE5]");
			Elements++;

			Renderer::DrawString(w / 2, h / 2 + 2 + Spacer * Elements, Enabled(Vars::RageMode), "F3 - Autoshoot");
			Elements++;

			Renderer::DrawString(w / 2, h / 2 + 2 + Spacer * Elements, Enabled(Vars::Speedhack), "F4 - Speedhack [MOUSE4]");
			Elements++;

			Renderer::DrawString(w / 2, h / 2 + 2 + Spacer * Elements, Enabled(Vars::Airstuck), "F5 - Airstuck [B]");
			Elements++;
		}

		if (ClientState->SignonState == SIGNONSTATE_FULL)
			ESP::Run();

		MatSystemSurface->PopMakeCurrent(panel);
	}

	return Ret;
}