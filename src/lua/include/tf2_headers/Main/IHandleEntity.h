#if !defined(MainIHandleEntity_Included)
#define MainIHandleEntity_Included

#include "Misc/CommonMacros.h"

typedef struct CBaseHandle CBaseHandle;
typedef struct IHandleEntity IHandleEntity;

typedef struct IHandleEntity_VMT
{
	// This was a pain to find
	//virtual ~IHandleEntity() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*SetRefEHandle)(IHandleEntity*, const CBaseHandle* handle);
	const CBaseHandle* (*GetRefEHandle)(IHandleEntity*); // const
} IHandleEntity_VMT;

struct IHandleEntity
{
	IHandleEntity_VMT* vmt;
};

#endif