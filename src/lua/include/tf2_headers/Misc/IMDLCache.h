#if !defined(MiscIMDLCache_Included)
#define MiscIMDLCache_Included

#include "Misc/IAppSystem.h"

typedef struct studiohdr_t studiohdr_t;
typedef struct studiohwdata_t studiohwdata_t;
typedef struct vcollide_t vcollide_t;
typedef struct virtualmodel_t virtualmodel_t;
typedef struct vertexFileHeader_t vertexFileHeader_t;
typedef struct FileHeader_t FileHeader_t;

typedef unsigned short MDLHandle_t;

enum
{
	MDLHANDLE_INVALID = (MDLHandle_t)~0
};

typedef enum MDLCacheDataType_t
{
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK
} MDLCacheDataType_t;

typedef struct IMDLCacheNotify IMDLCacheNotify;

typedef struct IMDLCacheNotify_VMT
{
	void (*OnDataLoaded)(IMDLCacheNotify*, MDLCacheDataType_t type, MDLHandle_t handle);
	void (*OnDataUnloaded)(IMDLCacheNotify*, MDLCacheDataType_t type, MDLHandle_t handle);
} IMDLCacheNotify_VMT;

struct IMDLCacheNotify
{
	IMDLCacheNotify_VMT* vmt;
};

typedef enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,
	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
} MDLCacheFlush_t;

typedef struct IMDLCache IMDLCache;

typedef struct IMDLCache_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	void (*SetCacheNotify)(IMDLCache*, IMDLCacheNotify* pNotify);
	MDLHandle_t (*FindMDL)(IMDLCache*, const char* pMDLRelativePath);
	int (*AddRef)(IMDLCache*, MDLHandle_t handle);
	int (*Release)(IMDLCache*, MDLHandle_t handle);
	int (*GetRef)(IMDLCache*, MDLHandle_t handle);
	studiohdr_t* (*GetStudioHdr)(IMDLCache*, MDLHandle_t handle);
	studiohwdata_t* (*GetHardwareData)(IMDLCache*, MDLHandle_t handle);
	vcollide_t* (*GetVCollide)(IMDLCache*, MDLHandle_t handle);
	unsigned char* (*GetAnimBlock)(IMDLCache*, MDLHandle_t handle, int nBlock);
	virtualmodel_t* (*GetVirtualModel)(IMDLCache*, MDLHandle_t handle);
	int (*GetAutoplayList)(IMDLCache*, MDLHandle_t handle, unsigned short** pOut);
	vertexFileHeader_t* (*GetVertexData)(IMDLCache*, MDLHandle_t handle);
	void (*TouchAllData)(IMDLCache*, MDLHandle_t handle);
	void (*SetUserData)(IMDLCache*, MDLHandle_t handle, void* pData);
	void* (*GetUserData)(IMDLCache*, MDLHandle_t handle);
	bool (*IsErrorModel)(IMDLCache*, MDLHandle_t handle);
	void (*Flush)(IMDLCache*, MDLCacheFlush_t nFlushFlags);
	void (*FlushHandle)(IMDLCache*, MDLHandle_t handle, int nFlushFlags);
	const char* (*GetModelName)(IMDLCache*, MDLHandle_t handle);
	virtualmodel_t* (*GetVirtualModelFast)(IMDLCache*, const studiohdr_t* pStudioHdr, MDLHandle_t handle);
	void (*BeginLock)(IMDLCache*);
	void (*EndLock)(IMDLCache*);
	int* (*GetFrameUnlockCounterPtrOLD)(IMDLCache*);
	void (*FinishPendingLoads)(IMDLCache*);
	vcollide_t* (*GetVCollideEx)(IMDLCache*, MDLHandle_t handle, bool synchronousLoad);
	bool (*GetVCollideSize)(IMDLCache*, MDLHandle_t handle, int* pVCollideSize);
	bool (*GetAsyncLoad)(IMDLCache*, MDLCacheDataType_t type);
	bool (*SetAsyncLoad)(IMDLCache*, MDLCacheDataType_t type, bool bAsync);
	void (*BeginMapLoad)(IMDLCache*);
	void (*EndMapLoad)(IMDLCache*);
	void (*MarkAsLoaded)(IMDLCache*, MDLHandle_t handle);
	void (*InitPreloadData)(IMDLCache*, bool rebuild);
	void (*ShutdownPreloadData)(IMDLCache*);
	bool (*IsDataLoaded)(IMDLCache*, MDLHandle_t handle, MDLCacheDataType_t type);
	int* (*GetFrameUnlockCounterPtr)(IMDLCache*, MDLCacheDataType_t type);
	studiohdr_t* (*LockStudioHdr)(IMDLCache*, MDLHandle_t handle);
	void (*UnlockStudioHdr)(IMDLCache*, MDLHandle_t handle);
	bool (*PreloadModel)(IMDLCache*, MDLHandle_t handle);
	void (*ResetErrorModelStatus)(IMDLCache*, MDLHandle_t handle);
	void (*MarkFrame)(IMDLCache*);
} IMDLCache_VMT;

struct IMDLCache
{
	IMDLCache_VMT* vmt;
};

#endif
