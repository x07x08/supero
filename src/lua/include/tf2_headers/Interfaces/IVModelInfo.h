#if !defined(IntfIVModelInfo_Included)
#define IntfIVModelInfo_Included

#include "Types.h"
#include "Misc/CommonMacros.h"

typedef struct IMaterial IMaterial;
typedef struct KeyValues KeyValues;
typedef struct vcollide_t vcollide_t;
typedef struct model_t model_t;
typedef struct CGameTrace CGameTrace;
typedef struct cplane_t cplane_t;
typedef CGameTrace trace_t;
typedef struct studiohdr_t studiohdr_t;
typedef struct virtualmodel_t virtualmodel_t;
typedef struct virtualterrainparams_t virtualterrainparams_t;
typedef struct CPhysCollide CPhysCollide;
typedef unsigned short MDLHandle_t;
typedef struct CUtlBuffer CUtlBuffer;
typedef struct IClientRenderable IClientRenderable;

typedef struct CRefCountedModelIndex CRefCountedModelIndex;

typedef struct IModelLoadCallback IModelLoadCallback;

typedef struct IModelLoadCallback_VMT
{
	void (*OnModelLoadComplete)(IModelLoadCallback*, const model_t* pModel);
} IModelLoadCallback_VMT;

struct IModelLoadCallback
{
	IModelLoadCallback_VMT* vmt;
};

struct CRefCountedModelIndex
{
	int m_nIndex;
};

typedef struct IVModelInfo IVModelInfo;

