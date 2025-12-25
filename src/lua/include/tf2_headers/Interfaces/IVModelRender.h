#if !defined(IntfIVModelRender_Included)
#define IntfIVModelRender_Included

#include "Interfaces/Interface.h"
#include "Interfaces/IStudioRender.h"
#include "Types.h"

typedef struct IClientRenderable IClientRenderable;
typedef struct model_t model_t;
typedef void* LightCacheHandle_t;

typedef struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pStudioHWData;
	IClientRenderable* m_pRenderable;
	const matrix3x4* m_pModelToWorld;
	StudioDecalHandle_t m_decals;
	int m_drawFlags;
	int m_lod;
} DrawModelState_t;

typedef unsigned short ModelInstanceHandle_t;

enum
{
	MODEL_INSTANCE_INVALID = (ModelInstanceHandle_t)~0
};

typedef struct ModelRenderInfo_t
{
	Vector origin;
	QAngle angles;
	IClientRenderable* pRenderable;
	const model_t* pModel;
	const matrix3x4* pModelToWorld;
	const matrix3x4* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
} ModelRenderInfo_t;

typedef struct StaticPropRenderInfo_t
{
	const matrix3x4* pModelToWorld;
	const model_t* pModel;
	IClientRenderable* pRenderable;
	Vector* pLightingOrigin;
	short skin;
	ModelInstanceHandle_t instance;
} StaticPropRenderInfo_t;

typedef struct IVModelRender IVModelRender;

typedef struct IVModelRender_VMT
{
	int (*DrawModel)(IVModelRender*,
	              int flags,
	              IClientRenderable* pRenderable,
	              ModelInstanceHandle_t instance,
	              int entity_index,
	              const model_t* model,
	              Vector const* origin,
	              QAngle const* angles,
	              int skin,
	              int body,
	              int hitboxset,
	              const matrix3x4* modelToWorld,
	              const matrix3x4* pLightingOffset);

	void (*ForcedMaterialOverride)(IVModelRender*, IMaterial* newMaterial, OverrideType_t nOverrideType);
	void (*SetViewTarget)(IVModelRender*, const CStudioHdr* pStudioHdr, int nBodyIndex, const Vector* target);
	ModelInstanceHandle_t (*CreateInstance)(IVModelRender*, IClientRenderable* pRenderable, LightCacheHandle_t* pCache);
	void (*DestroyInstance)(IVModelRender*, ModelInstanceHandle_t handle);
	void (*SetStaticLighting)(IVModelRender*, ModelInstanceHandle_t handle, LightCacheHandle_t* pHandle);
	LightCacheHandle_t (*GetStaticLighting)(IVModelRender*, ModelInstanceHandle_t handle);
	bool (*ChangeInstance)(IVModelRender*, ModelInstanceHandle_t handle, IClientRenderable* pRenderable);
	void (*AddDecal)(IVModelRender*, ModelInstanceHandle_t handle, Ray_t const* ray, Vector const* decalUp, int decalIndex, int body, bool noPokeThru, int maxLODToDecal);
	void (*RemoveAllDecals)(IVModelRender*, ModelInstanceHandle_t handle);
	void (*RemoveAllDecalsFromAllModels)(IVModelRender*);
	matrix3x4* (*DrawModelShadowSetup)(IVModelRender*, IClientRenderable* pRenderable, int body, int skin, DrawModelInfo_t* pInfo, matrix3x4* pCustomBoneToWorld);
	void (*DrawModelShadow)(IVModelRender*, IClientRenderable* pRenderable, const DrawModelInfo_t* info, matrix3x4* pCustomBoneToWorld);
	bool (*RecomputeStaticLighting)(IVModelRender*, ModelInstanceHandle_t handle);
	void (*ReleaseAllStaticPropColorData)(IVModelRender*);
	void (*RestoreAllStaticPropColorData)(IVModelRender*);
	int (*DrawModelEx)(IVModelRender*, ModelRenderInfo_t* pInfo);
	int (*DrawModelExStaticProp)(IVModelRender*, ModelRenderInfo_t* pInfo);
	bool (*DrawModelSetup)(IVModelRender*, ModelRenderInfo_t* pInfo, DrawModelState_t* pState, matrix3x4* pCustomBoneToWorld, matrix3x4** ppBoneToWorldOut);
	void (*DrawModelExecute)(IVModelRender*, const DrawModelState_t* state, const ModelRenderInfo_t* pInfo, matrix3x4* pCustomBoneToWorld);
	void (*SetupLighting)(IVModelRender*, const Vector* vecCenter);
	int (*DrawStaticPropArrayFast)(IVModelRender*, StaticPropRenderInfo_t* pProps, int count, bool bShadowDepth);
	void (*SuppressEngineLighting)(IVModelRender*, bool bSuppress);
	void (*SetupColorMeshes)(IVModelRender*, int nTotalVerts);
	void (*AddColoredDecal)(IVModelRender*, ModelInstanceHandle_t handle, Ray_t const* ray, Vector const* decalUp, int decalIndex, int body, Color_t cColor, bool noPokeThru, int maxLODToDecal);
	void (*GetMaterialOverride)(IVModelRender*, IMaterial** ppOutForcedMaterial, OverrideType_t* pOutOverrideType);
} IVModelRender_VMT;

struct IVModelRender
{
	IVModelRender_VMT* vmt;
};

#define IVModelRender_REFL "IVModelRender"
#define IVModelRender_SIGNATURE "VEngineModel016"
#define IVModelRender_MODULE "engine"

#endif
