#if !defined(MainIServerUnknown_Included)
#define MainIServerUnknown_Included

#include "Main/IHandleEntity.h"

typedef struct ICollideable ICollideable;
typedef struct IServerNetworkable IServerNetworkable;
typedef struct CBaseEntity CBaseEntity;

typedef struct IServerUnknown IServerUnknown;

typedef struct IServerUnknown_VMT
{
	IHandleEntity_VMT vmt_IHandleEntity;

	ICollideable* (*GetCollideable)(IServerUnknown*);
	IServerNetworkable* (*GetNetworkable)(IServerUnknown*);
	CBaseEntity* (*GetBaseEntity)(IServerUnknown*);
} IServerUnknown_VMT;

struct IServerUnknown
{
	IServerUnknown_VMT* vmt;
};

#endif
