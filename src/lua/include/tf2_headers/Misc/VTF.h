#if !defined(MiscVTF_Included)
#define MiscVTF_Included

#include "Types.h"
#include "Misc/ImageFormat.h"
#include "Misc/CommonMacros.h"

typedef struct CUtlBuffer CUtlBuffer;
typedef struct IFileSystem IFileSystem;

typedef struct IVTFTexture IVTFTexture;

typedef enum CompiledVtfFlags
{
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,
	TEXTUREFLAGS_STAGING_MEMORY = 0x00080000,
	TEXTUREFLAGS_IMMEDIATE_CLEANUP = 0x00100000,
	TEXTUREFLAGS_IGNORE_PICMIP = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,
	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,
	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,
	TEXTUREFLAGS_CLAMPU = 0x02000000,
	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,
	TEXTUREFLAGS_SSBUMP = 0x08000000,
	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,
	TEXTUREFLAGS_BORDER = 0x20000000,
	TEXTUREFLAGS_STREAMABLE_COARSE = 0x40000000,
	TEXTUREFLAGS_STREAMABLE_FINE = 0x80000000,
	TEXTUREFLAGS_STREAMABLE = (TEXTUREFLAGS_STREAMABLE_COARSE | TEXTUREFLAGS_STREAMABLE_FINE)
} CompiledVtfFlags;

typedef enum VersionedVtfFlags
{
	VERSIONED_VTF_FLAGS_MASK_7_3 = ~0xD1780400
} VersionedVtfFlags;

typedef enum VtfProcessingOptions_Flags0
{
	OPT_DECAY_R = 0x00000001,
	OPT_DECAY_G = 0x00000002,
	OPT_DECAY_B = 0x00000004,
	OPT_DECAY_A = 0x00000008,
	OPT_DECAY_EXP_R = 0x00000010,
	OPT_DECAY_EXP_G = 0x00000020,
	OPT_DECAY_EXP_B = 0x00000040,
	OPT_DECAY_EXP_A = 0x00000080,
	OPT_NOCOMPRESS = 0x00000100,
	OPT_NORMAL_DUDV = 0x00000200,
	OPT_FILTER_NICE = 0x00000400,
	OPT_SET_ALPHA_ONEOVERMIP = 0x00001000,
	OPT_PREMULT_COLOR_ONEOVERMIP = 0x00002000,
	OPT_MIP_ALPHATEST = 0x00004000
} VtfProcessingOptions_Flags0;

typedef struct VtfProcessingOptions
{
	uint32 cbSize;
	uint32 flags0;
	uint8 clrDecayGoal[4];
	uint8 numNotDecayMips[4];
	float fDecayExponentBase[4];
} VtfProcessingOptions;

typedef enum CubeMapFaceIndex_t
{
	CUBEMAP_FACE_RIGHT = 0,
	CUBEMAP_FACE_LEFT,
	CUBEMAP_FACE_BACK,
	CUBEMAP_FACE_FRONT,
	CUBEMAP_FACE_UP,
	CUBEMAP_FACE_DOWN,
	CUBEMAP_FACE_SPHEREMAP,
	CUBEMAP_FACE_COUNT
} CubeMapFaceIndex_t;

typedef enum LookDir_t
{
	LOOK_DOWN_X = 0,
	LOOK_DOWN_NEGX,
	LOOK_DOWN_Y,
	LOOK_DOWN_NEGY,
	LOOK_DOWN_Z,
	LOOK_DOWN_NEGZ
} LookDir_t;

// #define STREAMING_START_MIPMAP 3
// #define IMAGE_FORMAT_DEFAULT ((ImageFormat) - 2)

