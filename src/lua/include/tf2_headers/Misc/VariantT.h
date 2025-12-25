#if !defined(MiscVariantT_Included)
#define MiscVariantT_Included

// Incomplete (no fieldtype_t)

#include "Types.h"
#include "Main/CBaseHandle.h"

typedef struct variant_t
{
	union
	{
		bool bVal;
		const char* iszVal; // string_t
		int iVal;
		float flVal;
		float vecVal[3];
		color32 rgbaVal;
	};
	CHandle eVal;  // this can't be in the union because it has a constructor.

	int fieldType; // fieldtype_t
} variant_t;

#endif
