#if !defined(MiscDeformations_Included)
#define MiscDeformations_Included

#include "Types.h"

typedef enum DeformationType_t
{
	DEFORMATION_CLAMP_TO_BOX_IN_WORLDSPACE = 1
} DeformationType_t;

typedef struct DeformationBase_t
{
	DeformationType_t m_eType;
} DeformationBase_t;

typedef struct BoxDeformation_t
{
	DeformationBase_t m_DeformationBase;

	Vector m_SourceMins;
	float m_flPad0;
	Vector m_SourceMaxes;
	float m_flPad1;
	Vector m_ClampMins;
	float m_flPad2;
	Vector m_ClampMaxes;
	float m_flPad3;
} BoxDeformation_t;

#endif