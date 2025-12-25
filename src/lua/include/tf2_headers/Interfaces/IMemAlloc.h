#if !defined(IntfIMemAlloc_Included)
#define IntfIMemAlloc_Included

#include "Interfaces/Interface.h"
#include "Misc/BaseTypes.h"

typedef size_t (*MemAllocFailHandler_t)(size_t);
typedef struct IVirtualMemorySection IVirtualMemorySection;
typedef struct _CrtMemState _CrtMemState;

typedef struct GenericMemoryStat_t
{
	const char* name;
	int value;
} GenericMemoryStat_t;

typedef struct IMemAlloc IMemAlloc;

typedef struct IMemAlloc_VMT
{
	void* (*Alloc)(IMemAlloc*, size_t nSize);
	void* (*Realloc)(IMemAlloc*, void* pMem, size_t nSize);
	void (*Free)(IMemAlloc*, void* pMem);
	void* (*Expand_NoLongerSupported1)(IMemAlloc*, void* pMem, size_t nSize);
	void* (*AllocDebug)(IMemAlloc*, size_t nSize, const char* pFileName, int nLine);
	void* (*ReallocDebug)(IMemAlloc*, void* pMem, size_t nSize, const char* pFileName, int nLine);
	void (*FreeDebug)(IMemAlloc*, void* pMem, const char* pFileName, int nLine);
	void* (*Expand_NoLongerSupported2)(IMemAlloc*, void* pMem, size_t nSize, const char* pFileName, int nLine);
	size_t (*GetSize)(IMemAlloc*, void* pMem);
	void (*PushAllocDbgInfo)(IMemAlloc*, const char* pFileName, int nLine);
	void (*PopAllocDbgInfo)(IMemAlloc*);
	int32 (*CrtSetBreakAlloc)(IMemAlloc*, int32 lNewBreakAlloc);
	int (*CrtSetReportMode)(IMemAlloc*, int nReportType, int nReportMode);
	int (*CrtIsValidHeapPointer)(IMemAlloc*, const void* pMem);
	int (*CrtIsValidPointer)(IMemAlloc*, const void* pMem, unsigned int size, int access);
	int (*CrtCheckMemory)(IMemAlloc*);
	int (*CrtSetDbgFlag)(IMemAlloc*, int nNewFlag);
	void (*CrtMemCheckpoint)(IMemAlloc*, _CrtMemState* pState);
	void (*DumpStats)(IMemAlloc*);
	void (*DumpStatsFileBase)(IMemAlloc*, char const* pchFileBase);
	size_t (*ComputeMemoryUsedBy)(IMemAlloc*, char const* pchSubStr);
	void* (*CrtSetReportFile)(IMemAlloc*, int nRptType, void* hFile);
	void* (*CrtSetReportHook)(IMemAlloc*, void* pfnNewHook);
	int (*CrtDbgReport)(IMemAlloc*, int nRptType, const char* szFile, int nLine, const char* szModule, const char* pMsg);
	int (*heapchk)(IMemAlloc*);
	bool (*IsDebugHeap)(IMemAlloc*);
	void (*GetActualDbgInfo)(IMemAlloc*, const char** pFileName, int* nLine);
	void (*RegisterAllocation)(IMemAlloc*, const char* pFileName, int nLine, size_t nLogicalSize, size_t nActualSize, unsigned nTime);
	void (*RegisterDeallocation)(IMemAlloc*, const char* pFileName, int nLine, size_t nLogicalSize, size_t nActualSize, unsigned nTime);
	int (*GetVersion)(IMemAlloc*);
	void (*CompactHeap)(IMemAlloc*);
	MemAllocFailHandler_t (*SetAllocFailHandler)(IMemAlloc*, MemAllocFailHandler_t pfnMemAllocFailHandler);
	void (*DumpBlockStats)(IMemAlloc*, void*);
	void (*SetStatsExtraInfo)(IMemAlloc*, const char* pMapName, const char* pComment);
	size_t (*MemoryAllocFailed)(IMemAlloc*);
	//void (*CompactIncremental)(IMemAlloc*);
	//void (*OutOfMemory)(IMemAlloc*, size_t nBytesAttempted);
	//void* (*RegionAlloc)(IMemAlloc*, int region, size_t nSize);
	//void* (*RegionAllocDebug)(IMemAlloc*, int region, size_t nSize, const char* pFileName, int nLine);
	//void (*GlobalMemoryStatus)(IMemAlloc*, size_t* pUsedMemory, size_t* pFreeMemory);
	//IVirtualMemorySection* (*AllocateVirtualMemorySection)(IMemAlloc*, size_t numMaxBytes);
	//int (*GetGenericMemoryStats)(IMemAlloc*, GenericMemoryStat_t** ppMemoryStats);
	//virtual ~IMemAlloc() {};
	//uint32 (*GetDebugInfoSize)(IMemAlloc*);
	//void (*SaveDebugInfo)(IMemAlloc*, void* pvDebugInfo);
	//void (*RestoreDebugInfo)(IMemAlloc*, const void* pvDebugInfo);
	//void (*InitDebugInfo)(IMemAlloc*, void* pvDebugInfo, const char* pchRootFileName, int nLine);
} IMemAlloc_VMT;

struct IMemAlloc
{
	IMemAlloc_VMT* vmt;
};

#define IMemAlloc_REFL "IMemAlloc"
#define IMemAlloc_SIGNATURE "g_pMemAlloc"
#define IMemAlloc_MODULE "tier0"

#endif
