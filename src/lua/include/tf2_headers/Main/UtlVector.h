#if !defined(MainUtlVector_Included)
#define MainUtlVector_Included

typedef struct CUtlMemory
{
	void* m_pMemory;
	int m_nAllocationCount;
	int m_nGrowSize;
} CUtlMemory;

typedef struct CUtlVector
{
	CUtlMemory m_Memory;
	int m_Size;
	void* m_pElements;
} CUtlVector;

#endif
