#if !defined(MiscIVGuiMatInfo_Included)
#define MiscIVGuiMatInfo_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Misc/IVGuiMatInfoVar.h"
#include "Misc/CommonMacros.h"

typedef struct IVguiMatInfo IVguiMatInfo;

typedef struct IVguiMatInfo_VMT
{
	//virtual ~IVguiMatInfo() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	IVguiMatInfoVar* (*FindVarFactory)(IVguiMatInfo*, const char* varName, bool* found);
	int (*GetNumAnimationFrames)(IVguiMatInfo*);
} IVguiMatInfo_VMT;

struct IVguiMatInfo
{
	IVguiMatInfo_VMT* vmt;
};

#endif
