#if !defined(MiscThreadTools_Included)
#define MiscThreadTools_Included

// Incomplete

#include "Misc/BaseTypes.h"

typedef size_t ThreadId_t;

typedef struct CThreadFastMutex
{
	ThreadId_t m_ownerID;
	int m_depth;
} CThreadFastMutex;

#endif
