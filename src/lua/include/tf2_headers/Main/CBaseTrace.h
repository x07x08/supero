#if !defined(MainCBaseTrace_Included)
#define MainCBaseTrace_Included

#include "Types.h"

#define DISPSURF_FLAG_SURFACE (1 << 0)
#define DISPSURF_FLAG_WALKABLE (1 << 1)
#define DISPSURF_FLAG_BUILDABLE (1 << 2)
#define DISPSURF_FLAG_SURFPROP1 (1 << 3)
#define DISPSURF_FLAG_SURFPROP2 (1 << 4)

typedef struct cplane_t
{
	Vector normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
} cplane_t;

#define CPLANE_NORMAL_X 0
#define CPLANE_NORMAL_Y 4
#define CPLANE_NORMAL_Z 8
#define CPLANE_DIST 12
#define CPLANE_TYPE 16
#define CPLANE_SIGNBITS 17
#define CPLANE_PAD0 18
#define CPLANE_PAD1 19

#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2
#define PLANE_ANYX 3
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

typedef struct CBaseTrace
{
	Vector startpos;
	Vector endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;
} CBaseTrace;

#endif
