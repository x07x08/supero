#if !defined(MainCModel_Included)
#define MainCModel_Included

#include "Main/CBaseTrace.h"
#include "Main/CBaseHandle.h"
#include "Misc/BSPFlags.h"
#include "Misc/VCollide.h"

typedef struct edict_t edict_t;
typedef struct model_t model_t;

#define AREA_SOLID 1
#define AREA_TRIGGERS 2

typedef struct cmodel_t
{
	Vector mins, maxs;
	Vector origin;
	int headnode;
	vcollide_t vcollisionData;
} cmodel_t;

typedef struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
} csurface_t;

typedef struct Ray_t
{
	VectorAligned m_Start;
	VectorAligned m_Delta;
	VectorAligned m_StartOffset;
	VectorAligned m_Extents;
	bool m_IsRay;
	bool m_IsSwept;
} Ray_t;

#endif