typedef struct IVTFTexture_VMT
{
	//virtual ~IVTFTexture() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	bool (*Init)(IVTFTexture*, int nWidth, int nHeight, int nDepth, ImageFormat fmt, int nFlags, int iFrameCount, int nForceMipCount);
	void (*SetBumpScale)(IVTFTexture*, float flScale);
	void (*SetReflectivity)(IVTFTexture*, const Vector* vecReflectivity);
	void (*InitLowResImage)(IVTFTexture*, int nWidth, int nHeight, ImageFormat fmt);
	void* (*SetResourceData)(IVTFTexture*, uint32 eType, void const* pData, size_t nDataSize);
	void* (*GetResourceData)(IVTFTexture*, uint32 eType, size_t* pDataSize); // const
	bool (*HasResourceEntry)(IVTFTexture*, uint32 eType); // const
	unsigned int (*GetResourceTypes)(IVTFTexture*, uint32* arrTypesBuffer, int numTypesBufferElems); // const
	bool (*Unserialize)(IVTFTexture*, CUtlBuffer* buf, bool bHeaderOnly, int nSkipMipLevels);
	bool (*Serialize)(IVTFTexture*, CUtlBuffer* buf);
	void (*LowResFileInfo)(IVTFTexture*, int* pStartLocation, int* pSizeInBytes); // const
	void (*ImageFileInfo)(IVTFTexture*, int nFrame, int nFace, int nMip, int* pStartLocation, int* pSizeInBytes); // const
	int (*FileSize)(IVTFTexture*, int nMipSkipCount); // const
	int (*Width)(IVTFTexture*); // const
	int (*Height)(IVTFTexture*); // const
	int (*Depth)(IVTFTexture*); // const
	int (*MipCount)(IVTFTexture*); // const
	int (*RowSizeInBytes)(IVTFTexture*, int nMipLevel); // const
	int (*FaceSizeInBytes)(IVTFTexture*, int nMipLevel); // const
	ImageFormat (*Format)(IVTFTexture*); // const
	int (*FaceCount)(IVTFTexture*); // const
	int (*FrameCount)(IVTFTexture*); // const
	int (*Flags)(IVTFTexture*); // const
	float (*BumpScale)(IVTFTexture*); // const
	int (*LowResWidth)(IVTFTexture*); // const
	int (*LowResHeight)(IVTFTexture*); // const
	ImageFormat (*LowResFormat)(IVTFTexture*); // const
	const Vector* (*Reflectivity)(IVTFTexture*); // const
	bool (*IsCubeMap)(IVTFTexture*); // const
	bool (*IsNormalMap)(IVTFTexture*); // const
	bool (*IsVolumeTexture)(IVTFTexture*); // const
	void (*ComputeMipLevelDimensions)(IVTFTexture*, int iMipLevel, int* pMipWidth, int* pMipHeight, int* pMipDepth); // const
	int (*ComputeMipSize)(IVTFTexture*, int iMipLevel); // const
	void (*ComputeMipLevelSubRect)(IVTFTexture*, Rect_t* pSrcRect, int nMipLevel, Rect_t* pSubRect); // const
	int (*ComputeFaceSize)(IVTFTexture*, int iStartingMipLevel); // const
	int (*ComputeTotalSize)(IVTFTexture*); // const
	unsigned char* (*ImageData)(IVTFTexture*);
	unsigned char* (*ImageDataEx)(IVTFTexture*, int iFrame, int iFace, int iMipLevel);
	unsigned char* (*ImageDataEx2)(IVTFTexture*, int iFrame, int iFace, int iMipLevel, int x, int y, int z);
	unsigned char* (*LowResImageData)(IVTFTexture*);
	void (*ConvertImageFormat)(IVTFTexture*, ImageFormat fmt, bool bNormalToDUDV);
	void (*GenerateSpheremap)(IVTFTexture*, LookDir_t lookDir);
	void (*GenerateHemisphereMap)(IVTFTexture*, unsigned char* pSphereMapBitsRGBA, int targetWidth, int targetHeight, LookDir_t lookDir, int iFrame);
	void (*FixCubemapFaceOrientation)(IVTFTexture*);
	void (*GenerateMipmaps)(IVTFTexture*);
	void (*PutOneOverMipLevelInAlpha)(IVTFTexture*);
	void (*ComputeReflectivity)(IVTFTexture*);
	void (*ComputeAlphaFlags)(IVTFTexture*);
	bool (*ConstructLowResImage)(IVTFTexture*);
	void (*PostProcess)(IVTFTexture*, bool bGenerateSpheremap, LookDir_t lookDir, bool bAllowFixCubemapOrientation);
	void (*MatchCubeMapBorders)(IVTFTexture*, int iStage, ImageFormat finalFormat, bool bSkybox);
	void (*SetAlphaTestThreshholds)(IVTFTexture*, float flBase, float flHighFreq);
	void (*SetPostProcessingSettings)(IVTFTexture*, VtfProcessingOptions const* pOptions);
	bool (*UnserializeEx)(IVTFTexture*, CUtlBuffer* buf, bool bHeaderOnly, int nForceFlags, int nSkipMipLevels);
	void (*GetMipmapRange)(IVTFTexture*, int* pOutFinest, int* pOutCoarsest);
} IVTFTexture_VMT;

struct IVTFTexture
{
	IVTFTexture_VMT* vmt;
};

#endif