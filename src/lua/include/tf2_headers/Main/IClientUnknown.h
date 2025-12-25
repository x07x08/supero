#if !defined(MainIClientUnknown_Included)
#define MainIClientUnknown_Included

#include "Main/IHandleEntity.h"

typedef struct IClientNetworkable IClientNetworkable;
typedef struct C_BaseEntity C_BaseEntity;
typedef struct IClientRenderable IClientRenderable;
typedef struct ICollideable ICollideable;
typedef struct IClientEntity IClientEntity;
typedef struct IClientThinkable IClientThinkable;

typedef struct IClientUnknown IClientUnknown;

typedef struct IClientUnknown_VMT
{
	IHandleEntity_VMT vmt_IHandleEntity;

	ICollideable* (*GetCollideable)(IClientUnknown*);
	IClientNetworkable* (*GetClientNetworkable)(IClientUnknown*);
	IClientRenderable* (*GetClientRenderable)(IClientUnknown*);
	IClientEntity* (*GetIClientEntity)(IClientUnknown*);
	C_BaseEntity* (*GetBaseEntity)(IClientUnknown*);
	IClientThinkable* (*GetClientThinkable)(IClientUnknown*);
} IClientUnknown_VMT;

struct IClientUnknown
{
	IClientUnknown_VMT* vmt;
};

#endif
