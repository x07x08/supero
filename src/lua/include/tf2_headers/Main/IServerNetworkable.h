#if !defined(MainIServerNetworkable_Included)
#define MainIServerNetworkable_Included

#include "Main/IHandleEntity.h"
#include "Misc/BitBuf.h"
#include "Misc/Edict.h"
#include "Misc/ServerClass.h"
#include "Definitions.h"

#define MAX_FAST_ENT_CLUSTERS 4

typedef struct CBaseNetworkable CBaseNetworkable;

typedef struct PVSInfo_t
{
	short m_nHeadNode;
	short m_nClusterCount;
	unsigned short* m_pClusters;
	short m_nAreaNum;
	short m_nAreaNum2;
	float m_vCenter[3];
	unsigned short m_pClustersInline[MAX_FAST_ENT_CLUSTERS];
	// friend class CVEngineServer;
} PVSInfo_t;

typedef struct IServerNetworkable IServerNetworkable;

typedef struct IServerNetworkable_VMT
{
	IHandleEntity* (*GetEntityHandle)(IServerNetworkable*);
	ServerClass* (*GetServerClass)(IServerNetworkable*);
	edict_t* (*GetEdict)(IServerNetworkable*); // const
	const char* (*GetClassName)(IServerNetworkable*); // const
	void (*Release)(IServerNetworkable*);
	int (*AreaNum)(IServerNetworkable*); // const
	CBaseNetworkable* (*GetBaseNetworkable)(IServerNetworkable*);
	CBaseEntity* (*GetBaseEntity)(IServerNetworkable*);
	PVSInfo_t* (*GetPVSInfo)(IServerNetworkable*);
	//virtual ~IServerNetworkable() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)
} IServerNetworkable_VMT;

struct IServerNetworkable
{
	IServerNetworkable_VMT* vmt;
};

#endif
