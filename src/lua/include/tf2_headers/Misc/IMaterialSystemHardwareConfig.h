#if !defined(MiscIMaterialSystemHardwareConfig_Included)
#define MiscIMaterialSystemHardwareConfig_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

typedef enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT
} HDRType_t;

typedef enum VertexCompressionType_t
{
	VERTEX_COMPRESSION_INVALID = 0xFFFFFFFF,
	VERTEX_COMPRESSION_NONE = 0,
	VERTEX_COMPRESSION_ON = 1
} VertexCompressionType_t;

typedef struct IMaterialSystemHardwareConfig IMaterialSystemHardwareConfig;

#define DEFCONFIGMETHOD(ret_type, method, xbox_return_value) ret_type (*method)(IMaterialSystemHardwareConfig*)

typedef struct IMaterialSystemHardwareConfig_VMT
{
	DEFCONFIGMETHOD(bool, HasDestAlphaBuffer, true);
	DEFCONFIGMETHOD(bool, HasStencilBuffer, true);
	int (*GetFrameBufferColorDepth)(IMaterialSystemHardwareConfig*); // const
	int (*GetSamplerCount)(IMaterialSystemHardwareConfig*); // const
	bool (*HasSetDeviceGammaRamp)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, SupportsCompressedTextures, true);
	VertexCompressionType_t (*SupportsCompressedVertices)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsNormalMapCompression)(IMaterialSystemHardwareConfig*); // const { return false; }
	DEFCONFIGMETHOD(bool, SupportsVertexAndPixelShaders, true);
	DEFCONFIGMETHOD(bool, SupportsPixelShaders_1_4, true);
	DEFCONFIGMETHOD(bool, SupportsStaticControlFlow, true);
	DEFCONFIGMETHOD(bool, SupportsPixelShaders_2_0, true);
	DEFCONFIGMETHOD(bool, SupportsVertexShaders_2_0, true);
	int (*MaximumAnisotropicLevel)(IMaterialSystemHardwareConfig*); // const
	int (*MaxTextureWidth)(IMaterialSystemHardwareConfig*); // const
	int (*MaxTextureHeight)(IMaterialSystemHardwareConfig*); // const
	int (*TextureMemorySize)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsOverbright)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsCubeMaps)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsMipmappedCubemaps)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsNonPow2Textures)(IMaterialSystemHardwareConfig*); // const
	int (*GetTextureStageCount)(IMaterialSystemHardwareConfig*); // const
	int (*NumVertexShaderConstants)(IMaterialSystemHardwareConfig*); // const
	int (*NumPixelShaderConstants)(IMaterialSystemHardwareConfig*); // const
	int (*MaxNumLights)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsHardwareLighting)(IMaterialSystemHardwareConfig*); // const
	int (*MaxBlendMatrices)(IMaterialSystemHardwareConfig*); // const
	int (*MaxBlendMatrixIndices)(IMaterialSystemHardwareConfig*); // const
	int (*MaxTextureAspectRatio)(IMaterialSystemHardwareConfig*); // const
	int (*MaxVertexShaderBlendMatrices)(IMaterialSystemHardwareConfig*); // const
	int (*MaxUserClipPlanes)(IMaterialSystemHardwareConfig*); // const
	bool (*UseFastClipping)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(int, GetDXSupportLevel, 98);
	const char* (*GetShaderDLLName)(IMaterialSystemHardwareConfig*); // const
	bool (*ReadPixelsFromFrontBuffer)(IMaterialSystemHardwareConfig*); // const
	bool (*PreferDynamicTextures)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, SupportsHDR, true);
	bool (*HasProjectedBumpEnv)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsSpheremapping)(IMaterialSystemHardwareConfig*); // const
	bool (*NeedsAAClamp)(IMaterialSystemHardwareConfig*); // const
	bool (*NeedsATICentroidHack)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsColorOnSecondStream)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsStaticPlusDynamicLighting)(IMaterialSystemHardwareConfig*); // const
	bool (*PreferReducedFillrate)(IMaterialSystemHardwareConfig*); // const
	int (*GetMaxDXSupportLevel)(IMaterialSystemHardwareConfig*); // const
	bool (*SpecifiesFogColorInLinearSpace)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, SupportsSRGB, true);
	DEFCONFIGMETHOD(bool, FakeSRGBWrite, false);
	DEFCONFIGMETHOD(bool, CanDoSRGBReadFromRTs, true);
	bool (*SupportsGLMixedSizeTargets)(IMaterialSystemHardwareConfig*); // const
	bool (*IsAAEnabled)(IMaterialSystemHardwareConfig*); // const
	int (*GetVertexTextureCount)(IMaterialSystemHardwareConfig*); // const
	int (*GetMaxVertexTextureDimension)(IMaterialSystemHardwareConfig*); // const
	int (*MaxTextureDepth)(IMaterialSystemHardwareConfig*); // const
	HDRType_t (*GetHDRType)(IMaterialSystemHardwareConfig*); // const
	HDRType_t (*GetHardwareHDRType)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, SupportsPixelShaders_2_b, true);
	bool (*SupportsStreamOffset)(IMaterialSystemHardwareConfig*); // const
	int (*StencilBufferBits)(IMaterialSystemHardwareConfig*); // const
	int (*MaxViewports)(IMaterialSystemHardwareConfig*); // const
	void (*OverrideStreamOffsetSupport)(IMaterialSystemHardwareConfig*, bool bOverrideEnabled, bool bEnableSupport); // const
	int (*GetShadowFilterMode)(IMaterialSystemHardwareConfig*); // const
	int (*NeedsShaderSRGBConversion)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, UsesSRGBCorrectBlending, true);
	bool (*SupportsShaderModel_3_0)(IMaterialSystemHardwareConfig*); // const
	bool (*HasFastVertexTextures)(IMaterialSystemHardwareConfig*); // const
	int (*MaxHWMorphBatchCount)(IMaterialSystemHardwareConfig*); // const
	DEFCONFIGMETHOD(bool, ActuallySupportsPixelShaders_2_b, true);
	bool (*SupportsHDRMode)(IMaterialSystemHardwareConfig*, HDRType_t nHDRMode); // const
	bool (*GetHDREnabled)(IMaterialSystemHardwareConfig*); // const
	void (*SetHDREnabled)(IMaterialSystemHardwareConfig*, bool bEnable); // const
	bool (*SupportsBorderColor)(IMaterialSystemHardwareConfig*); // const
	bool (*SupportsFetch4)(IMaterialSystemHardwareConfig*); // const
	bool (*CanStretchRectFromTextures)(IMaterialSystemHardwareConfig*); // const
} IMaterialSystemHardwareConfig_VMT;

struct IMaterialSystemHardwareConfig
{
	IMaterialSystemHardwareConfig_VMT* vmt;
};

#endif