#if !defined(MiscIRefCounted_Included)
#define MiscIRefCounted_Included

typedef struct IRefCounted IRefCounted;

typedef struct IRefCounted_VMT
{
	int (*AddRef)(IRefCounted*);
	int (*Release)(IRefCounted*);
} IRefCounted_VMT;

struct IRefCounted
{
	IRefCounted_VMT* vmt;
};

#endif