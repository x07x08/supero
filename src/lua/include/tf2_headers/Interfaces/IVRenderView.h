#if !defined(IntfIVRenderView_Included)
#define IntfIVRenderView_Included

#include "Interfaces/IMaterialSystem.h"
#include "Misc/BaseTypes.h"
#include "Misc/IRefCounted.h"
#include "Misc/VPlane.h"
#include "Definitions.h"

typedef struct IClientRenderable IClientRenderable;
typedef struct IClientEntity IClientEntity;
typedef struct CViewSetup CViewSetup;
typedef struct model_t model_t;

enum
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_TOP = 2,
	FRUSTUM_BOTTOM = 3,
	FRUSTUM_NEARZ = 4,
	FRUSTUM_FARZ = 5,
	FRUSTUM_NUMPLANES = 6
};

enum
{
	DRAWWORLDLISTS_DRAW_STRICTLYABOVEWATER = 0x001,
	DRAWWORLDLISTS_DRAW_STRICTLYUNDERWATER = 0x002,
	DRAWWORLDLISTS_DRAW_INTERSECTSWATER = 0x004,
	DRAWWORLDLISTS_DRAW_WATERSURFACE = 0x008,
	DRAWWORLDLISTS_DRAW_SKYBOX = 0x010,
	DRAWWORLDLISTS_DRAW_CLIPSKYBOX = 0x020,
	DRAWWORLDLISTS_DRAW_SHADOWDEPTH = 0x040,
	DRAWWORLDLISTS_DRAW_REFRACTION = 0x080,
	DRAWWORLDLISTS_DRAW_REFLECTION = 0x100,
	DRAWWORLDLISTS_DRAW_SSAO = 0x800
};

enum
{
	MAT_SORT_GROUP_STRICTLY_ABOVEWATER = 0,
	MAT_SORT_GROUP_STRICTLY_UNDERWATER,
	MAT_SORT_GROUP_INTERSECTS_WATER_SURFACE,
	MAT_SORT_GROUP_WATERSURFACE,
	MAX_MAT_SORT_GROUPS
};

typedef enum ERenderDepthMode
{
	DEPTH_MODE_NORMAL = 0,
	DEPTH_MODE_SHADOW = 1,
	DEPTH_MODE_SSA0 = 2,
	DEPTH_MODE_OVERRIDE = 3,
	DEPTH_MODE_MAX
} ERenderDepthMode;

typedef VPlane Frustum[FRUSTUM_NUMPLANES];

typedef unsigned short LeafIndex_t;
typedef short LeafFogVolume_t;

enum
{
	INVALID_LEAF_INDEX = (LeafIndex_t)~0
};

typedef struct WorldListInfo_t
{
	int m_ViewFogVolume;
	int m_LeafCount;
	LeafIndex_t* m_pLeafList;
	LeafFogVolume_t* m_pLeafFogVolume;
} WorldListInfo_t;

typedef IRefCounted IWorldRenderList;

typedef struct VisibleFogVolumeInfo_t
{
	int m_nVisibleFogVolume;
	int m_nVisibleFogVolumeLeaf;
	bool m_bEyeInFogVolume;
	float m_flDistanceToWater;
	float m_flWaterHeight;
	IMaterial* m_pFogVolumeMaterial;
} VisibleFogVolumeInfo_t;

typedef struct BrushVertex_t
{
	Vector m_Pos;
	Vector m_Normal;
	Vector m_TangentS;
	Vector m_TangentT;
	Vector2D m_TexCoord;
	Vector2D m_LightmapCoord;
} BrushVertex_t;

typedef struct VisOverrideData_t
{
	Vector m_vecVisOrigin;
	float m_fDistToAreaPortalTolerance;
} VisOverrideData_t;

typedef struct IBrushSurface IBrushSurface;

typedef struct IBrushSurface_VMT
{
	void (*ComputeTextureCoordinate)(IBrushSurface*, Vector const* worldPos, Vector2D* texCoord);
	void (*ComputeLightmapCoordinate)(IBrushSurface*, Vector const* worldPos, Vector2D* lightmapCoord);
	int (*GetVertexCount)(IBrushSurface*); // const
	void (*GetVertexData)(IBrushSurface*, BrushVertex_t* pVerts);
	IMaterial* (*GetMaterial)(IBrushSurface*);
} IBrushSurface_VMT;

struct IBrushSurface
{
	IBrushSurface_VMT* vmt;
};

typedef struct IBrushRenderer IBrushRenderer;

typedef struct IBrushRenderer_VMT
{
	bool (*RenderBrushModelSurface)(IBrushRenderer*, IClientEntity* pBaseEntity, IBrushSurface* pBrushSurface);
} IBrushRenderer_VMT;

struct IBrushRenderer
{
	IBrushRenderer_VMT* vmt;
};

#define MAX_VIS_LEAVES 32

typedef enum DrawBrushModelMode_t
{
	DBM_DRAW_ALL = 0,
	DBM_DRAW_OPAQUE_ONLY,
	DBM_DRAW_TRANSLUCENT_ONLY
} DrawBrushModelMode_t;

enum
{
	VIEW_SETUP_VIS_EX_RETURN_FLAGS_USES_RADIAL_VIS = 0x00000001
};

typedef struct IVRenderView IVRenderView;

