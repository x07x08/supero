#if !defined(IntfIStudioRender_Included)
#define IntfIStudioRender_Included

#include "Interfaces/IMaterialSystem.h"
#include "Main/IMaterial.h"
#include "Misc/IMDLCache.h"
#include "Misc/Studio.h"
#include "Main/UtlVector.h"
#include "Types.h"

typedef struct studiohdr_t studiohdr_t;
typedef struct studiomeshdata_t studiomeshdata_t;
typedef struct LightDesc_t LightDesc_t;
typedef struct IMaterial IMaterial;
typedef struct studiohwdata_t studiohwdata_t;
typedef struct Ray_t Ray_t;
typedef struct IMaterialSystem IMaterialSystem;
typedef struct IMesh IMesh;
typedef struct vertexFileHeader_t vertexFileHeader_t;
typedef struct FlashlightState_t FlashlightState_t;
typedef struct FileHeader_t FileHeader_t;
typedef struct IPooledVBAllocator IPooledVBAllocator;

typedef void (*StudioRender_Printf_t)(const char* fmt, ...);

typedef struct StudioRenderConfig_t
{
	float fEyeShiftX;
	float fEyeShiftY;
	float fEyeShiftZ;
	float fEyeSize;
	float fEyeGlintPixelWidthLODThreshold;
	int maxDecalsPerModel;
	int drawEntities;
	int skin;
	int fullbright;
	bool bEyeMove : 1;
	bool bSoftwareSkin : 1;
	bool bNoHardware : 1;
	bool bNoSoftware : 1;
	bool bTeeth : 1;
	bool bEyes : 1;
	bool bFlex : 1;
	bool bWireframe : 1;
	bool bDrawNormals : 1;
	bool bDrawTangentFrame : 1;
	bool bDrawZBufferedWireframe : 1;
	bool bSoftwareLighting : 1;
	bool bShowEnvCubemapOnly : 1;
	bool bWireframeDecals : 1;
	int m_nReserved[4];
} StudioRenderConfig_t;

typedef void* StudioDecalHandle_t;
#define STUDIORENDER_DECAL_INVALID ((StudioDecalHandle_t)0)

enum
{
	ADDDECAL_TO_ALL_LODS = -1
};

enum
{
	STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
	STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
	STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
	STUDIORENDER_DRAW_GROUP_MASK = 0x03,
	STUDIORENDER_DRAW_NO_FLEXES = 0x04,
	STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,
	STUDIORENDER_DRAW_ACCURATETIME = 0x10,
	STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
	STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,
	STUDIORENDER_DRAW_WIREFRAME = 0x80,
	STUDIORENDER_DRAW_ITEM_BLINK = 0x100,
	STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,
	STUDIORENDER_SSAODEPTHTEXTURE = 0x1000,
	STUDIORENDER_GENERATE_STATS = 0x8000
};

#define VERTEX_TEXCOORD0_2D (((uint64)2) << (TEX_COORD_SIZE_BIT + (3 * 0)))

typedef enum MaterialVertexFormat_t
{
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_SKINNED_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_BONEWEIGHT(2) | VERTEX_BONE_INDEX,
	MATERIAL_VERTEX_FORMAT_MODEL = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D | VERTEX_USERDATA_SIZE(4),
	MATERIAL_VERTEX_FORMAT_MODEL_DX7 = (VertexFormat_t)VERTEX_POSITION | VERTEX_COLOR | VERTEX_NORMAL | VERTEX_TEXCOORD0_2D,
	MATERIAL_VERTEX_FORMAT_COLOR = (VertexFormat_t)VERTEX_SPECULAR
} MaterialVertexFormat_t;

typedef enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE
} OverrideType_t;

enum
{
	USESHADOWLOD = -2
};

#define MAX_DRAW_MODEL_INFO_MATERIALS 8

typedef struct DrawModelResults_t
{
	int m_ActualTriCount;
	int m_TextureMemoryBytes;
	int m_NumHardwareBones;
	int m_NumBatches;
	int m_NumMaterials;
	int m_nLODUsed;
	int m_flLODMetric;
	// CFastTimer m_RenderTime;
	// CUtlVector<IMaterial *> m_Materials;
} DrawModelResults_t;

