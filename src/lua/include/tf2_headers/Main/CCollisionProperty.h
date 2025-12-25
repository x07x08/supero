#if !defined(MainCCollisionProperty_Included)
#define MainCCollisionProperty_Included

#include "Main/CGameTrace.h"
#include "Misc/ISpatialPartition.h"

//MAKE_SIGNATURE(CCollisionProperty_SetCollisionBounds, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? F3 0F 10 0A", 0x0);
//MAKE_SIGNATURE(CCollisionProperty_CalcNearestPoint, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 49 8B D8 48 8B F9 4C 8D 44 24", 0x0);

typedef struct model_t model_t;

typedef struct ICollideable ICollideable;

typedef struct ICollideable_VMT
{
	void* (*GetEntityHandle)(ICollideable*);
	const Vec3* (*OBBMinsPreScaled)(ICollideable*);
	const Vec3* (*OBBMaxsPreScaled)(ICollideable*);
	const Vec3* (*OBBMins)(ICollideable*);
	const Vec3* (*OBBMaxs)(ICollideable*);
	void (*WorldSpaceTriggerBounds)(ICollideable*, Vec3* pVecWorldMins, Vec3* pVecWorldMaxs);
	bool (*TestCollision)(ICollideable*, const Ray_t* ray, unsigned int fContentsMask, CGameTrace* tr);
	bool (*TestHitboxes)(ICollideable*, const Ray_t* ray, unsigned int fContentsMask, CGameTrace* tr);
	int (*GetCollisionModelIndex)(ICollideable*);
	const model_t* (*GetCollisionModel)(ICollideable*);
	const Vec3* (*GetCollisionOrigin)(ICollideable*);
	const Vec3* (*GetCollisionAngles)(ICollideable*);
	const matrix3x4* (*CollisionToWorldTransform)(ICollideable*);
	SolidType_t (*GetSolid)(ICollideable*);
	int (*GetSolidFlags)(ICollideable*);
	void* (*GetIClientUnknown)(ICollideable*);
	int (*GetCollisionGroup)(ICollideable*);
	void (*WorldSpaceSurroundingBounds)(ICollideable*, Vec3* pVecMins, Vec3* pVecMaxs);
	bool (*ShouldTouchTrigger)(ICollideable*, int triggerSolidFlags);
	const matrix3x4* (*GetRootParentToWorldTransform)(ICollideable*);
} ICollideable_VMT;

struct ICollideable
{
	ICollideable_VMT* vmt;
};

typedef ICollideable_VMT CCollisionProperty_VMT;

typedef struct CCollisionProperty// : public ICollideable
{
	CCollisionProperty_VMT* vmt;

	CBaseEntity* m_pOuter;
	Vec3 m_vecMinsPreScaled;
	Vec3 m_vecMaxsPreScaled;
	Vec3 m_vecMins;
	Vec3 m_vecMaxs;
	float m_flRadius;
	unsigned short m_usSolidFlags;
	SpatialPartitionHandle_t m_Partition;
	unsigned char m_nSurroundType;
	unsigned char m_nSolidType;
	unsigned char m_triggerBloat;
	bool m_bUniformTriggerBloat;
	Vec3 m_vecSpecifiedSurroundingMinsPreScaled;
	Vec3 m_vecSpecifiedSurroundingMaxsPreScaled;
	Vec3 m_vecSpecifiedSurroundingMins;
	Vec3 m_vecSpecifiedSurroundingMaxs;
	Vec3 m_vecSurroundingMins;
	Vec3 m_vecSurroundingMaxs;

	//SIGNATURE_ARGS(SetCollisionBounds, void, CCollisionProperty, (const Vec3& mins, const Vec3& maxs), this, std::ref(mins), std::ref(maxs));
	//SIGNATURE_ARGS(CalcNearestPoint, void, CCollisionProperty, (const Vec3& vecWorldPt, Vec3* pVecNearestWorldPt), this, std::ref(vecWorldPt), pVecNearestWorldPt);
} CCollisionProperty;

#endif
