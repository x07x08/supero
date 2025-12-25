#if !defined(IntfIMaterialSystem_Included)
#define IntfIMaterialSystem_Included

#include "Misc/IAppSystem.h"
#include "Misc/Deformations.h"
#include "Misc/IColorCorrectionSystem.h"
#include "Misc/ImageFormat.h"
#include "Misc/IMaterialSystemHardwareConfig.h"
#include "Misc/IRefCounted.h"
#include "Misc/LightDesc.h"
#include "Misc/TextureGroupNames.h"
#include "Misc/VTF.h"
#include "Types.h"

#define OVERBRIGHT 2.0f
#define OO_OVERBRIGHT (1.0f / 2.0f)
#define GAMMA 2.2f
#define TEXGAMMA 2.2f

typedef struct IMaterial IMaterial;
typedef struct IMesh IMesh;
typedef struct IVertexBuffer IVertexBuffer;
typedef struct IIndexBuffer IIndexBuffer;
typedef struct MaterialSystem_Config_t MaterialSystem_Config_t;
typedef struct ITexture ITexture;
typedef struct ITextureCompositor ITextureCompositor;
typedef struct MaterialSystemHardwareIdentifier_t MaterialSystemHardwareIdentifier_t;
typedef struct KeyValues KeyValues;
typedef struct IShader IShader;
typedef struct IVertexTexture IVertexTexture;
typedef struct IMorph IMorph;
typedef struct ICallQueue ICallQueue;
typedef struct MorphWeight_t MorphWeight_t;
typedef struct IFileList IFileList;

typedef uint64 VertexFormat_t;

#define ABSOLUTE_MINIMUM_DXLEVEL 80

typedef enum ShaderParamType_t
{
	SHADER_PARAM_TYPE_TEXTURE,
	SHADER_PARAM_TYPE_INTEGER,
	SHADER_PARAM_TYPE_COLOR,
	SHADER_PARAM_TYPE_VEC2,
	SHADER_PARAM_TYPE_VEC3,
	SHADER_PARAM_TYPE_VEC4,
	SHADER_PARAM_TYPE_ENVMAP,
	SHADER_PARAM_TYPE_FLOAT,
	SHADER_PARAM_TYPE_BOOL,
	SHADER_PARAM_TYPE_FOURCC,
	SHADER_PARAM_TYPE_MATRIX,
	SHADER_PARAM_TYPE_MATERIAL,
	SHADER_PARAM_TYPE_STRING,
	SHADER_PARAM_TYPE_MATRIX4X2
} ShaderParamType_t;

typedef enum MaterialMatrixMode_t
{
	MATERIAL_VIEW = 0,
	MATERIAL_PROJECTION,
	MATERIAL_TEXTURE0,
	MATERIAL_TEXTURE1,
	MATERIAL_TEXTURE2,
	MATERIAL_TEXTURE3,
	MATERIAL_TEXTURE4,
	MATERIAL_TEXTURE5,
	MATERIAL_TEXTURE6,
	MATERIAL_TEXTURE7,
	MATERIAL_MODEL,
	NUM_MATRIX_MODES = MATERIAL_MODEL + 1,
	NUM_TEXTURE_TRANSFORMS = MATERIAL_TEXTURE7 - MATERIAL_TEXTURE0 + 1
} MaterialMatrixMode_t;

// const int NUM_MODEL_TRANSFORMS = 53;
// const int MATERIAL_MODEL_MAX = MATERIAL_MODEL + NUM_MODEL_TRANSFORMS;

typedef enum MaterialPrimitiveType_t
{
	MATERIAL_POINTS = 0x0,
	MATERIAL_LINES,
	MATERIAL_TRIANGLES,
	MATERIAL_TRIANGLE_STRIP,
	MATERIAL_LINE_STRIP,
	MATERIAL_LINE_LOOP,
	MATERIAL_POLYGON,
	MATERIAL_QUADS,
	MATERIAL_INSTANCED_QUADS,
	MATERIAL_HETEROGENOUS
} MaterialPrimitiveType_t;

typedef enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,
	MATERIAL_PROPERTY_OPACITY,
	MATERIAL_PROPERTY_REFLECTIVITY,
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS
} MaterialPropertyTypes_t;

typedef enum MaterialPropertyOpacityTypes_t
{
	MATERIAL_ALPHATEST = 0,
	MATERIAL_OPAQUE,
	MATERIAL_TRANSLUCENT
} MaterialPropertyOpacityTypes_t;

typedef enum MaterialBufferTypes_t
{
	MATERIAL_FRONT = 0,
	MATERIAL_BACK
} MaterialBufferTypes_t;

typedef enum MaterialCullMode_t
{
	MATERIAL_CULLMODE_CCW,
	MATERIAL_CULLMODE_CW
} MaterialCullMode_t;

typedef enum MaterialIndexFormat_t
{
	MATERIAL_INDEX_FORMAT_UNKNOWN = -1,
	MATERIAL_INDEX_FORMAT_16BIT = 0,
	MATERIAL_INDEX_FORMAT_32BIT
} MaterialIndexFormat_t;

typedef enum MaterialFogMode_t
{
	MATERIAL_FOG_NONE,
	MATERIAL_FOG_LINEAR,
	MATERIAL_FOG_LINEAR_BELOW_FOG_Z
} MaterialFogMode_t;

typedef enum MaterialHeightClipMode_t
{
	MATERIAL_HEIGHTCLIPMODE_DISABLE,
	MATERIAL_HEIGHTCLIPMODE_RENDER_ABOVE_HEIGHT,
	MATERIAL_HEIGHTCLIPMODE_RENDER_BELOW_HEIGHT
} MaterialHeightClipMode_t;

typedef enum MaterialNonInteractiveMode_t
{
	MATERIAL_NON_INTERACTIVE_MODE_NONE = -1,
	MATERIAL_NON_INTERACTIVE_MODE_STARTUP = 0,
	MATERIAL_NON_INTERACTIVE_MODE_LEVEL_LOAD,
	MATERIAL_NON_INTERACTIVE_MODE_COUNT
} MaterialNonInteractiveMode_t;