typedef struct ColorTexelsInfo_t
{
	int m_nWidth;
	int m_nHeight;
	int m_nMipmapCount;
	ImageFormat m_ImageFormat;
	int m_nByteCount;
	byte* m_pTexelData;
} ColorTexelsInfo_t;

typedef struct ColorMeshInfo_t
{
	IMesh* m_pMesh;
	IPooledVBAllocator* m_pPooledVBAllocator;
	int m_nVertOffsetInBytes;
	int m_nNumVerts;
	ITexture* m_pLightmap;
	ColorTexelsInfo_t* m_pLightmapData;
} ColorMeshInfo_t;

typedef struct DrawModelInfo_t
{
	studiohdr_t* m_pStudioHdr;
	studiohwdata_t* m_pHardwareData;
	StudioDecalHandle_t m_Decals;
	int m_Skin;
	int m_Body;
	int m_HitboxSet;
	void* m_pClientEntity;
	int m_Lod;
	ColorMeshInfo_t* m_pColorMeshes;
	bool m_bStaticLighting;
	Vector m_vecAmbientCube[6];
	int m_nLocalLightCount;
	LightDesc_t m_LocalLightDescs[4];
} DrawModelInfo_t;

typedef struct GetTriangles_Vertex_t
{
	Vector m_Position;
	Vector m_Normal;
	Vector4D m_TangentS;
	Vector2D m_TexCoord;
	Vector4D m_BoneWeight;
	int m_BoneIndex[4];
	int m_NumBones;
} GetTriangles_Vertex_t;

typedef struct GetTriangles_MaterialBatch_t
{
	IMaterial* m_pMaterial;
	CUtlVector m_Verts;
	CUtlVector m_TriListIndices;
} GetTriangles_MaterialBatch_t;

typedef struct GetTriangles_Output_t
{
	CUtlVector m_MaterialBatches;
	matrix3x4 m_PoseToWorld[MAXSTUDIOBONES];
} GetTriangles_Output_t;

typedef struct model_array_instance_t
{
	matrix3x4 modelToWorld;
} model_array_instance_t;

typedef struct IStudioDataCache IStudioDataCache;

typedef struct IStudioDataCache_VMT
{
	IAppSystem vmt_IAppSystem;

	bool (*VerifyHeaders)(IStudioDataCache*, studiohdr_t* pStudioHdr);
	vertexFileHeader_t* (*CacheVertexData)(IStudioDataCache*, studiohdr_t* pStudioHdr);
} IStudioDataCache_VMT;

struct IStudioDataCache
{
	IStudioDataCache_VMT* vmt;
};

typedef struct IStudioRender IStudioRender;

