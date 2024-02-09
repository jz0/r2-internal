#include "ESP.hpp"
#include "../Renderer/Renderer.hpp"

typedef float matrix_t[3][4];
const auto GetViewMatrix = []() -> float(*)[4]
{
	auto ViowRender = *(uintptr_t*)(ViewRenderPtr);

	if (!ViowRender)
		return nullptr;

	auto* const ViewMatrix = **(matrix_t**)(ViowRender + 0x12EF90);
	return ViewMatrix;
};

bool ESP::WorldToScreen(Vector Src, Vector& Dst)
{
	auto* const Matrix = GetViewMatrix();
	auto w = 0.0f;

	Dst[0] = Matrix[0][0] * Src[0] + Matrix[0][1] * Src[1] + Matrix[0][2] * Src[2] + Matrix[0][3];
	Dst[1] = Matrix[1][0] * Src[0] + Matrix[1][1] * Src[1] + Matrix[1][2] * Src[2] + Matrix[1][3];
	w = Matrix[3][0] * Src[0] + Matrix[3][1] * Src[1] + Matrix[3][2] * Src[2] + Matrix[3][3];

	if (w < 0.01f)
		return false;

	const auto InvW = 1.f / w;
	Dst[0] *= InvW;
	Dst[1] *= InvW;

	int ScreenWidth;
	int ScreenHeight;
	MatSystemSurface->GetScreenSize(&ScreenWidth, &ScreenHeight);

	auto x = ScreenWidth / 2.f;
	auto y = ScreenHeight / 2.f;

	x += .5f * Dst[0] * ScreenWidth + .5f;
	y -= .5f * Dst[1] * ScreenHeight + .5f;

	Dst[0] = x;
	Dst[1] = y;

	return true;
}

bool IsVisible(Vector Start, Vector End, CBaseEntity* Local, CBaseEntity* Entity)
{
	Ray_t RayData;
	RayData.Init(Start, End);

	trace_t Trace;
	memset(&Trace, 0, sizeof(trace_t));

	CTraceFilterCustom Filter;
	Filter.Entity = Local;
	EngineTrace->TraceRay(RayData, TRACE_MASK_SHOT, &Filter, &Trace);

	CBaseEntity* TraceEntity = (CBaseEntity*)Trace.m_pEnt;

	if (!TraceEntity || TraceEntity->GetHealth() <= 0 || TraceEntity == Local)
		return false;

	return TraceEntity == Entity;
}

void RenderBoundingBox(CBaseEntity* Local, CBaseEntity* Entity, float Dist)
{
	auto AbsOrigin = Entity->GetAbsOrigin();
	Vector vecMin = AbsOrigin + Entity->GetMins();
	Vector vecMax = AbsOrigin + Entity->GetMaxs();

	Vector points[] =
	{
		{ vecMin[0], vecMin[1], vecMin[2] },
		{ vecMin[0], vecMax[1], vecMin[2] },
		{ vecMax[0], vecMax[1], vecMin[2] },
		{ vecMax[0], vecMin[1], vecMin[2] },
		{ vecMax[0], vecMax[1], vecMax[2] },
		{ vecMin[0], vecMax[1], vecMax[2] },
		{ vecMin[0], vecMin[1], vecMax[2] },
		{ vecMax[0], vecMin[1], vecMax[2] },
	};

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!ESP::WorldToScreen(points[3], flb) ||
		!ESP::WorldToScreen(points[0], blb) ||
		!ESP::WorldToScreen(points[2], frb) ||
		!ESP::WorldToScreen(points[6], blt) ||
		!ESP::WorldToScreen(points[5], brt) ||
		!ESP::WorldToScreen(points[4], frt) ||
		!ESP::WorldToScreen(points[1], brb) ||
		!ESP::WorldToScreen(points[7], flt))
		return;

	Vector screen[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb[0];
	float top = flb[1];
	float right = flb[0];
	float bottom = flb[1];

	for (int i = 0; i < 8; i++)
	{
		if (left > screen[i][0])
			left = screen[i][0];
		if (top < screen[i][1])
			top = screen[i][1];
		if (right < screen[i][0])
			right = screen[i][0];
		if (bottom > screen[i][1])
			bottom = screen[i][1];
	}

	float x = left,
		y = bottom,
		w = right - left,
		h = top - bottom;

	int t = 1;

	Vector Head;
	Entity->GetHitboxPosition(Head, 3);

	Vector Out;
	if (ESP::WorldToScreen(Head, Out))
	{
		Renderer::DrawRect(Out.x, Out.y, 3, 3, Color(255, 0, 0, 255));
	}

	auto PlayerVisible = IsVisible(Local->GetCameraPos(), Head, Local, Entity);
	Color VisibleColor = PlayerVisible ? Color(255, 255, 255, 255) : Color(255, 0, 0, 255);

	//Box
	{
		Renderer::DrawOutlinedRect(x, y, w, h, VisibleColor);
		Renderer::DrawOutlinedRect(x - t, y - t, w + t * 2, h + t * 2, Color(0, 0, 0, 200));
		Renderer::DrawOutlinedRect(x + t, y + t, w - t * 2, h - t * 2, Color(0, 0, 0, 200));
	}

	int Elements = 0;
	int Spacer = 14;
	MatSystemSurface->DrawColoredTextWrapper(5, x + w + 2, y + Spacer * Elements, 255, 255, 255, 255, Entity->GetName());
	Elements++;
}

void ESP::Run()
{
	auto Local = BaseEntity->GetLocalPlayer();

	if (!Local || Local->GetLifeState() != LIFE_ALIVE || Local->GetHealth() <= 0)
		return;

	int w, h;
	MatSystemSurface->GetScreenSize(&w, &h);

	//Crosshair
	Renderer::DrawRect(w / 2, h / 2, 3, 3, Color(255, 255, 255, 255));

	for (int i = 1; i < EntityList->GetHighestEntityIndex(); i++)
	{
		auto Entity = EntityList->GetClientEntity(i);

		if (!Entity || Entity->GetLifeState() != LIFE_ALIVE || Entity->GetHealth() <= 0 || Entity->GetTeam() == Local->GetTeam() || Entity == Local)
			continue;

		auto Tmp = *(int*)((uintptr_t)Entity + 0x4b0);

		if (!Tmp)
			continue;

		auto Name = Entity->GetSignifierName();

		if (Name[0] == 'p') //player
		{
			Vector AbsOrigin = Entity->GetAbsOrigin();
			float Dist = AbsOrigin.DistTo(Local->GetAbsOrigin()) / METERS_TO_INCHES;

			if (Dist > 250.f)
				continue;

			RenderBoundingBox(Local, Entity, Dist);
		}
	}
}