#define MATERIAL_MORPH_DECAL ((IMorph*)1)

typedef enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
} MaterialThreadMode_t;

typedef enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
} MaterialContextType_t;

typedef enum MaterialFindContext_t
{
	MATERIAL_FINDCONTEXT_NONE,
	MATERIAL_FINDCONTEXT_ISONAMODEL
} MaterialFindContext_t;

#define CREATERENDERTARGETFLAGS_HDR 0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP 0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
#define CREATERENDERTARGETFLAGS_NOEDRAM 0x00000008
#define CREATERENDERTARGETFLAGS_TEMP 0x00000010

typedef enum StencilOperation_t
{
	STENCILOPERATION_KEEP = 1,
	STENCILOPERATION_ZERO = 2,
	STENCILOPERATION_REPLACE = 3,
	STENCILOPERATION_INCRSAT = 4,
	STENCILOPERATION_DECRSAT = 5,
	STENCILOPERATION_INVERT = 6,
	STENCILOPERATION_INCR = 7,
	STENCILOPERATION_DECR = 8,
	STENCILOPERATION_FORCE_DWORD = 0x7fffffff
} StencilOperation_t;

typedef enum StencilComparisonFunction_t
{
	STENCILCOMPARISONFUNCTION_NEVER = 1,
	STENCILCOMPARISONFUNCTION_LESS = 2,
	STENCILCOMPARISONFUNCTION_EQUAL = 3,
	STENCILCOMPARISONFUNCTION_LESSEQUAL = 4,
	STENCILCOMPARISONFUNCTION_GREATER = 5,
	STENCILCOMPARISONFUNCTION_NOTEQUAL = 6,
	STENCILCOMPARISONFUNCTION_GREATEREQUAL = 7,
	STENCILCOMPARISONFUNCTION_ALWAYS = 8,
	STENCILCOMPARISONFUNCTION_FORCE_DWORD = 0x7fffffff
} StencilComparisonFunction_t;

typedef enum MorphFormatFlags_t
{
	MORPH_POSITION = 0x0001,
	MORPH_NORMAL = 0x0002,
	MORPH_WRINKLE = 0x0004,
	MORPH_SPEED = 0x0008,
	MORPH_SIDE = 0x0010
} MorphFormatFlags_t;

typedef unsigned int MorphFormat_t;

typedef enum StandardLightmap_t
{
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE = -1,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_WHITE_BUMP = -2,
	MATERIAL_SYSTEM_LIGHTMAP_PAGE_USER_DEFINED = -3
} StandardLightmap_t;

typedef struct MaterialSystem_SortInfo_t
{
	IMaterial* material;
	int lightmapPageID;
} MaterialSystem_SortInfo_t;

#define MAX_FB_TEXTURES 4

enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

typedef struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
} MaterialAdapterInfo_t;

typedef struct MaterialVideoMode_t
{
	int m_Width;
	int m_Height;
	ImageFormat m_Format;
	int m_RefreshRate;
} MaterialVideoMode_t;

typedef struct FlashlightState_t
{
	Vector m_vecLightOrigin;
	Quaternion m_quatOrientation;
	float m_NearZ;
	float m_FarZ;
	float m_fHorizontalFOVDegrees;
	float m_fVerticalFOVDegrees;
	float m_fQuadraticAtten;
	float m_fLinearAtten;
	float m_fConstantAtten;
	float m_Color[4];
	ITexture* m_pSpotlightTexture;
	int m_nSpotlightTextureFrame;
	bool m_bEnableShadows;
	bool m_bDrawShadowFrustum;
	float m_flShadowMapResolution;
	float m_flShadowFilterSize;
	float m_flShadowSlopeScaleDepthBias;
	float m_flShadowDepthBias;
	float m_flShadowJitterSeed;
	float m_flShadowAtten;
	int m_nShadowQuality;
	bool m_bScissor;
	int m_nLeft;
	int m_nTop;
	int m_nRight;
	int m_nBottom;
} FlashlightState_t;

typedef struct IAsyncTextureOperationReceiver IAsyncTextureOperationReceiver;

typedef struct IAsyncTextureOperationReceiver_VMT
{
	IRefCounted_VMT vmt_IRefCounted;

	void (*OnAsyncCreateComplete)(IAsyncTextureOperationReceiver*, ITexture* pTex, void* pExtraArgs);
	void (*OnAsyncFindComplete)(IAsyncTextureOperationReceiver*, ITexture* pTex, void* pExtraArgs);
	void (*OnAsyncMapComplete)(IAsyncTextureOperationReceiver*, ITexture* pTex, void* pExtraArgs, void* pMemory, int nPitch);
	void (*OnAsyncReadbackBegin)(IAsyncTextureOperationReceiver*, ITexture* pDst, ITexture* pSrc, void* pExtraArgs);
	int (*GetRefCount)(IAsyncTextureOperationReceiver*);
} IAsyncTextureOperationReceiver_VMT;

struct IAsyncTextureOperationReceiver
{
	IAsyncTextureOperationReceiver_VMT* vmt;
};

typedef enum MaterialInitFlags_t
{
	MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE = 0x2,
	MATERIAL_INIT_REFERENCE_RASTERIZER = 0x4
} MaterialInitFlags_t;

typedef enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3
} MaterialRenderTargetDepth_t;

typedef enum RestoreChangeFlags_t
{
	MATERIAL_RESTORE_VERTEX_FORMAT_CHANGED = 0x1
} RestoreChangeFlags_t;

typedef enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,
	RT_SIZE_DEFAULT = 1,
	RT_SIZE_PICMIP = 2,
	RT_SIZE_HDR = 3,
	RT_SIZE_FULL_FRAME_BUFFER = 4,
	RT_SIZE_OFFSCREEN = 5,
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6,
	RT_SIZE_REPLAY_SCREENSHOT = 7,
	RT_SIZE_LITERAL = 8,
	RT_SIZE_LITERAL_PICMIP = 9
} RenderTargetSizeMode_t;

