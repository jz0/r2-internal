#pragma once

struct plane_t
{
	Vector	surfaceNormal;	//0x0000
	float	dist;			//0x000C
};

struct csurface_t
{
	const char* name;					//0x0000
	int surfaceProps;					//0x0008
	unsigned short	flags;				//0x000C
};

struct trace_t
{
	Vector start;
	int unk0;
	Vector end;
	int unk01;
	plane_t plane;
	float flFraction;
	int contents;
	unsigned short dispFlags;
	bool allSolid;
	bool startSolid;
	int unk02;
	float fractionLeftSolid;
	int unk03;
	csurface_t surface;
	int hitGroup;
	short physicsBone;
	unsigned short m_nWorldSurfaceIndex;
	void* m_pEnt; //CBaseEntity
	int hitbox;
	char pad[6]; //0x006A

	bool DidHit()
	{
		return flFraction < 1 || allSolid || startSolid;
	}
};

class __declspec(align(16))VectorAligned : public Vector
{
public:
	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}
	float w;
};

struct Ray_t
{
	VectorAligned  m_Start;	// starting point, centered within the extents
	VectorAligned  m_Delta;	// direction + length of the ray
	VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
	char unk03[12];
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?

	void Init(const Vector& start, const Vector& end)
	{
		memset(this, 0, sizeof(Ray_t));

		m_Start = start;
		m_Delta = (end - start);

		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_IsRay = true;
	}
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class IHandleEntity;
class CBaseEntity;

class ITraceFilter
{
public:
	virtual ~ITraceFilter()
	{
	}
	virtual bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t    GetTraceType() const = 0;
	virtual bool unk0()
	{
		return false;
	}
};

class CTraceFilterCustom : public ITraceFilter
{
public:
	CBaseEntity* Entity = nullptr;

	virtual bool ShouldHitEntity(IHandleEntity* pServerEntity, int contentsMask)
	{
		if ((CBaseEntity*)pServerEntity == Entity)
			return false;

		return true;
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING_FILTER_PROPS;
	}
};

class CEngineTrace
{
public:
	void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		return Memory::CallVirtual<3, void>(this, ray, fMask, pTraceFilter, pTrace);
	}
};

inline CEngineTrace* EngineTrace;