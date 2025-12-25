#if !defined(MiscVCollide_Included)
#define MiscVCollide_Included

typedef struct CPhysCollide CPhysCollide;

typedef struct vcollide_t
{
	unsigned short solidCount : 15;
	unsigned short isPacked : 1;
	unsigned short descSize;
	CPhysCollide** solids;
	char* pKeyValues;
} vcollide_t;

#endif
