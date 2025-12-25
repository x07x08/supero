#if !defined(MainCBaseHandle_Included)
#define MainCBaseHandle_Included

typedef struct CBaseHandle
{
	unsigned long m_Index;
} CBaseHandle;

typedef CBaseHandle CHandle;
typedef CHandle EHANDLE;

#include "Main/IHandleEntity.h"

#endif