typedef struct IStudioRender_VMT
{
	IAppSystem vmt_IAppSystem;

	void (*BeginFrame)(IStudioRender*);
	void (*EndFrame)(IStudioRender*);
	void (*Mat_Stub)(IStudioRender*, IMaterialSystem* pMatSys);
	void (*UpdateConfig)(IStudioRender*, const StudioRenderConfig_t* config);
	void (*GetCurrentConfig)(IStudioRender*, StudioRenderConfig_t* config);
	bool (*LoadModel)(IStudioRender*, studiohdr_t* pStudioHdr, void* pVtxData, studiohwdata_t* pHardwareData);
	void (*UnloadModel)(IStudioRender*, studiohwdata_t* pHardwareData);
	void (*RefreshStudioHdr)(IStudioRender*, studiohdr_t* pStudioHdr, studiohwdata_t* pHardwareData);
	void (*SetEyeViewTarget)(IStudioRender*, const studiohdr_t* pStudioHdr, int nBodyIndex, const Vector* worldPosition);
	int (*GetNumAmbientLightSamples)(IStudioRender*);
	const Vector* (*GetAmbientLightDirections)(IStudioRender*);
	void (*SetAmbientLightColors)(IStudioRender*, const Vector4D* pAmbientOnlyColors);
	void (*SetAmbientLightColors)(IStudioRender*, const Vector* pAmbientOnlyColors);
	void (*SetLocalLights)(IStudioRender*, int numLights, const LightDesc_t* pLights);
	void (*SetViewState)(IStudioRender*, const Vector* viewOrigin, const Vector* viewRight, const Vector* viewUp, const Vector* viewPlaneNormal);
	void (*LockFlexWeights)(IStudioRender*, int nWeightCount, float** ppFlexWeights, float** ppFlexDelayedWeights);
	void (*UnlockFlexWeights)(IStudioRender*);
	matrix3x4* (*LockBoneMatrices)(IStudioRender*, int nBoneCount);
	void (*UnlockBoneMatrices)(IStudioRender*);
	int (*GetNumLODs)(IStudioRender*, const studiohwdata_t* hardwareData); // const
	float (*GetLODSwitchValue)(IStudioRender*, const studiohwdata_t* hardwareData, int lod); // const
	void (*SetLODSwitchValue)(IStudioRender*, studiohwdata_t* hardwareData, int lod, float switchValue);
	void (*SetColorModulation)(IStudioRender*, float const* pColor);
	void (*SetAlphaModulation)(IStudioRender*, float flAlpha);
	void (*DrawModel)(IStudioRender*, DrawModelResults_t* pResults, const DrawModelInfo_t* info, matrix3x4* pBoneToWorld, float* pFlexWeights, float* pFlexDelayedWeights, const Vector* modelOrigin, int flags);
	void (*DrawModelStaticProp)(IStudioRender*, const DrawModelInfo_t* drawInfo, const matrix3x4* modelToWorld, int flags);
	void (*DrawStaticPropDecals)(IStudioRender*, const DrawModelInfo_t* drawInfo, const matrix3x4* modelToWorld);
	void (*DrawStaticPropShadows)(IStudioRender*, const DrawModelInfo_t* drawInfo, const matrix3x4* modelToWorld, int flags);
	void (*ForcedMaterialOverride)(IStudioRender*, IMaterial* newMaterial, OverrideType_t nOverrideType);
	StudioDecalHandle_t (*CreateDecalList)(IStudioRender*, studiohwdata_t* pHardwareData);
	void (*DestroyDecalList)(IStudioRender*, StudioDecalHandle_t handle);
	void (*AddDecal)(IStudioRender*, StudioDecalHandle_t handle, studiohdr_t* pStudioHdr, matrix3x4* pBoneToWorld, const Ray_t* ray, const Vector* decalUp, IMaterial* pDecalMaterial, float radius, int body, bool noPokethru, int maxLODToDecal);
	void (*ComputeLighting)(IStudioRender*, const Vector* pAmbient, int lightCount, LightDesc_t* pLights, const Vector* pt, const Vector* normal, Vector* lighting);
	void (*ComputeLightingConstDirectional)(IStudioRender*, const Vector* pAmbient, int lightCount, LightDesc_t* pLights, const Vector* pt, const Vector* normal, Vector* lighting, float flDirectionalAmount);
	void (*AddShadow)(IStudioRender*, IMaterial* pMaterial, void* pProxyData, FlashlightState_t* m_pFlashlightState, VMatrix* pWorldToTexture, ITexture* pFlashlightDepthTexture);
	void (*ClearAllShadows)(IStudioRender*);
	int (*ComputeModelLod)(IStudioRender*, studiohwdata_t* pHardwareData, float unitSphereSize, float* pMetric);
	void (*GetPerfStats)(IStudioRender*, DrawModelResults_t* pResults, const DrawModelInfo_t* info, CUtlBuffer* pSpewBuf); // const
	void (*GetTriangles)(IStudioRender*, const DrawModelInfo_t* info, matrix3x4* pBoneToWorld, GetTriangles_Output_t* out);
	int (*GetMaterialList)(IStudioRender*, studiohdr_t* pStudioHdr, int count, IMaterial** ppMaterials);
	int (*GetMaterialListFromBodyAndSkin)(IStudioRender*, MDLHandle_t studio, int nSkin, int nBody, int nCountOutputMaterials, IMaterial** ppOutputMaterials);
	void (*DrawModelArray)(IStudioRender*, const DrawModelInfo_t* drawInfo, int arrayCount, model_array_instance_t* pInstanceData, int instanceStride, int flags);
	void (*GetMaterialOverride)(IStudioRender*, IMaterial** ppOutForcedMaterial, OverrideType_t* pOutOverrideType);
} IStudioRender_VMT;

struct IStudioRender
{
	IStudioRender_VMT* vmt;
};

#define IStudioRender_REFL "IStudioRender"
#define IStudioRender_SIGNATURE "VStudioRender025"
#define IStudioRender_MODULE "studiorender"

#endif
