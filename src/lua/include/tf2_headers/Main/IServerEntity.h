#if !defined(MainIServerEntity_Included)
#define MainIServerEntity_Included

#include "Main/IServerNetworkable.h"
#include "Main/IServerUnknown.h"

typedef struct CMouthInfo CMouthInfo;
typedef struct SpatializationInfo_t SpatializationInfo_t;

typedef struct IServerEntity IServerEntity;

typedef struct IServerEntity_VMT
{
	IServerUnknown_VMT vmt_IServerUnknown;

	//virtual ~IServerEntity() {}
	int (*GetModelIndex)(IServerEntity*); // const
	const char* (*GetModelName)(IServerEntity*); // const
	void (*SetModelIndex)(IServerEntity*, int index);
} IServerEntity_VMT;

struct IServerEntity
{
	IServerEntity_VMT* vmt;
};

#endif