typedef struct IVModelInfo_VMT
{
	//virtual ~IVModelInfo(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	const model_t* (*GetModel)(IVModelInfo*, int modelindex);
	int (*GetModelIndex)(IVModelInfo*, const char* name); // const
	const char* (*GetModelName)(IVModelInfo*, const model_t* model); // const
	vcollide_t* (*GetVCollide)(IVModelInfo*, const model_t* model);
	vcollide_t* (*GetVCollideIndex)(IVModelInfo*, int modelindex);
	void (*GetModelBounds)(IVModelInfo*, const model_t* model, Vector* mins, Vector* maxs); // const
	void (*GetModelRenderBounds)(IVModelInfo*, const model_t* model, Vector* mins, Vector* maxs); // const
	int (*GetModelFrameCount)(IVModelInfo*, const model_t* model); // const
	int (*GetModelType)(IVModelInfo*, const model_t* model); // const
	void* (*GetModelExtraData)(IVModelInfo*, const model_t* model);
	bool (*ModelHasMaterialProxy)(IVModelInfo*, const model_t* model); // const
	bool (*IsTranslucent)(IVModelInfo*, model_t const* model); // const
	bool (*IsTranslucentTwoPass)(IVModelInfo*, const model_t* model); // const
	void (*RecomputeTranslucency)(IVModelInfo*, const model_t* model, int nSkin, int nBody, void* pClientRenderable, float fInstanceAlphaModulate);
	int (*GetModelMaterialCount)(IVModelInfo*, const model_t* model); // const
	void (*GetModelMaterials)(IVModelInfo*, const model_t* model, int count, IMaterial** ppMaterial);
	bool (*IsModelVertexLit)(IVModelInfo*, const model_t* model); // const
	const char* (*GetModelKeyValueText)(IVModelInfo*, const model_t* model);
	bool (*GetModelKeyValue)(IVModelInfo*, const model_t* model, CUtlBuffer* buf);
	float (*GetModelRadius)(IVModelInfo*, const model_t* model);
	const studiohdr_t* (*FindModel)(IVModelInfo*, const studiohdr_t* pStudioHdr, void** cache, const char* modelname); // const
	const studiohdr_t* (*FindModelCache)(IVModelInfo*, void* cache); // const
	virtualmodel_t* (*GetVirtualModel)(IVModelInfo*, const studiohdr_t* pStudioHdr); // const
	byte* (*GetAnimBlock)(IVModelInfo*, const studiohdr_t* pStudioHdr, int iBlock); // const
	void (*GetModelMaterialColorAndLighting)(IVModelInfo*, const model_t* model, Vector const* origin, QAngle const* angles, trace_t* pTrace, Vector* lighting, Vector* matColor);
	void (*GetIlluminationPoint)(IVModelInfo*, const model_t* model, IClientRenderable* pRenderable, Vector const* origin, QAngle const* angles, Vector* pLightingCenter);
	int (*GetModelContents)(IVModelInfo*, int modelIndex);
	studiohdr_t* (*GetStudiomodel)(IVModelInfo*, const model_t* mod);
	int (*GetModelSpriteWidth)(IVModelInfo*, const model_t* model); // const
	int (*GetModelSpriteHeight)(IVModelInfo*, const model_t* model); // const
	void (*SetLevelScreenFadeRange)(IVModelInfo*, float flMinSize, float flMaxSize);
	void (*GetLevelScreenFadeRange)(IVModelInfo*, float* pMinArea, float* pMaxArea); // const
	void (*SetViewScreenFadeRange)(IVModelInfo*, float flMinSize, float flMaxSize);
	unsigned char (*ComputeLevelScreenFade)(IVModelInfo*, const Vector* vecAbsOrigin, float flRadius, float flFadeScale); // const
	unsigned char (*ComputeViewScreenFade)(IVModelInfo*, const Vector* vecAbsOrigin, float flRadius, float flFadeScale); // const
	int (*GetAutoplayList)(IVModelInfo*, const studiohdr_t* pStudioHdr, unsigned short** pAutoplayList); // const
	CPhysCollide* (*GetCollideForVirtualTerrain)(IVModelInfo*, int index);
	bool (*IsUsingFBTexture)(IVModelInfo*, const model_t* model, int nSkin, int nBody, void* pClientRenderable); // const // IClientRenderable*
	// IVModelInfo::FindOrLoadModel is now obsolte
	const model_t* (*FindOrLoadModel)(IVModelInfo*, const char* name);
	// IVModelInfo::InitDynamicModels is now obsolte
	void (*InitDynamicModels)(IVModelInfo*);
	// IVModelInfo::ShutdownDynamicModels is now obsolte
	void (*ShutdownDynamicModels)(IVModelInfo*);
	// IVModelInfo::AddDynamicModel is now obsolte
	void (*AddDynamicModel)(IVModelInfo*, const char* name, int nModelIndex);
	// IVModelInfo::ReferenceModel is now obsolte
	void (*ReferenceModel)(IVModelInfo*, int modelindex);
	// IVModelInfo::UnreferenceModel is now obsolte
	void (*UnreferenceModel)(IVModelInfo*, int modelindex);
	// IVModelInfo::CleanupDynamicModels is now obsolte
	void (*CleanupDynamicModels)(IVModelInfo*, bool bForce);
	MDLHandle_t (*GetCacheHandle)(IVModelInfo*, const model_t* model); // const
	int (*GetBrushModelPlaneCount)(IVModelInfo*, const model_t* model); // const
	void (*GetBrushModelPlane)(IVModelInfo*, const model_t* model, int nIndex, cplane_t* plane, Vector* pOrigin); // const
	int (*GetSurfacepropsForVirtualTerrain)(IVModelInfo*, int index);
	void (*OnLevelChange)(IVModelInfo*);
	int (*GetModelClientSideIndex)(IVModelInfo*, const char* name); // const
	int (*RegisterDynamicModel)(IVModelInfo*, const char* name, bool bClientSide);
	bool (*IsDynamicModelLoading)(IVModelInfo*, int modelIndex);
	void (*AddRefDynamicModel)(IVModelInfo*, int modelIndex);
	void (*ReleaseDynamicModel)(IVModelInfo*, int modelIndex);
	bool (*RegisterModelLoadCallback)(IVModelInfo*, int modelindex, IModelLoadCallback* pCallback, bool bCallImmediatelyIfLoaded);
	void (*UnregisterModelLoadCallback)(IVModelInfo*, int modelindex, IModelLoadCallback* pCallback);
} IVModelInfo_VMT;

struct IVModelInfo
{
	IVModelInfo_VMT* vmt;
};

typedef struct IVModelInfoClient IVModelInfoClient;

typedef struct IVModelInfoClient_VMT
{
	IVModelInfo_VMT vmt_IVModelInfo;

	void (*OnDynamicModelsStringTableChange)(IVModelInfoClient*, int nStringIndex, const char* pString, const void* pData);
	// const model_t* (*FindOrLoadModel)(IVModelInfoClient*, const char* name);
} IVModelInfoClient_VMT;

struct IVModelInfoClient
{
	IVModelInfoClient_VMT* vmt;
};

#define IVModelInfoClient_REFL "IVModelInfoClient"
#define IVModelInfoClient_SIGNATURE "VModelInfoClient006"
#define IVModelInfoClient_MODULE "engine"

#endif
