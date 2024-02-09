#pragma once

class mstudiobone_t
{
public:
	int32_t name_index;         // 0x0000
	int32_t parent;             // 0x0004
	int32_t bonecontroller[6];  // 0x0008
	Vector N00001D0F;      // 0x0020
	char pad_002C[136];         // 0x002C
};

class mstudiobbox_t
{
public:
	int32_t bone;             // 0x0000
	int32_t group;            // 0x0004
	Vector min;          // 0x0008
	Vector max;          // 0x0014
	int32_t hitbox_name_id;   // 0x0020
	char pad_0024[4];         // 0x0024
	int32_t hitbox_name_id_;  // 0x0028
};


class mstudiohitboxset_t
{
public:
	int hitbox_set_name_index;  // 0x0000
	int num_hitboxes;           // 0x0004
	int hitbox_index;           // 0x0008

	mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitbox_index) + i; };
};

class studiohdr_t
{

public:
	int32_t id;                     // 0x0000
	int32_t version;                // 0x0004
	char pad_0008[8];               // 0x0008
	char name[64];                  // 0x0010
	int32_t length;                 // 0x0050
	char pad_0054[24];              // 0x0054
	Vector min;						// 0x006C
	Vector max;						// 0x0078
	char pad_0084[24];              // 0x0084
	uint32_t flags;                 // 0x009C
	int32_t num_bones;              // 0x00A0
	int32_t bone_index;             // 0x00A4
	int32_t num_bone_controllers;   // 0x00A8
	int32_t bone_controller_index;  // 0x00AC
	int32_t num_hitbox_sets;        // 0x00B0
	int32_t hitbox_set_index;       // 0x00B4

	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + 0xb4) + i;
	};

	mstudiobone_t* pBoneSet(int i) const
	{
		return (mstudiobone_t*)(((char*)this) + bone_index) + i;
	}
};

class CBaseEntity
{
public:
	CBaseEntity* GetLocalPlayer()
	{
		auto LocalPlayer = LocalPlayerPtr;

		if (!LocalPlayer)
			return nullptr;

		return *reinterpret_cast<CBaseEntity**>(LocalPlayer);
	}

	CBaseEntity* GetEntity(const int Index)
	{
		auto EntityList = EntityListPtr;

		if (!EntityList)
			return nullptr;

		return *reinterpret_cast<CBaseEntity**>(EntityList + (static_cast<uintptr_t>(Index) << 5));
	}

	Vector GetEyePos(Vector& in)
	{
		return Memory::CallVirtual<180, Vector>(this, in);
	}

	void GetViewAngles(Vector& in)
	{
		return Memory::CallVirtual<188, void>(this, in);
	}

	const char* GetSignifierName()
	{
		if (!this)
			return NULL;

		return *(char**)(this + 0x4b0);
	}

	const char* GetName()
	{
		if (!this)
			return NULL;

		return *(char**)(this + 0x3258);
	}

	int GetPetTitanMode()
	{
		if (!this)
			return NULL;

		return *(unsigned int*)(this + 0x2C50);
	}

	Vector GetCameraPos()
	{
		if (!this)
			return {};

		return *(Vector*)(this + 0x19A4);
	}

	Vector GetViewAngles()
	{
		if (!this)
			return {};

		return *(Vector*)(this + 0x19B0);
	}

	inline int GetFlags()
	{
		if (!this)
			return NULL;

		return *(int*)(this + 0x8C);
	}

	int GetHealth()
	{
		if (!this)
			return NULL;

		return *(int*)(this + 0x390);
	}

	int GetMaxHealth()
	{
		if (!this)
			return NULL;

		return *(int*)(this + 0x4A8);
	}

	int GetTeam()
	{
		if (!this)
			return NULL;

		return *(int*)(this + 0x3A4);
	}

	int GetLifeState()
	{
		if (!this)
			return NULL;

		return *(int*)(this + 0x6C8);
	}

	Vector GetAbsOrigin()
	{
		if (!this)
			return {};

		return *(Vector*)(this + 0x12C);
	}

	Vector GetMins()
	{
		if (!this)
			return Vector();

		return *(Vector*)(this + 0x3F8 + 0x10);
	}

	Vector GetMaxs()
	{
		if (!this)
			return Vector();

		return *(Vector*)(this + 0x3F8 + 0x1c);
	}

	Vector GetPunchAngles()
	{
		if (!this)
			return {};

		return *(Vector*)(this + 0x1e34);
	}

	inline uintptr_t GetBoneClass()
	{
		if (!this)
			return NULL;

		uintptr_t Bones = *(uintptr_t*)(this + 0x1008);
		return Bones;
	}

	bool GetHitboxPosition(Vector& Out, int HitboxNr)
	{
		auto pStudioHdr = *(uintptr_t*)(this + 0x1208);
		if (!pStudioHdr)
			return 0;

		auto set = *(uintptr_t*)(pStudioHdr + 8) + *(int*)(*(uintptr_t*)(pStudioHdr + 8) + 0xB4i64);

		auto Hitbox = (*(mstudiobbox_t*)(*(int*)(set + 8) + set + 0x44i64 * HitboxNr));// pHitbox->bone

		auto Bones = GetBoneClass();
		if (!Bones)
			return false;

		matrix3x4_t* Matrix = (matrix3x4_t*)(Bones);

		Vector Min, Max;
		Math::VectorTransform(Hitbox.min, Matrix[Hitbox.bone], Min);
		Math::VectorTransform(Hitbox.max, Matrix[Hitbox.bone], Max);

		Vector FinalMin = Min;
		Vector FinalMax = Max;

		Out = (FinalMin + FinalMax) * 0.5f; // return approx center

		return true;
	}
};

inline CBaseEntity* BaseEntity;

class CClientEntityList
{
public:
	CBaseEntity* GetClientEntity(int Index)
	{
		return Memory::CallVirtual<3, CBaseEntity*>(this, Index);
	}

	int GetHighestEntityIndex()
	{
		return Memory::CallVirtual<6, int>(this);
	}
};

inline CClientEntityList* EntityList;