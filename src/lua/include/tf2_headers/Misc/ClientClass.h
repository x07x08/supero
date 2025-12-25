#if !defined(MiscClientClass_Included)
#define MiscClientClass_Included

#include "Misc/DTRecv.h"

#define DECLARE_CLIENTCLASS(TYPE)          \
	ClientClass* (*GetClientClass)(TYPE*); \
	int (*YouForgotToImplementOrDeclareClientClass)(TYPE*)

typedef struct IClientNetworkable IClientNetworkable;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

typedef struct ClientClass ClientClass;

struct ClientClass
{
	CreateClientClassFn m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};

#endif