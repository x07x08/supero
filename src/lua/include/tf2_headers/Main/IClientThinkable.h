#if !defined(MainIClientThinkable_Included)
#define MainIClientThinkable_Included

#include "Main/IClientUnknown.h"

typedef struct CClientThinkHandlePtr CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

typedef struct IClientThinkable IClientThinkable;

typedef struct IClientThinkable_VMT
{
	IClientUnknown* (*GetIClientUnknown)(IClientThinkable*);
	void (*ClientThink)(IClientThinkable*);
	ClientThinkHandle_t (*GetThinkHandle)(IClientThinkable*);
	void (*SetThinkHandle)(IClientThinkable*, ClientThinkHandle_t hThink);
	void (*Release)(IClientThinkable*);
} IClientThinkable_VMT;

struct IClientThinkable
{
	IClientThinkable_VMT* vmt;
};

#endif
