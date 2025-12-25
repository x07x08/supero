#if !defined(MainIClientNetworkable_Included)
#define MainIClientNetworkable_Included

#include "Main/IClientUnknown.h"
#include "Misc/BitBuf.h"
#include "Misc/ClientClass.h"
#include "Definitions.h"

typedef struct IClientEntity IClientEntity;

typedef enum ShouldTransmitState_t
{
	SHOULDTRANSMIT_START = 0,
	SHOULDTRANSMIT_END
} ShouldTransmitState_t;

typedef enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED
} DataUpdateType_t;

typedef struct IClientNetworkable IClientNetworkable;

typedef struct IClientNetworkable_VMT
{
	IClientUnknown* (*GetIClientUnknown)(IClientNetworkable*);
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
} IClientNetworkable_VMT;

struct IClientNetworkable
{
	IClientNetworkable_VMT* vmt;
};

#endif
