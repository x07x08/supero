#if !defined(MiscServerClass_Included)
#define MiscServerClass_Included

#include "Misc/DTSend.h"

#define DECLARE_SERVERCLASS(TYPE)          \
	ServerClass* (*GetServerClass)(TYPE*); \
	int (*YouForgotToImplementOrDeclareServerClass)(TYPE*)

typedef struct ServerClass ServerClass;

struct ServerClass
{
	const char* m_pNetworkName;
	SendTable* m_pTable;
	ServerClass* m_pNext;
	int m_ClassID;
	int m_InstanceBaselineIndex;
};

#endif