typedef enum RenderBackend_t
{
	RENDER_BACKEND_UNKNOWN,
	RENDER_BACKEND_D3D9,
	RENDER_BACKEND_TOGL,
	RENDER_BACKEND_VULKAN,
	RENDER_BACKEND_NULL,
} RenderBackend_t;

typedef void (*MaterialBufferReleaseFunc_t)();
typedef void (*MaterialBufferRestoreFunc_t)(int nChangeFlags);
typedef void (*ModeChangeCallbackFunc_t)(void);

typedef int VertexBufferHandle_t;
typedef unsigned short MaterialHandle_t;

typedef void* OcclusionQueryObjectHandle_t;
// #define INVALID_OCCLUSION_QUERY_OBJECT_HANDLE ( (OcclusionQueryObjectHandle_t)0 )

typedef struct IMaterialProxyFactory IMaterialProxyFactory;
// typedef struct ITexture ITexture;
// typedef struct IMaterialSystemHardwareConfig IMaterialSystemHardwareConfig;
typedef struct CShadowMgr CShadowMgr;

typedef void* MaterialLock_t;

typedef struct IMaterialSystem IMaterialSystem;
typedef struct IMatRenderContext IMatRenderContext;

typedef struct IMaterialSystem_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	CreateInterfaceFn (*Init)(IMaterialSystem*, char const* pShaderAPIDLL, IMaterialProxyFactory* pMaterialProxyFactory, CreateInterfaceFn fileSystemFactory, CreateInterfaceFn cvarFactory);
	void (*SetShaderAPI)(IMaterialSystem*, char const* pShaderAPIDLL);
	void (*SetAdapter)(IMaterialSystem*, int nAdapter, int nFlags);
	void (*ModInit)(IMaterialSystem*);
	void (*ModShutdown)(IMaterialSystem*);
	void (*SetThreadMode)(IMaterialSystem*, MaterialThreadMode_t mode, int nServiceThread);
	MaterialThreadMode_t (*GetThreadMode)(IMaterialSystem*);
	bool (*IsRenderThreadSafe)(IMaterialSystem*);
	void (*ExecuteQueued)(IMaterialSystem*);
	IMaterialSystemHardwareConfig* (*GetHardwareConfig)(IMaterialSystem*, const char* pVersion, int* returnCode);
	bool (*UpdateConfig)(IMaterialSystem*, bool bForceUpdate);
	bool (*OverrideConfig)(IMaterialSystem*, const MaterialSystem_Config_t* config, bool bForceUpdate);
	const MaterialSystem_Config_t* (*GetCurrentConfigForVideoCard)(IMaterialSystem*); // const
	bool (*GetRecommendedConfigurationInfo)(IMaterialSystem*, int nDXLevel, KeyValues* pKeyValues);
	int (*GetDisplayAdapterCount)(IMaterialSystem*); // const
	int (*GetCurrentAdapter)(IMaterialSystem*); // const
	void (*GetDisplayAdapterInfo)(IMaterialSystem*, int adapter, MaterialAdapterInfo_t* info); // const
	int (*GetModeCount)(IMaterialSystem*, int adapter); // const
	void (*GetModeInfo)(IMaterialSystem*, int adapter, int mode, MaterialVideoMode_t* info); // const
	void (*AddModeChangeCallBack)(IMaterialSystem*, ModeChangeCallbackFunc_t func);
	void (*GetDisplayMode)(IMaterialSystem*, MaterialVideoMode_t* mode); // const
	bool (*SetMode)(IMaterialSystem*, void* hwnd, const MaterialSystem_Config_t* config);
	bool (*SupportsMSAAMode)(IMaterialSystem*, int nMSAAMode);
	const MaterialSystemHardwareIdentifier_t* (*GetVideoCardIdentifier)(IMaterialSystem*); // const
	void (*SpewDriverInfo)(IMaterialSystem*); // const
	void (*GetDXLevelDefaults)(IMaterialSystem*, uint* max_dxlevel, uint* recommended_dxlevel);
	void (*GetBackBufferDimensions)(IMaterialSystem*, int* width, int* height); // const
	ImageFormat (*GetBackBufferFormat)(IMaterialSystem*); // const
	bool (*SupportsHDRMode)(IMaterialSystem*, HDRType_t nHDRModede);
	bool (*AddView)(IMaterialSystem*, void* hwnd);
	void (*RemoveView)(IMaterialSystem*, void* hwnd);
	void (*SetView)(IMaterialSystem*, void* hwnd);
	void (*BeginFrame)(IMaterialSystem*, float frameTime);
	void (*EndFrame)(IMaterialSystem*);
	void (*Flush)(IMaterialSystem*, bool flushHardware);
	void (*SwapBuffers)(IMaterialSystem*);
	void (*EvictManagedResources)(IMaterialSystem*);
	void (*ReleaseResources)(IMaterialSystem*);
	void (*ReacquireResources)(IMaterialSystem*);
	void (*AddReleaseFunc)(IMaterialSystem*, MaterialBufferReleaseFunc_t func);
	void (*RemoveReleaseFunc)(IMaterialSystem*, MaterialBufferReleaseFunc_t func);
	void (*AddRestoreFunc)(IMaterialSystem*, MaterialBufferRestoreFunc_t func);
	void (*RemoveRestoreFunc)(IMaterialSystem*, MaterialBufferRestoreFunc_t func);
	void (*ResetTempHWMemory)(IMaterialSystem*, bool bExitingLevel);
	void (*HandleDeviceLost)(IMaterialSystem*);
	int (*ShaderCount)(IMaterialSystem*); // const
	int (*GetShaders)(IMaterialSystem*, int nFirstShader, int nMaxCount, IShader** ppShaderList); // const
	int (*ShaderFlagCount)(IMaterialSystem*); // const
	const char* (*ShaderFlagName)(IMaterialSystem*, int nIndex); // const
	void (*GetShaderFallback)(IMaterialSystem*, const char* pShaderName, char* pFallbackShader, int nFallbackLength);
	IMaterialProxyFactory* (*GetMaterialProxyFactory)(IMaterialSystem*);
	void (*SetMaterialProxyFactory)(IMaterialSystem*, IMaterialProxyFactory* pFactory);
	void (*EnableEditorMaterials)(IMaterialSystem*);
	void (*SetInStubMode)(IMaterialSystem*, bool bInStubMode);
	void (*DebugPrintUsedMaterials)(IMaterialSystem*, const char* pSearchSubString, bool bVerbose);
	void (*DebugPrintUsedTextures)(IMaterialSystem*);
	void (*ToggleSuppressMaterial)(IMaterialSystem*, char const* pMaterialName);
	void (*ToggleDebugMaterial)(IMaterialSystem*, char const* pMaterialName);
	bool (*UsingFastClipping)(IMaterialSystem*);
	int (*StencilBufferBits)(IMaterialSystem*);
	void (*UncacheAllMaterials)(IMaterialSystem*);
	void (*UncacheUnusedMaterials)(IMaterialSystem*, bool bRecomputeStateSnapshots);
	void (*CacheUsedMaterials)(IMaterialSystem*);
	void (*ReloadTextures)(IMaterialSystem*);
	void (*ReloadMaterials)(IMaterialSystem*, const char* pSubString);
	IMaterial* (*CreateMaterial)(IMaterialSystem*, const char* pMaterialName, KeyValues* pVMTKeyValues);
	IMaterial* (*FindMaterial)(IMaterialSystem*, char const* pMaterialName, const char* pTextureGroupName, bool complain, const char* pComplainPrefix);
	bool (*IsMaterialLoaded)(IMaterialSystem*, char const* pMaterialName);
	MaterialHandle_t (*FirstMaterial)(IMaterialSystem*); // const
	MaterialHandle_t (*NextMaterial)(IMaterialSystem*, MaterialHandle_t h); // const
	MaterialHandle_t (*InvalidMaterial)(IMaterialSystem*); // const
	IMaterial* (*GetMaterial)(IMaterialSystem*, MaterialHandle_t h); // const
	int (*GetNumMaterials)(IMaterialSystem*); // const
	void (*SetAsyncTextureLoadCache)(IMaterialSystem*, void* hFileCache);
	ITexture* (*FindTexture)(IMaterialSystem*, char const* pTextureName, const char* pTextureGroupName, bool complain, int nAdditionalCreationFlags);
	bool (*IsTextureLoaded)(IMaterialSystem*, char const* pTextureName); // const
	ITexture* (*CreateProceduralTexture)(IMaterialSystem*, const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags);
	void (*BeginRenderTargetAllocation)(IMaterialSystem*);
	void (*EndRenderTargetAllocation)(IMaterialSystem*);
	ITexture* (*CreateRenderTargetTexture)(IMaterialSystem*, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth);
	ITexture* (*CreateNamedRenderTargetTextureEx)(IMaterialSystem*, const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags);
	ITexture* (*CreateNamedRenderTargetTexture)(IMaterialSystem*, const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, bool bClampTexCoords, bool bAutoMipMap);
	ITexture* (*CreateNamedRenderTargetTextureEx2)(IMaterialSystem*, const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags);
	void (*BeginLightmapAllocation)(IMaterialSystem*);
	void (*EndLightmapAllocation)(IMaterialSystem*);
	int (*AllocateLightmap)(IMaterialSystem*, int width, int height, int offsetIntoLightmapPage[2], IMaterial* pMaterial);
	int (*AllocateWhiteLightmap)(IMaterialSystem*, IMaterial* pMaterial);
	void (*UpdateLightmap)(IMaterialSystem*, int lightmapPageID, int lightmapSize[2], int offsetIntoLightmapPage[2], float* pFloatImage, float* pFloatImageBump1, float* pFloatImageBump2, float* pFloatImageBump3);
	int (*GetNumSortIDs)(IMaterialSystem*);
	void (*GetSortInfo)(IMaterialSystem*, MaterialSystem_SortInfo_t* sortInfoArray);
	void (*GetLightmapPageSize)(IMaterialSystem*, int lightmap, int* width, int* height); // const
	void (*ResetMaterialLightmapPageInfo)(IMaterialSystem*);
	void (*ClearBuffers)(IMaterialSystem*, bool bClearColor, bool bClearDepth, bool bClearStencil);
	IMatRenderContext* (*GetRenderContext)(IMaterialSystem*);
	bool (*SupportsShadowDepthTextures)(IMaterialSystem*);
	void (*BeginUpdateLightmaps)(IMaterialSystem*);
	void (*EndUpdateLightmaps)(IMaterialSystem*);
	MaterialLock_t (*Lock)(IMaterialSystem*);
	void (*Unlock)(IMaterialSystem*, MaterialLock_t);
	ImageFormat (*GetShadowDepthTextureFormat)(IMaterialSystem*);
	bool (*SupportsFetch4)(IMaterialSystem*);
	IMatRenderContext* (*CreateRenderContext)(IMaterialSystem*, MaterialContextType_t type);
	IMatRenderContext* (*SetRenderContext)(IMaterialSystem*, IMatRenderContext*);
	bool (*SupportsCSAAMode)(IMaterialSystem*, int nNumSamples, int nQualityLevel);
	void (*RemoveModeChangeCallBack)(IMaterialSystem*, ModeChangeCallbackFunc_t func);
	IMaterial* (*FindProceduralMaterial)(IMaterialSystem*, const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues);
	ImageFormat (*GetNullTextureFormat)(IMaterialSystem*);
	void (*AddTextureAlias)(IMaterialSystem*, const char* pAlias, const char* pRealName);
	void (*RemoveTextureAlias)(IMaterialSystem*, const char* pAlias);
	int (*AllocateDynamicLightmap)(IMaterialSystem*, int lightmapSize[2], int* pOutOffsetIntoPage, int frameID);
	void (*SetExcludedTextures)(IMaterialSystem*, const char* pScriptName);
	void (*UpdateExcludedTextures)(IMaterialSystem*);
	bool (*IsInFrame)(IMaterialSystem*); // const
	void (*CompactMemory)(IMaterialSystem*);
	void (*ReloadFilesInList)(IMaterialSystem*, IFileList* pFilesToReload);
	bool (*AllowThreading)(IMaterialSystem*, bool bAllow, int nServiceThread);
	IMaterial* (*FindMaterialEx)(IMaterialSystem*, char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain, const char* pComplainPrefix);
	void (*SetRenderTargetFrameBufferSizeOverrides)(IMaterialSystem*, int nWidth, int nHeight);
	void (*GetRenderTargetFrameBufferDimensions)(IMaterialSystem*, int* nWidth, int* nHeight);
	char* (*GetDisplayDeviceName)(IMaterialSystem*); // const
	ITexture* (*CreateTextureFromBits)(IMaterialSystem*, int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits);
	void (*OverrideRenderTargetAllocation)(IMaterialSystem*, bool rtAlloc);
	ITextureCompositor* (*NewTextureCompositor)(IMaterialSystem*, int w, int h, const char* pCompositeName, int nTeamNum, uint64 randomSeed, KeyValues* stageDesc, uint32 texCompositeCreateFlags);
	void (*AsyncFindTexture)(IMaterialSystem*, const char* pFilename, const char* pTextureGroupName, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs, bool bComplain, int nAdditionalCreationFlags);
	ITexture* (*CreateNamedTextureFromBitsEx)(IMaterialSystem*, const char* pName, const char* pTextureGroupName, int w, int h, int mips, ImageFormat fmt, int srcBufferSize, byte* srcBits, int nFlags);
	bool (*AddTextureCompositorTemplate)(IMaterialSystem*, const char* pName, KeyValues* pTmplDesc, int nTexCompositeTemplateFlags);
	bool (*VerifyTextureCompositorTemplates)(IMaterialSystem*);
	RenderBackend_t (*GetRenderBackend)(IMaterialSystem*); // const
	void (*SuspendTextureStreaming)(IMaterialSystem*);
	void (*ResumeTextureStreaming)(IMaterialSystem*);
} IMaterialSystem_VMT;

