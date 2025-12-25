#if !defined(MiscIVGuiMatInfoVar_Included)
#define MiscIVGuiMatInfoVar_Included

#include "Misc/CommonMacros.h"

typedef struct IVguiMatInfoVar IVguiMatInfoVar;

typedef struct IVguiMatInfoVar_VMT
{
	//virtual ~IVguiMatInfoVar() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	int (*GetIntValue)(IVguiMatInfoVar*); // const
	void (*SetIntValue)(IVguiMatInfoVar*, int val);
} IVguiMatInfoVar_VMT;

struct IVguiMatInfoVar
{
	IVguiMatInfoVar_VMT* vmt;
};

#endif
