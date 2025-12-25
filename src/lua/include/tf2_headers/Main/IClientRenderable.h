#if !defined(MainIClientRenderable_Included)
#define MainIClientRenderable_Included

#include "Main/IClientUnknown.h"
#include "Main/CModel.h"

typedef unsigned short ClientShadowHandle_t;

enum
{
	CLIENTSHADOW_INVALID_HANDLE = (ClientShadowHandle_t)~0
};

typedef enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,
	SHADOWS_RENDER_TO_DEPTH_TEXTURE
} ShadowType_t;

typedef struct IPVSNotify IPVSNotify;

typedef struct IPVSNotify_VMT
{
	void (*OnPVSStatusChanged)(IPVSNotify*, bool bInPVS);
} IPVSNotify_VMT;

struct IPVSNotify
{
	IPVSNotify_VMT* vmt;
};

typedef void* ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef struct model_t model_t;

typedef struct IClientRenderable IClientRenderable;

typedef struct IClientRenderable_VMT
{
	IClientUnknown* (*GetIClientUnknown)(IClientRenderable*);
	Vector const* (*GetRenderOrigin)(IClientRenderable*);
	QAngle const* (*GetRenderAngles)(IClientRenderable*);
	bool (*ShouldDraw)(IClientRenderable*);
	bool (*IsTransparent)(IClientRenderable*);
	bool (*UsesPowerOfTwoFrameBufferTexture)(IClientRenderable*);
	bool (*UsesFullFrameBufferTexture)(IClientRenderable*);
	ClientShadowHandle_t (*GetShadowHandle)(IClientRenderable*); // const
	ClientRenderHandle_t* (*RenderHandle)(IClientRenderable*);
	const model_t* (*GetModel)(IClientRenderable*); // const
	int (*DrawModel)(IClientRenderable*, int flags);
	int (*GetBody)(IClientRenderable*);
	void (*ComputeFxBlend)(IClientRenderable*);
	int (*GetFxBlend)(IClientRenderable*);
	void (*GetColorModulation)(IClientRenderable*, float* color);
	bool (*LODTest)(IClientRenderable*);
	bool (*SetupBones)(IClientRenderable*, matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	void (*SetupWeights)(IClientRenderable*, const matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);
	void (*DoAnimationEvents)(IClientRenderable*);
	IPVSNotify* (*GetPVSNotifyInterface)(IClientRenderable*);
	void (*GetRenderBounds)(IClientRenderable*, Vector* mins, Vector* maxs);
	void (*GetRenderBoundsWorldspace)(IClientRenderable*, Vector* mins, Vector* maxs);
	void (*GetShadowRenderBounds)(IClientRenderable*, Vector* mins, Vector* maxs, ShadowType_t shadowType);
	bool (*ShouldReceiveProjectedTextures)(IClientRenderable*, int flags);
	bool (*GetShadowCastDistance)(IClientRenderable*, float* pDist, ShadowType_t shadowType); // const
	bool (*GetShadowCastDirection)(IClientRenderable*, Vector* pDirection, ShadowType_t shadowType); // const
	bool (*IsShadowDirty)(IClientRenderable*);
	void (*MarkShadowDirty)(IClientRenderable*, bool bDirty);
	IClientRenderable* (*GetShadowParent)(IClientRenderable*);
	IClientRenderable* (*FirstShadowChild)(IClientRenderable*);
	IClientRenderable* (*NextShadowPeer)(IClientRenderable*);
	ShadowType_t (*ShadowCastType)(IClientRenderable*);
	void (*CreateModelInstance)(IClientRenderable*);
	ModelInstanceHandle_t (*GetModelInstance)(IClientRenderable*);
	const matrix3x4* (*RenderableToWorldTransform)(IClientRenderable*);
	int (*LookupAttachment)(IClientRenderable*, const char* pAttachmentName);
	bool (*GetAttachment)(IClientRenderable*, int number, Vector* origin, QAngle* angles);
	bool (*GetAttachmentMatrix)(IClientRenderable*, int number, matrix3x4* matrix);
	float* (*GetRenderClipPlane)(IClientRenderable*);
	int (*GetSkin)(IClientRenderable*);
	bool (*IsTwoPass)(IClientRenderable*);
	void (*OnThreadedDrawSetup)(IClientRenderable*);
	bool (*UsesFlexDelayedWeights)(IClientRenderable*);
	void (*RecordToolMessage)(IClientRenderable*);
	bool (*IgnoresZBuffer)(IClientRenderable*); // const
} IClientRenderable_VMT;

struct IClientRenderable
{
	IClientRenderable_VMT* vmt;
};

#endif
