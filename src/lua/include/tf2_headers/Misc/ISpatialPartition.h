#if !defined(MiscISpatialPartition_Included)
#define MiscISpatialPartition_Included

#include "Definitions.h"
#include "Misc/CommonMacros.h"

typedef struct Ray_t Ray_t;
typedef struct IHandleEntity IHandleEntity;

#define PARTITION_ALL_CLIENT_EDICTS (PARTITION_CLIENT_NON_STATIC_EDICTS | PARTITION_CLIENT_STATIC_PROPS | PARTITION_CLIENT_RESPONSIVE_EDICTS | PARTITION_CLIENT_SOLID_EDICTS)

#define PARTITION_CLIENT_GAME_EDICTS (PARTITION_ALL_CLIENT_EDICTS & ~PARTITION_CLIENT_STATIC_PROPS)
#define PARTITION_SERVER_GAME_EDICTS (PARTITION_ENGINE_SOLID_EDICTS | PARTITION_ENGINE_TRIGGER_EDICTS | PARTITION_ENGINE_NON_STATIC_EDICTS)

typedef enum IterationRetval_t
{
	ITERATION_CONTINUE = 0,
	ITERATION_STOP
} IterationRetval_t;

typedef unsigned short SpatialPartitionHandle_t;
typedef int SpatialPartitionListMask_t;
typedef int SpatialTempHandle_t;

typedef struct IPartitionEnumerator IPartitionEnumerator;

typedef struct IPartitionEnumerator_VMT
{
	IterationRetval_t (*EnumElement)(IPartitionEnumerator*, IHandleEntity* pHandleEntity);
} IPartitionEnumerator_VMT;

struct IPartitionEnumerator
{
	IPartitionEnumerator_VMT* vmt;
};

typedef struct IPartitionQueryCallback IPartitionQueryCallback;

typedef struct IPartitionQueryCallback_VMT
{
	void (*OnPreQuery_V1)(IPartitionQueryCallback*);
	void (*OnPreQuery)(IPartitionQueryCallback*, SpatialPartitionListMask_t listMask);
	void (*OnPostQuery)(IPartitionQueryCallback*, SpatialPartitionListMask_t listMask);
} IPartitionQueryCallback_VMT;

struct IPartitionQueryCallback
{
	IPartitionQueryCallback_VMT* vmt;
};

enum
{
	PARTITION_INVALID_HANDLE = (SpatialPartitionHandle_t)~0
};

typedef struct ISpatialPartition ISpatialPartition;

typedef struct ISpatialPartition_VMT
{
	//virtual ~ISpatialPartition() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	SpatialPartitionHandle_t (*CreateHandle)(ISpatialPartition*, IHandleEntity* pHandleEntity);
	SpatialPartitionHandle_t (*CreateHandleEx)(ISpatialPartition*, IHandleEntity* pHandleEntity, SpatialPartitionListMask_t listMask, const Vector* mins, const Vector* maxs);
	void (*DestroyHandle)(ISpatialPartition*, SpatialPartitionHandle_t handle);
	void (*Insert)(ISpatialPartition*, SpatialPartitionListMask_t listMask, SpatialPartitionHandle_t handle);
	void (*RemoveListMask)(ISpatialPartition*, SpatialPartitionListMask_t listMask, SpatialPartitionHandle_t handle);
	void (*RemoveAndInsert)(ISpatialPartition*, SpatialPartitionListMask_t removeMask, SpatialPartitionListMask_t insertMask, SpatialPartitionHandle_t handle);
	void (*Remove)(ISpatialPartition*, SpatialPartitionHandle_t handle);
	void (*ElementMoved)(ISpatialPartition*, SpatialPartitionHandle_t handle, const Vector* mins, const Vector* maxs);
	SpatialTempHandle_t (*HideElement)(ISpatialPartition*, SpatialPartitionHandle_t handle);
	void (*UnhideElement)(ISpatialPartition*, SpatialPartitionHandle_t handle, SpatialTempHandle_t tempHandle);
	void (*InstallQueryCallback_V1)(ISpatialPartition*, IPartitionQueryCallback* pCallback);
	void (*RemoveQueryCallback)(ISpatialPartition*, IPartitionQueryCallback* pCallback);

	void (*EnumerateElementsInBox)(
		ISpatialPartition*,
		SpatialPartitionListMask_t listMask,
		const Vector* mins,
		const Vector* maxs,
		bool coarseTest,
		IPartitionEnumerator* pIterator);

	void (*EnumerateElementsInSphere)(
		ISpatialPartition*,
		SpatialPartitionListMask_t listMask,
		const Vector* origin,
		float radius,
		bool coarseTest,
		IPartitionEnumerator* pIterator);

	void (*EnumerateElementsAlongRay)(
		ISpatialPartition*,
		SpatialPartitionListMask_t listMask,
		const Ray_t* ray,
		bool coarseTest,
		IPartitionEnumerator* pIterator);

	void (*EnumerateElementsAtPoint)(
		ISpatialPartition*,
		SpatialPartitionListMask_t listMask,
		const Vector* pt,
		bool coarseTest,
		IPartitionEnumerator* pIterator);

	void (*SuppressLists)(ISpatialPartition*, SpatialPartitionListMask_t nListMask, bool bSuppress);
	SpatialPartitionListMask_t (*GetSuppressedLists)(ISpatialPartition*);
	void (*RenderAllObjectsInTree)(ISpatialPartition*, float flTime);
	void (*RenderObjectsInPlayerLeafs)(ISpatialPartition*, const Vector* vecPlayerMin, const Vector* vecPlayerMax, float flTime);
	void (*RenderLeafsForRayTraceStart)(ISpatialPartition*, float flTime);
	void (*RenderLeafsForRayTraceEnd)(ISpatialPartition*);
	void (*RenderLeafsForHullTraceStart)(ISpatialPartition*, float flTime);
	void (*RenderLeafsForHullTraceEnd)(ISpatialPartition*);
	void (*RenderLeafsForBoxStart)(ISpatialPartition*, float flTime);
	void (*RenderLeafsForBoxEnd)(ISpatialPartition*);
	void (*RenderLeafsForSphereStart)(ISpatialPartition*, float flTime);
	void (*RenderLeafsForSphereEnd)(ISpatialPartition*);
	void (*RenderObjectsInBox)(ISpatialPartition*, const Vector* vecMin, const Vector* vecMax, float flTime);
	void (*RenderObjectsInSphere)(ISpatialPartition*, const Vector* vecCenter, float flRadius, float flTime);
	void (*RenderObjectsAlongRay)(ISpatialPartition*, const Ray_t* ray, float flTime);
	void (*ReportStats)(ISpatialPartition*, const char* pFileName);
	void (*InstallQueryCallback)(ISpatialPartition*, IPartitionQueryCallback* pCallback);
} ISpatialPartition_VMT;

struct ISpatialPartition
{
	ISpatialPartition_VMT* vmt;
};

#endif
