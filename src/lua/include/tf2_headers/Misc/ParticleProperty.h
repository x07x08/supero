#if !defined(MiscParticleProperty_Included)
#define MiscParticleProperty_Included

// Incomplete

#include "Main/UtlVector.h"
#include "Misc/Datamap.h"

typedef struct CBaseEntity CBaseEntity;
typedef struct CParticleProperty CParticleProperty;

typedef struct CParticleProperty_VMT
{
	DECLARE_DATADESC(CParticleProperty);
} CParticleProperty_VMT;

struct CParticleProperty
{
	CParticleProperty_VMT* vmt;

	CBaseEntity* m_pOuter;
	CUtlVector m_ParticleEffects;
	int m_iDormancyChangedAtFrame;
};

#endif
