#if !defined(IntfIClientEntityList_Included)
#define IntfIClientEntityList_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Main/CBaseHandle.h"

typedef struct IClientEntity IClientEntity;
// typedef struct ClientClass ClientClass;
typedef struct IClientNetworkable IClientNetworkable;
typedef struct IClientUnknown IClientUnknown;

typedef struct IClientEntityList IClientEntityList;

typedef struct IClientEntityList_VMT
{
	IClientNetworkable* (*GetClientNetworkable)(IClientEntityList*, int entnum);
	IClientNetworkable* (*GetClientNetworkableFromHandle)(IClientEntityList*, CBaseHandle hEnt);
	IClientUnknown* (*GetClientUnknownFromHandle)(IClientEntityList*, CBaseHandle hEnt);
	IClientEntity* (*GetClientEntity)(IClientEntityList*, int entnum);
	IClientEntity* (*GetClientEntityFromHandle)(IClientEntityList*, CBaseHandle hEnt);
	int (*NumberOfEntities)(IClientEntityList*, bool bIncludeNonNetworkable);
	int (*GetHighestEntityIndex)(IClientEntityList*);
	void (*SetMaxEntities)(IClientEntityList*, int maxents);
	int (*GetMaxEntities)(IClientEntityList*);
} IClientEntityList_VMT;

struct IClientEntityList
{
	IClientEntityList_VMT* vmt;
};

#define IClientEntityList_REFL "IClientEntityList"
#define IClientEntityList_SIGNATURE "VClientEntityList003"
#define IClientEntityList_MODULE "client"

#endif
