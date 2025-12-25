#if !defined(MiscServerNetworkProperty_Included)
#define MiscServerNetworkProperty_Included

#include "Misc/ServerClass.h"
#include "Main/IServerNetworkable.h"
#include "Misc/Datamap.h"
#include "Main/CBaseHandle.h"
#include "Misc/TimedEventMgr.h"

typedef struct CServerNetworkProperty CServerNetworkProperty;

typedef struct CServerNetworkProperty_VMT
{
	IServerNetworkable_VMT vmt_IServerNetworkable;
	DECLARE_DATADESC(CServerNetworkProperty);

#if defined(__linux__)
	IEventRegisterCallback_VMT vmt_IEventRegisterCallback;
#endif
} CServerNetworkProperty_VMT;

struct CServerNetworkProperty
{
	CServerNetworkProperty_VMT* vmt;

	CBaseEntity* m_pOuter;
	// CBaseTransmitProxy *m_pTransmitProxy;
	edict_t* m_pPev;
	PVSInfo_t m_PVSInfo;
	ServerClass* m_pServerClass;

	// NOTE: This state is 'owned' by the entity. It's only copied here
	// also to help improve cache performance in networking code.
	EHANDLE m_hParent;

	// Counters for SetUpdateInterval.
	CEventRegister m_TimerEvent;
	bool m_bPendingStateChange : 1;
};

#endif
