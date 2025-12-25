#if !defined(MainCGameTrace_Included)
#define MainCGameTrace_Included

#include "Main/CModel.h"
#include "Main/UtlVector.h"
#include "Main/IHandleEntity.h"
//#include "Misc/ISpatialPartition.h"

typedef struct CBaseEntity CBaseEntity;

typedef struct CGameTrace
{
	CBaseTrace m_CBaseTrace;

	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	CBaseEntity* m_pEnt;
	int hitbox;
} CGameTrace;

typedef CGameTrace trace_t;

#endif
