#if !defined(IntfIEngineTrace_Included)
#define IntfIEngineTrace_Included

#include "Main/CBaseHandle.h"
#include "Main/UtlVector.h"
#include "Main/CModel.h"
#include "Types.h"

typedef struct CGameTrace CGameTrace;
typedef CGameTrace trace_t;
typedef struct ICollideable ICollideable;
typedef struct CTraceListData CTraceListData;
typedef struct CPhysCollide CPhysCollide;
typedef struct cplane_t cplane_t;

typedef enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS
} TraceType_t;

typedef struct ITraceFilter ITraceFilter;

typedef struct ITraceFilter_VMT
{
	bool (*ShouldHitEntity)(ITraceFilter*, IHandleEntity* pEntity, int contentsMask);
	TraceType_t (*GetTraceType)(ITraceFilter*); // const
} ITraceFilter_VMT;

struct ITraceFilter
{
	ITraceFilter_VMT* vmt;
};

typedef struct IEntityEnumerator IEntityEnumerator;

typedef struct IEntityEnumerator_VMT
{
	bool (*EnumEntity)(IEntityEnumerator*, IHandleEntity* pHandleEntity);
} IEntityEnumerator_VMT;

struct IEntityEnumerator
{
	IEntityEnumerator_VMT* vmt;
};

typedef struct IEngineTrace IEngineTrace;

typedef struct IEngineTrace_VMT
{
	int (*GetPointContents)(IEngineTrace*, const Vector* vecAbsPosition, IHandleEntity** ppEntity);
	int (*GetPointContents_Collideable)(IEngineTrace*, ICollideable* pCollide, const Vector* vecAbsPosition);
	void (*ClipRayToEntity)(IEngineTrace*, const Ray_t* ray, unsigned int fMask, IHandleEntity* pEnt, trace_t* pTrace);
	void (*ClipRayToCollideable)(IEngineTrace*, const Ray_t* ray, unsigned int fMask, ICollideable* pCollide, trace_t* pTrace);
	void (*TraceRay)(IEngineTrace*, const Ray_t* ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	void (*SetupLeafAndEntityListRay)(IEngineTrace*, const Ray_t* ray, CTraceListData* traceData);
	void (*SetupLeafAndEntityListBox)(IEngineTrace*, const Vector* vecBoxMin, const Vector* vecBoxMax, CTraceListData* traceData);
	void (*TraceRayAgainstLeafAndEntityList)(IEngineTrace*, const Ray_t* ray, CTraceListData* traceData, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	void (*SweepCollideable)(IEngineTrace*, ICollideable* pCollide, const Vector* vecAbsStart, const Vector* vecAbsEnd, const QAngle* vecAngles, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	void (*EnumerateEntities)(IEngineTrace*, const Ray_t* ray, bool triggers, IEntityEnumerator* pEnumerator);
	void (*EnumerateEntitiesEx)(IEngineTrace*, const Vector* vecAbsMins, const Vector* vecAbsMaxs, IEntityEnumerator* pEnumerator);
	ICollideable* (*GetCollideable)(IEngineTrace*, IHandleEntity* pEntity);
	int (*GetStatByIndex)(IEngineTrace*, int index, bool bClear);
	void (*GetBrushesInAABB)(IEngineTrace*, const Vector* vMins, const Vector* vMaxs, CUtlVector* pOutput, int iContentsMask);
	CPhysCollide* (*GetCollidableFromDisplacementsInAABB)(IEngineTrace*, const Vector* vMins, const Vector* vMaxs);
	bool (*GetBrushInfo)(IEngineTrace*, int iBrush, CUtlVector* pPlanesOut, int* pContentsOut);
	bool (*PointOutsideWorld)(IEngineTrace*, const Vector* ptTest);
	int (*GetLeafContainingPoint)(IEngineTrace*, const Vector* ptTest);
} IEngineTrace_VMT;

struct IEngineTrace
{
	IEngineTrace_VMT* vmt;
};

#define IEngineTrace_REFL "IEngineTrace"
#define IEngineTrace_SIGNATURE "EngineTraceClient003"
#define IEngineTrace_MODULE "engine"

#endif