typedef struct IVRenderView_VMT
{
	void (*DrawBrushModel)(IVRenderView*, IClientEntity* baseentity, model_t* model, const Vector* origin, const QAngle* angles, bool bUnused);
	void (*DrawIdentityBrushModel)(IVRenderView*, IWorldRenderList* pList, model_t* model);
	void (*TouchLight)(IVRenderView*, struct dlight_t* light);
	void (*Draw3DDebugOverlays)(IVRenderView*);
	void (*SetBlend)(IVRenderView*, float blend);
	float (*GetBlend)(IVRenderView*);
	void (*SetColorModulation)(IVRenderView*, float const* blend);
	void (*GetColorModulation)(IVRenderView*, float* blend);
	void (*SceneBegin)(IVRenderView*);
	void (*SceneEnd)(IVRenderView*);
	void (*GetVisibleFogVolume)(IVRenderView*, const Vector* eyePoint, VisibleFogVolumeInfo_t* pInfo);
	IWorldRenderList* (*CreateWorldList)(IVRenderView*);
	void (*BuildWorldLists)(IVRenderView*, IWorldRenderList* pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData, bool bShadowDepth, float* pReflectionWaterHeight);
	void (*DrawWorldLists)(IVRenderView*, IWorldRenderList* pList, unsigned long flags, float waterZAdjust);
	void (*DrawTopView)(IVRenderView*, bool enable);
	void (*TopViewBounds)(IVRenderView*, Vector2D const* mins, Vector2D const* maxs);
	void (*DrawLights)(IVRenderView*);
	void (*DrawMaskEntities)(IVRenderView*);
	void (*DrawTranslucentSurfaces)(IVRenderView*, IWorldRenderList* pList, int sortIndex, unsigned long flags, bool bShadowDepth);
	void (*DrawLineFile)(IVRenderView*);
	void (*DrawLightmaps)(IVRenderView*, IWorldRenderList* pList, int pageId);
	void (*ViewSetupVis)(IVRenderView*, bool novis, int numorigins, const Vector origin[]);
	bool (*AreAnyLeavesVisible)(IVRenderView*, int* leafList, int nLeaves);
	void (*VguiPaint)(IVRenderView*);
	void (*ViewDrawFade)(IVRenderView*, byte* color, IMaterial* pMaterial);
	void (*OLD_SetProjectionMatrix)(IVRenderView*, float fov, float zNear, float zFar);
	colorVec (*GetLightAtPoint)(IVRenderView*, Vector* pos);
	int (*GetViewEntity)(IVRenderView*);
	float (*GetFieldOfView)(IVRenderView*);
	unsigned char** (*GetAreaBits)(IVRenderView*);
	void (*SetFogVolumeState)(IVRenderView*, int nVisibleFogVolume, bool bUseHeightFog);
	void (*InstallBrushSurfaceRenderer)(IVRenderView*, IBrushRenderer* pBrushRenderer);
	void (*DrawBrushModelShadow)(IVRenderView*, IClientRenderable* pRenderable);
	bool (*LeafContainsTranslucentSurfaces)(IVRenderView*, IWorldRenderList* pList, int sortIndex, unsigned long flags);
	bool (*DoesBoxIntersectWaterVolume)(IVRenderView*, const Vector* mins, const Vector* maxs, int leafWaterDataID);
	void (*SetAreaState)(IVRenderView*, unsigned char chAreaBits[MAX_AREA_STATE_BYTES], unsigned char chAreaPortalBits[MAX_AREA_PORTAL_STATE_BYTES]);
	void (*VGui_Paint)(IVRenderView*, int mode);
	void (*Push3DView)(IVRenderView*, const CViewSetup* view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes);
	void (*Push2DView)(IVRenderView*, const CViewSetup* view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes);
	void (*PopView)(IVRenderView*, Frustum frustumPlanes);
	void (*SetMainView)(IVRenderView*, const Vector* vecOrigin, const QAngle* angles);
	void (*ViewSetupVisEx)(IVRenderView*, bool novis, int numorigins, const Vector origin[], unsigned int* returnFlags);
	void (*OverrideViewFrustum)(IVRenderView*, Frustum custom);
	void (*DrawBrushModelShadowDepth)(IVRenderView*, IClientEntity* baseentity, model_t* model, const Vector* origin, const QAngle* angles, ERenderDepthMode DepthMode);
	void (*UpdateBrushModelLightmap)(IVRenderView*, model_t* model, IClientRenderable* pRenderable);
	void (*BeginUpdateLightmaps)(IVRenderView*);
	void (*EndUpdateLightmaps)(IVRenderView*);
	void (*OLD_SetOffCenterProjectionMatrix)(IVRenderView*, float fov, float zNear, float zFar, float flAspectRatio, float flBottom, float flTop, float flLeft, float flRight);
	void (*OLD_SetProjectionMatrixOrtho)(IVRenderView*, float left, float top, float right, float bottom, float zNear, float zFar);
	void (*Push3DView)(IVRenderView*, const CViewSetup* view, int nFlags, ITexture* pRenderTarget, Frustum frustumPlanes, ITexture* pDepthTexture);
	void (*GetMatricesForView)(IVRenderView*, const CViewSetup* view, VMatrix* pWorldToView, VMatrix* pViewToProjection, VMatrix* pWorldToProjection, VMatrix* pWorldToPixels);
	void (*DrawBrushModelEx)(IVRenderView*, IClientEntity* baseentity, model_t* model, const Vector* origin, const QAngle* angles, DrawBrushModelMode_t mode);
} IVRenderView_VMT;

struct IVRenderView
{
	IVRenderView_VMT* vmt;
};

#define IVRenderView_REFL "IVRenderView"
#define IVRenderView_SIGNATURE "VEngineRenderView014"
#define IVRenderView_MODULE "engine"

#endif