struct IMaterialSystem
{
	IMaterialSystem_VMT* vmt;
};

typedef struct IMatRenderContext_VMT
{
	IRefCounted_VMT vmt_IRefCounted;

	void (*BeginRender)(IMatRenderContext*);
	void (*EndRender)(IMatRenderContext*);
	void (*Flush)(IMatRenderContext*, bool flushHardware);
	void (*BindLocalCubemap)(IMatRenderContext*, ITexture* pTexture);
	void (*SetRenderTarget)(IMatRenderContext*, ITexture* pTexture);
	ITexture* (*GetRenderTarget)(IMatRenderContext*);
	void (*GetRenderTargetDimensions)(IMatRenderContext*, int* width, int* height); // const
	void (*Bind)(IMatRenderContext*, IMaterial* material, void* proxyData);
	void (*BindLightmapPage)(IMatRenderContext*, int lightmapPageID);
	void (*DepthRange)(IMatRenderContext*, float zNear, float zFar);
	void (*ClearBuffers)(IMatRenderContext*, bool bClearColor, bool bClearDepth, bool bClearStencil);
	void (*ReadPixels)(IMatRenderContext*, int x, int y, int width, int height, unsigned char* data, ImageFormat dstFormat);
	void (*SetAmbientLight)(IMatRenderContext*, float r, float g, float b);
	void (*SetLight)(IMatRenderContext*, int lightNum, const LightDesc_t* desc);
	void (*SetAmbientLightCube)(IMatRenderContext*, Vector4D cube[6]);
	void (*CopyRenderTargetToTexture)(IMatRenderContext*, ITexture* pTexture);
	void (*SetFrameBufferCopyTexture)(IMatRenderContext*, ITexture* pTexture, int textureIndex);
	ITexture* (*GetFrameBufferCopyTexture)(IMatRenderContext*, int textureIndex);
	void (*MatrixMode)(IMatRenderContext*, MaterialMatrixMode_t matrixMode);
	void (*PushMatrix)(IMatRenderContext*);
	void (*PopMatrix)(IMatRenderContext*);
	void (*LoadMatrix)(IMatRenderContext*, VMatrix const* matrix);
	void (*LoadMatrix3x4)(IMatRenderContext*, matrix3x4 const* matrix);
	void (*MultMatrix)(IMatRenderContext*, VMatrix const* matrix);
	void (*MultMatrix3x4)(IMatRenderContext*, matrix3x4 const* matrix);
	void (*MultMatrixLocal)(IMatRenderContext*, VMatrix const* matrix);
	void (*MultMatrixLocal3x4)(IMatRenderContext*, matrix3x4 const* matrix);
	void (*GetMatrix)(IMatRenderContext*, MaterialMatrixMode_t matrixMode, VMatrix* matrix);
	void (*GetMatrix3x4)(IMatRenderContext*, MaterialMatrixMode_t matrixMode, matrix3x4* matrix);
	void (*LoadIdentity)(IMatRenderContext*);
	void (*Ortho)(IMatRenderContext*, double left, double top, double right, double bottom, double zNear, double zFar);
	void (*PerspectiveX)(IMatRenderContext*, double fovx, double aspect, double zNear, double zFar);
	void (*PickMatrix)(IMatRenderContext*, int x, int y, int width, int height);
	void (*Rotate)(IMatRenderContext*, float angle, float x, float y, float z);
	void (*Translate)(IMatRenderContext*, float x, float y, float z);
	void (*Scale)(IMatRenderContext*, float x, float y, float z);
	void (*Viewport)(IMatRenderContext*, int x, int y, int width, int height);
	void (*GetViewport)(IMatRenderContext*, int* x, int* y, int* width, int* height); // const
	void (*CullMode)(IMatRenderContext*, MaterialCullMode_t cullMode);
	void (*SetHeightClipMode)(IMatRenderContext*, MaterialHeightClipMode_t nHeightClipMode);
	void (*SetHeightClipZ)(IMatRenderContext*, float z);
	void (*FogMode)(IMatRenderContext*, MaterialFogMode_t fogMode);
	void (*FogStart)(IMatRenderContext*, float fStart);
	void (*FogEnd)(IMatRenderContext*, float fEnd);
	void (*SetFogZ)(IMatRenderContext*, float fogZ);
	MaterialFogMode_t (*GetFogMode)(IMatRenderContext*);
	void (*FogColor3f)(IMatRenderContext*, float r, float g, float b);
	void (*FogColor3fv)(IMatRenderContext*, float const* rgb);
	void (*FogColor3ub)(IMatRenderContext*, unsigned char r, unsigned char g, unsigned char b);
	void (*FogColor3ubv)(IMatRenderContext*, unsigned char const* rgb);
	void (*GetFogColor)(IMatRenderContext*, unsigned char* rgb);
	void (*SetNumBoneWeights)(IMatRenderContext*, int numBones);
	IMesh* (*CreateStaticMesh)(IMatRenderContext*, VertexFormat_t fmt, const char* pTextureBudgetGroup, IMaterial* pMaterial);
	void (*DestroyStaticMesh)(IMatRenderContext*, IMesh* mesh);
	IMesh* (*GetDynamicMesh)(IMatRenderContext*, bool buffered, IMesh* pVertexOverride, IMesh* pIndexOverride, IMaterial* pAutoBind);
	IVertexBuffer* (*CreateStaticVertexBuffer)(IMatRenderContext*, VertexFormat_t fmt, int nVertexCount, const char* pTextureBudgetGroup);
	IIndexBuffer* (*CreateStaticIndexBuffer)(IMatRenderContext*, MaterialIndexFormat_t fmt, int nIndexCount, const char* pTextureBudgetGroup);
	void (*DestroyVertexBuffer)(IMatRenderContext*, IVertexBuffer*);
	void (*DestroyIndexBuffer)(IMatRenderContext*, IIndexBuffer*);
	IVertexBuffer* (*GetDynamicVertexBuffer)(IMatRenderContext*, int streamID, VertexFormat_t vertexFormat, bool bBuffered);
	IIndexBuffer* (*GetDynamicIndexBuffer)(IMatRenderContext*, MaterialIndexFormat_t fmt, bool bBuffered);
	void (*BindVertexBuffer)(IMatRenderContext*, int streamID, IVertexBuffer* pVertexBuffer, int nOffsetInBytes, int nFirstVertex, int nVertexCount, VertexFormat_t fmt, int nRepetitions);
	void (*BindIndexBuffer)(IMatRenderContext*, IIndexBuffer* pIndexBuffer, int nOffsetInBytes);
	void (*Draw)(IMatRenderContext*, MaterialPrimitiveType_t primitiveType, int firstIndex, int numIndices);
	int (*SelectionMode)(IMatRenderContext*, bool selectionMode);
	void (*SelectionBuffer)(IMatRenderContext*, unsigned int* pBuffer, int size);
	void (*ClearSelectionNames)(IMatRenderContext*);
	void (*LoadSelectionName)(IMatRenderContext*, int name);
	void (*PushSelectionName)(IMatRenderContext*, int name);
	void (*PopSelectionName)(IMatRenderContext*);
	void (*ClearColor3ub)(IMatRenderContext*, unsigned char r, unsigned char g, unsigned char b);
	void (*ClearColor4ub)(IMatRenderContext*, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void (*OverrideDepthEnable)(IMatRenderContext*, bool bEnable, bool bDepthEnable);
	void (*DrawScreenSpaceQuad)(IMatRenderContext*, IMaterial* pMaterial);
	void (*SyncToken)(IMatRenderContext*, const char* pToken);
	float (*ComputePixelWidthOfSphere)(IMatRenderContext*, const Vector* origin, float flRadius);
	OcclusionQueryObjectHandle_t (*CreateOcclusionQueryObject)(IMatRenderContext*);
	void (*DestroyOcclusionQueryObject)(IMatRenderContext*, OcclusionQueryObjectHandle_t);
	void (*BeginOcclusionQueryDrawing)(IMatRenderContext*, OcclusionQueryObjectHandle_t);
	void (*EndOcclusionQueryDrawing)(IMatRenderContext*, OcclusionQueryObjectHandle_t);
	int (*OcclusionQuery_GetNumPixelsRendered)(IMatRenderContext*, OcclusionQueryObjectHandle_t);
	void (*SetFlashlightMode)(IMatRenderContext*, bool bEnable);
	void (*SetFlashlightState)(IMatRenderContext*, const FlashlightState_t* state, const VMatrix* worldToTexture);
	MaterialHeightClipMode_t (*GetHeightClipMode)(IMatRenderContext*);
	float (*ComputePixelDiameterOfSphere)(IMatRenderContext*, const Vector* vecAbsOrigin, float flRadius);
	void (*EnableUserClipTransformOverride)(IMatRenderContext*, bool bEnable);
	void (*UserClipTransform)(IMatRenderContext*, const VMatrix* worldToView);
	bool (*GetFlashlightMode)(IMatRenderContext*); // const
	void (*ResetOcclusionQueryObject)(IMatRenderContext*, OcclusionQueryObjectHandle_t);
	void (*Unused3)(IMatRenderContext*);
	IMorph* (*CreateMorph)(IMatRenderContext*, MorphFormat_t format, const char* pDebugName);
	void (*DestroyMorph)(IMatRenderContext*, IMorph* pMorph);
	void (*BindMorph)(IMatRenderContext*, IMorph* pMorph);
	void (*SetFlexWeights)(IMatRenderContext*, int nFirstWeight, int nCount, const MorphWeight_t* pWeights);
	void (*Unused4)(IMatRenderContext*);
	void (*Unused5)(IMatRenderContext*);
	void (*Unused6)(IMatRenderContext*);
	void (*Unused7)(IMatRenderContext*);
	void (*Unused8)(IMatRenderContext*);
	void (*ReadPixelsAndStretch)(IMatRenderContext*, Rect_t* pSrcRect, Rect_t* pDstRect, unsigned char* pBuffer, ImageFormat dstFormat, int nDstStride);
	void (*GetWindowSize)(IMatRenderContext*, int* width, int* height); // const

	void (*DrawScreenSpaceRectangle)(
		IMatRenderContext*,
		IMaterial* pMaterial,
		int destx,
		int desty,
		int width,
		int height,
		float src_texture_x0,
		float src_texture_y0,
		float src_texture_x1,
		float src_texture_y1,
		int src_texture_width,
		int src_texture_height,
		void* pClientRenderable,
		int nXDice,
		int nYDice);

	void (*LoadBoneMatrix)(IMatRenderContext*, int boneIndex, const matrix3x4* matrix);
	void (*PushRenderTargetAndViewport)(IMatRenderContext*);
	void (*PushRenderTargetAndViewportTexture)(IMatRenderContext*, ITexture* pTexture);
	void (*PushRenderTargetAndViewportTextureEx)(IMatRenderContext*, ITexture* pTexture, int nViewX, int nViewY, int nViewW, int nViewH);
	void (*PushRenderTargetAndViewportTextureEx2)(IMatRenderContext*, ITexture* pTexture, ITexture* pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH);
	void (*PopRenderTargetAndViewport)(IMatRenderContext*);
	void (*BindLightmapTexture)(IMatRenderContext*, ITexture* pLightmapTexture);
	void (*CopyRenderTargetToTextureEx)(IMatRenderContext*, ITexture* pTexture, int nRenderTargetID, Rect_t* pSrcRect, Rect_t* pDstRect);
	void (*CopyTextureToRenderTargetEx)(IMatRenderContext*, int nRenderTargetID, ITexture* pTexture, Rect_t* pSrcRect, Rect_t* pDstRect);
	void (*PerspectiveOffCenterX)(IMatRenderContext*, double fovx, double aspect, double zNear, double zFar, double bottom, double top, double left, double right);
	void (*SetFloatRenderingParameter)(IMatRenderContext*, int parm_number, float value);
	void (*SetIntRenderingParameter)(IMatRenderContext*, int parm_number, int value);
	void (*SetVectorRenderingParameter)(IMatRenderContext*, int parm_number, Vector const* value);
	void (*SetStencilEnable)(IMatRenderContext*, bool onoff);
	void (*SetStencilFailOperation)(IMatRenderContext*, StencilOperation_t op);
	void (*SetStencilZFailOperation)(IMatRenderContext*, StencilOperation_t op);
	void (*SetStencilPassOperation)(IMatRenderContext*, StencilOperation_t op);
	void (*SetStencilCompareFunction)(IMatRenderContext*, StencilComparisonFunction_t cmpfn);
	void (*SetStencilReferenceValue)(IMatRenderContext*, int ref);
	void (*SetStencilTestMask)(IMatRenderContext*, uint32 msk);
	void (*SetStencilWriteMask)(IMatRenderContext*, uint32 msk);
	void (*ClearStencilBufferRectangle)(IMatRenderContext*, int xmin, int ymin, int xmax, int ymax, int value);
	void (*SetRenderTargetEx)(IMatRenderContext*, int nRenderTargetID, ITexture* pTexture);
	void (*PushCustomClipPlane)(IMatRenderContext*, const float* pPlane);
	void (*PopCustomClipPlane)(IMatRenderContext*);
	void (*GetMaxToRender)(IMatRenderContext*, IMesh* pMesh, bool bMaxUntilFlush, int* pMaxVerts, int* pMaxIndices);
	int (*GetMaxVerticesToRender)(IMatRenderContext*, IMaterial* pMaterial);
	int (*GetMaxIndicesToRender)(IMatRenderContext*);
	void (*DisableAllLocalLights)(IMatRenderContext*);
	int (*CompareMaterialCombos)(IMatRenderContext*, IMaterial* pMaterial1, IMaterial* pMaterial2, int lightMapID1, int lightMapID2);
	IMesh* (*GetFlexMesh)(IMatRenderContext*);
	void (*SetFlashlightStateEx)(IMatRenderContext*, const FlashlightState_t* state, const VMatrix* worldToTexture, ITexture* pFlashlightDepthTexture);
	ITexture* (*GetLocalCubemap)(IMatRenderContext*);
	void (*ClearBuffersObeyStencil)(IMatRenderContext*, bool bClearColor, bool bClearDepth);
	bool (*EnableClipping)(IMatRenderContext*, bool bEnable);
	void (*GetFogDistances)(IMatRenderContext*, float* fStart, float* fEnd, float* fFogZ);
	void (*BeginPIXEvent)(IMatRenderContext*, unsigned long color, const char* szName);
	void (*EndPIXEvent)(IMatRenderContext*);
	void (*SetPIXMarker)(IMatRenderContext*, unsigned long color, const char* szName);
	void (*BeginBatch)(IMatRenderContext*, IMesh* pIndices);
	void (*BindBatch)(IMatRenderContext*, IMesh* pVertices, IMaterial* pAutoBind);
	void (*DrawBatch)(IMatRenderContext*, int firstIndex, int numIndices);
	void (*EndBatch)(IMatRenderContext*);
	ICallQueue* (*GetCallQueue)(IMatRenderContext*);
	void (*GetWorldSpaceCameraPosition)(IMatRenderContext*, Vector* pCameraPos);
	void (*GetWorldSpaceCameraVectors)(IMatRenderContext*, Vector* pVecForward, Vector* pVecRight, Vector* pVecUp);
	void (*ResetToneMappingScale)(IMatRenderContext*, float monoscale);
	void (*SetGoalToneMappingScale)(IMatRenderContext*, float monoscale);
	void (*TurnOnToneMapping)(IMatRenderContext*);
	void (*SetToneMappingScaleLinear)(IMatRenderContext*, const Vector* scale);
	Vector (*GetToneMappingScaleLinear)(IMatRenderContext*);
	void (*SetShadowDepthBiasFactors)(IMatRenderContext*, float fSlopeScaleDepthBias, float fDepthBias);
	void (*PerformFullScreenStencilOperation)(IMatRenderContext*);
	void (*SetLightingOrigin)(IMatRenderContext*, Vector vLightingOrigin);
	void (*SetScissorRect)(IMatRenderContext*, const int nLeft, const int nTop, const int nRight, const int nBottom, const bool bEnableScissor);
	void (*BeginMorphAccumulation)(IMatRenderContext*);
	void (*EndMorphAccumulation)(IMatRenderContext*);
	void (*AccumulateMorph)(IMatRenderContext*, IMorph* pMorph, int nMorphCount, const MorphWeight_t* pWeights);
	void (*PushDeformation)(IMatRenderContext*, DeformationBase_t const* Deformation);
	void (*PopDeformation)(IMatRenderContext*);
	int (*GetNumActiveDeformations)(IMatRenderContext*); // const
	bool (*GetMorphAccumulatorTexCoord)(IMatRenderContext*, Vector2D* pTexCoord, IMorph* pMorph, int nVertex);
	IMesh* (*GetDynamicMeshEx)(IMatRenderContext*, VertexFormat_t vertexFormat, bool bBuffered, IMesh* pVertexOverride, IMesh* pIndexOverride, IMaterial* pAutoBind);
	void (*FogMaxDensity)(IMatRenderContext*, float flMaxDensity);
	IMaterial* (*GetCurrentMaterial)(IMatRenderContext*);
	int (*GetCurrentNumBones)(IMatRenderContext*); // const
	void* (*GetCurrentProxy)(IMatRenderContext*);
	void (*EnableColorCorrection)(IMatRenderContext*, bool bEnable);
	ColorCorrectionHandle_t (*AddLookup)(IMatRenderContext*, const char* pName);
	bool (*RemoveLookup)(IMatRenderContext*, ColorCorrectionHandle_t handle);
	void (*LockLookup)(IMatRenderContext*, ColorCorrectionHandle_t handle);
	void (*LoadLookup)(IMatRenderContext*, ColorCorrectionHandle_t handle, const char* pLookupName);
	void (*UnlockLookup)(IMatRenderContext*, ColorCorrectionHandle_t handle);
	void (*SetLookupWeight)(IMatRenderContext*, ColorCorrectionHandle_t handle, float flWeight);
	void (*ResetLookupWeights)(IMatRenderContext*);
	void (*SetResetable)(IMatRenderContext*, ColorCorrectionHandle_t handle, bool bResetable);
	void (*SetFullScreenDepthTextureValidityFlag)(IMatRenderContext*, bool bIsValid);
	void (*SetNonInteractivePacifierTexture)(IMatRenderContext*, ITexture* pTexture, float flNormalizedX, float flNormalizedY, float flNormalizedSize);
	void (*SetNonInteractiveTempFullscreenBuffer)(IMatRenderContext*, ITexture* pTexture, MaterialNonInteractiveMode_t mode);
	void (*EnableNonInteractiveMode)(IMatRenderContext*, MaterialNonInteractiveMode_t mode);
	void (*RefreshFrontBufferNonInteractive)(IMatRenderContext*);
	void* (*LockRenderData)(IMatRenderContext*, int nSizeInBytes);
	void (*UnlockRenderData)(IMatRenderContext*, void* pData);
	void (*AddRefRenderData)(IMatRenderContext*);
	void (*ReleaseRenderData)(IMatRenderContext*);
	bool (*IsRenderData)(IMatRenderContext*, const void* pData); // const
	void (*PrintfVA)(IMatRenderContext*, char* fmt, void* vargs); // va_list
	void (*Printf)(IMatRenderContext*, const char* fmt, ...);
	float (*Knob)(IMatRenderContext*, char* knobname, float* setvalue);
	void (*OverrideAlphaWriteEnable)(IMatRenderContext*, bool bEnable, bool bAlphaWriteEnable);
	void (*OverrideColorWriteEnable)(IMatRenderContext*, bool bOverrideEnable, bool bColorWriteEnable);
	void (*ClearBuffersObeyStencilEx)(IMatRenderContext*, bool bClearColor, bool bClearAlpha, bool bClearDepth);
	void (*AsyncCreateTextureFromRenderTarget)(IMatRenderContext*, ITexture* pSrcRt, const char* pDstName, ImageFormat dstFmt, bool bGenMips, int nAdditionalCreationFlags, IAsyncTextureOperationReceiver* pRecipient, void* pExtraArgs);
} IMatRenderContext_VMT;

struct IMatRenderContext
{
	IMatRenderContext_VMT* vmt;
};

typedef struct ShaderStencilState_t
{
	bool m_bEnable;
	StencilOperation_t m_FailOp;
	StencilOperation_t m_ZFailOp;
	StencilOperation_t m_PassOp;
	StencilComparisonFunction_t m_CompareFunc;
	int m_nReferenceValue;
	uint32 m_nTestMask;
	uint32 m_nWriteMask;
} ShaderStencilState_t;

#define IMaterialSystem_REFL "IMaterialSystem"
#define IMaterialSystem_SIGNATURE "VMaterialSystem082"
#define IMaterialSystem_MODULE "materialsystem"

#endif
