#if !defined(MiscCInterpolatedVar_Included)
#define MiscCInterpolatedVar_Included

#include "Misc/BaseTypes.h"
#include "Misc/CommonMacros.h"

typedef struct CInterpolatedVarEntryBase
{
	float changetime;
	int count;
	void* value;
} CInterpolatedVarEntryBase;

typedef struct CSimpleRingBuffer
{
	void* m_pElements;
	unsigned short m_maxElement;
	unsigned short m_firstElement;
	unsigned short m_count;
	unsigned short m_growSize;
} CSimpleRingBuffer;

typedef struct IInterpolatedVar IInterpolatedVar;

typedef struct IInterpolatedVar_VMT
{
	//virtual ~IInterpolatedVar() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*Setup)(IInterpolatedVar*, void* pValue, int type);
	void (*SetInterpolationAmount)(IInterpolatedVar*, float seconds);
	void (*NoteLastNetworkedValue)(IInterpolatedVar*);
	bool (*NoteChanged)(IInterpolatedVar*, float changetime, bool bUpdateLastNetworkedValue);
	void (*Reset)(IInterpolatedVar*);
	int (*Interpolate)(IInterpolatedVar*, float currentTime);
	int (*GetType)(IInterpolatedVar*);
	void (*RestoreToLastNetworked)(IInterpolatedVar*);
	void (*Copy)(IInterpolatedVar*, IInterpolatedVar* pSrc);
	const char* (*GetDebugName)(IInterpolatedVar*);
	void (*SetDebugName)(IInterpolatedVar*, const char* pName);
	void (*SetDebug)(IInterpolatedVar*, bool bDebug);
} IInterpolatedVar_VMT;

struct IInterpolatedVar
{
	IInterpolatedVar_VMT* vmt;
};

typedef CInterpolatedVarEntryBase CInterpolatedVarEntry; // <Type, IS_ARRAY>
typedef CSimpleRingBuffer CVarHistory; // CInterpolatedVarEntry

typedef struct CInterpolatedVarArrayBase
{
	IInterpolatedVar* m_vTable;
	void* m_pValue;
	CVarHistory m_VarHistory;
	void* m_LastNetworkedValue;
	float m_LastNetworkedTime;
	byte m_fType;
	byte m_nMaxCount;
	byte* m_bLooping;
	float m_InterpolationAmount;
	const char* m_pDebugName;
	bool m_bDebug : 1;
} CInterpolatedVarArrayBase;

typedef CInterpolatedVarArrayBase CInterpolatedVar;

#endif
