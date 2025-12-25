#if !defined(MainIClientEntity_Included)
#define MainIClientEntity_Included

#include "Main/IClientNetworkable.h"
#include "Main/IClientRenderable.h"
#include "Main/IClientThinkable.h"

typedef struct CMouthInfo CMouthInfo;
typedef struct SpatializationInfo_t SpatializationInfo_t;

typedef struct IClientNetworkable_VMT_IClientRenderable
{
	void (*Release)(IClientNetworkable*);
	ClientClass* (*GetClientClass)(IClientNetworkable*);
	void (*NotifyShouldTransmit)(IClientNetworkable*, ShouldTransmitState_t state);
	void (*OnPreDataChanged)(IClientNetworkable*, DataUpdateType_t updateType);
	void (*OnDataChanged)(IClientNetworkable*, DataUpdateType_t updateType);
	void (*PreDataUpdate)(IClientNetworkable*, DataUpdateType_t updateType);
	void (*PostDataUpdate)(IClientNetworkable*, DataUpdateType_t updateType);
	bool (*IsDormant)(IClientNetworkable*);
	int (*entindex)(IClientNetworkable*); // const
	void (*ReceiveMessage)(IClientNetworkable*, int classID, bf_read* msg);
	void* (*GetDataTableBasePtr)(IClientNetworkable*);
	void (*SetDestroyedOnRecreateEntities)(IClientNetworkable*);
	void (*OnDataUnchangedInPVS)(IClientNetworkable*);
} IClientNetworkable_VMT_IClientRenderable;

typedef struct IClientThinkable_VMT_IClientNetworkable
{
	void (*ClientThink)(IClientThinkable*);
	ClientThinkHandle_t (*GetThinkHandle)(IClientThinkable*);
	void (*SetThinkHandle)(IClientThinkable*, ClientThinkHandle_t hThink);
} IClientThinkable_VMT_IClientNetworkable;

typedef struct IClientEntity IClientEntity;

// Incorrect
typedef struct IClientEntity_VMT
{
	IClientUnknown_VMT vmt_IClientUnknown;
	IClientRenderable_VMT vmt_IClientRenderable;
	IClientNetworkable_VMT_IClientRenderable vmt_IClientNetworkable;
	IClientThinkable_VMT_IClientNetworkable vmt_IClientThinkable;

	// void (*Release)(IClientEntity*);
	const Vector* (*GetAbsOrigin)(IClientEntity*); // const
	const QAngle* (*GetAbsAngles)(IClientEntity*); // const
	CMouthInfo* (*GetMouth)(IClientEntity*);
	bool (*GetSoundSpatialization)(IClientEntity*, SpatializationInfo_t* info);
} IClientEntity_VMT;

struct IClientEntity
{
	IClientEntity_VMT* vmt;
};

#endif
