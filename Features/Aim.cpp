#include "Aim.hpp"
#include "Vars.hpp"

bool AimIsVisible(Vector Start, Vector End, CBaseEntity* Local, CBaseEntity* Entity)
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

int GetTargetEntity(CUserCmd* Cmd, CBaseEntity* LocalPlayer)
{
	int Target = -1;
	float BestTarget = {};
	float AimFoV = 20.f;
	Vector HitboxPosition;
	auto CameraPos = LocalPlayer->GetCameraPos();

	if (Vars::RageMode)
		AimFoV = 360.f;

	for (int i = 1; i < EntityList->GetHighestEntityIndex(); i++)
	{
		auto TargetEntity = EntityList->GetClientEntity(i);

		if (!TargetEntity || TargetEntity->GetLifeState() != LIFE_ALIVE || TargetEntity->GetTeam() == LocalPlayer->GetTeam())
			continue;

		if (TargetEntity->GetHealth() <= 0)
			continue;

		auto Tmp = *(int*)((uintptr_t)TargetEntity + 0x4b0);

		if (!Tmp)
			continue;

		auto Name = TargetEntity->GetSignifierName();

		if (Name[0] != 'p')
			continue;

		if (!TargetEntity->GetHitboxPosition(HitboxPosition, 3))
			continue;

		auto Visible = AimIsVisible(CameraPos, HitboxPosition, LocalPlayer, TargetEntity);

		if (Visible)
		{
			auto Angle = Math::CalcAngle(CameraPos, HitboxPosition);
			auto FoV = Math::CalcFov(Cmd->worldViewAngles, Angle);

			if (FoV < AimFoV)
			{
				AimFoV = BestTarget;
				Target = i;
			}
		}
	}

	return Target;
}

void Aim::Run(CUserCmd* Cmd)
{
	if (!InputSystem->IsButtonDown(MOUSE_5))
		return;

	CBaseEntity* LocalPlayer = BaseEntity->GetLocalPlayer();

	if (!LocalPlayer || LocalPlayer->GetLifeState() != LIFE_ALIVE)
		return;

	auto Camera = LocalPlayer->GetCameraPos();
	auto WeaponPunch = LocalPlayer->GetPunchAngles();

	auto TargetIndex = GetTargetEntity(Cmd, LocalPlayer);

	if (TargetIndex == -1)
		return;

	auto Target = EntityList->GetClientEntity(TargetIndex);

	//lol
	if (Target->GetHealth() > 120)
		return;

	Vector AimPoint;
	if (!Target->GetHitboxPosition(AimPoint, 3))
		return;

	Vector AimAngle = Math::CalcAngle(Camera, AimPoint);

	AimAngle -= WeaponPunch;

	AimAngle = Math::ClampAngles(AimAngle);

	if (Vars::RageMode)
		Cmd->buttons |= IN_ATTACK;

	Cmd->attackangles = AimAngle;
}