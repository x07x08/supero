#if !defined(IntfIFileSystem_Included)
#define IntfIFileSystem_Included

#include "stdio.h"
#include "Types.h"
#include "Misc/IAppSystem.h"
#include "Main/MD5.h"

typedef void* FileHandle_t;
typedef void* (*FSAllocFunc_t)(const char* pszFilename, unsigned nBytes);
typedef uint32_t PathTypeQuery_t;
typedef int FileFindHandle_t;
typedef void* FileNameHandle_t;
struct FSAsyncControl_t__
{
	int unused;
};
typedef struct FSAsyncControl_t__* FSAsyncControl_t;
struct FSAsyncFile_t__
{
	int unused;
};
typedef struct FSAsyncFile_t__* FSAsyncFile_t;
typedef int WaitForResourcesHandle_t;
typedef void (*FileSystemLoggingFunc_t)(const char* fileName, const char* accessType);
typedef void (*FSDirtyDiskReportFunc_t)();
typedef void* FileCacheHandle_t;

typedef enum FileSystemSeek_t
{
	FILESYSTEM_SEEK_HEAD = SEEK_SET,
	FILESYSTEM_SEEK_CURRENT = SEEK_CUR,
	FILESYSTEM_SEEK_TAIL = SEEK_END,
} FileSystemSeek_t;
typedef enum FilesystemMountRetval_t
{
	FILESYSTEM_MOUNT_OK = 0,
	FILESYSTEM_MOUNT_FAILED,
} FilesystemMountRetval_t;
typedef enum SearchPathAdd_t
{
	PATH_ADD_TO_HEAD,
	PATH_ADD_TO_TAIL,
} SearchPathAdd_t;
typedef enum PathTypeFilter_t
{
	FILTER_NONE = 0,
	FILTER_CULLPACK = 1,
	FILTER_CULLNONPACK = 2,
} PathTypeFilter_t;
typedef enum FSAsyncStatus_t
{
	FSASYNC_ERR_NOT_MINE = -8,
	FSASYNC_ERR_RETRY_LATER = -7,
	FSASYNC_ERR_ALIGNMENT = -6,
	FSASYNC_ERR_FAILURE = -5,
	FSASYNC_ERR_READING = -4,
	FSASYNC_ERR_NOMEMORY = -3,
	FSASYNC_ERR_UNKNOWNID = -2,
	FSASYNC_ERR_FILEOPEN = -1,
	FSASYNC_OK = 0,
	FSASYNC_STATUS_PENDING,
	FSASYNC_STATUS_INPROGRESS,
	FSASYNC_STATUS_ABORTED,
	FSASYNC_STATUS_UNSERVICED,
} FSAsyncStatus_t;
typedef enum FileWarningLevel_t
{
	FILESYSTEM_WARNING = -1,
	FILESYSTEM_WARNING_QUIET = 0,
	FILESYSTEM_WARNING_REPORTUNCLOSED,
	FILESYSTEM_WARNING_REPORTUSAGE,
	FILESYSTEM_WARNING_REPORTALLACCESSES,
	FILESYSTEM_WARNING_REPORTALLACCESSES_READ,
	FILESYSTEM_WARNING_REPORTALLACCESSES_READWRITE,
	FILESYSTEM_WARNING_REPORTALLACCESSES_ASYNC,
} FileWarningLevel_t;
typedef enum DVDMode_t
{
	DVDMODE_OFF = 0,
	DVDMODE_STRICT = 1,
	DVDMODE_DEV = 2,
} DVDMode_t;
typedef enum EPureServerFileClass
{
	ePureServerFileClass_Unknown = -1,
	ePureServerFileClass_Any = 0,
	ePureServerFileClass_AnyTrusted,
	ePureServerFileClass_CheckHash,
} EPureServerFileClass;
typedef enum ECacheCRCType
{
	k_eCacheCRCType_SingleFile,
	k_eCacheCRCType_Directory,
	k_eCacheCRCType_Directory_Recursive
} ECacheCRCType;
typedef enum EFileCRCStatus
{
	k_eFileCRCStatus_CantOpenFile,
	k_eFileCRCStatus_GotCRC,
	k_eFileCRCStatus_FileInVPK
} EFileCRCStatus;

typedef struct FileAsyncRequest_t FileAsyncRequest_t;
typedef struct FileSystemStatistics FileSystemStatistics;
typedef struct FileHash_t FileHash_t;
typedef struct CUtlBuffer CUtlBuffer;
typedef struct CSysModule CSysModule;
typedef struct IThreadPool IThreadPool;
typedef struct KeyValues KeyValues;
typedef struct IAsyncFileFetch IAsyncFileFetch;
typedef struct IPureServerWhitelist IPureServerWhitelist;
typedef struct IFileList IFileList;
typedef struct CUnverifiedFileHash CUnverifiedFileHash;
typedef struct CMemoryFileBacking CMemoryFileBacking;
typedef void (*FSAsyncCallbackFunc_t)(const FileAsyncRequest_t* request, int nBytesRead, FSAsyncStatus_t err);

typedef struct IBaseFileSystem IBaseFileSystem;

typedef struct IBaseFileSystem_VMT
{
	int (*Read)(IBaseFileSystem*, void* pOutput, int size, FileHandle_t file);
	int (*Write)(IBaseFileSystem*, void const* pInput, int size, FileHandle_t file);
	FileHandle_t (*Open)(IBaseFileSystem*, const char* pFileName, const char* pOptions, const char* pathID);
	void (*Close)(IBaseFileSystem*, FileHandle_t file);
	void (*Seek)(IBaseFileSystem*, FileHandle_t file, int pos, FileSystemSeek_t seekType);
	unsigned int (*Tell)(IBaseFileSystem*, FileHandle_t file);
	unsigned int (*SizeHandle)(IBaseFileSystem*, FileHandle_t file);
	unsigned int (*Size)(IBaseFileSystem*, const char* pFileName, const char* pPathID);
	void (*Flush)(IBaseFileSystem*, FileHandle_t file);
	bool (*Precache)(IBaseFileSystem*, const char* pFileName, const char* pPathID);
	bool (*FileExists)(IBaseFileSystem*, const char* pFileName, const char* pPathID);
	bool (*IsFileWritable)(IBaseFileSystem*, char const* pFileName, const char* pPathID);
	bool (*SetFileWritable)(IBaseFileSystem*, char const* pFileName, bool writable, const char* pPathID);
	long (*GetFileTime)(IBaseFileSystem*, const char* pFileName, const char* pPathID);
	bool (*ReadFile)(IBaseFileSystem*, const char* pFileName, const char* pPath, CUtlBuffer* buf, int nMaxBytes, int nStartingByte, FSAllocFunc_t pfnAlloc);
	bool (*WriteFile)(IBaseFileSystem*, const char* pFileName, const char* pPath, CUtlBuffer* buf);
	bool (*UnzipFile)(IBaseFileSystem*, const char* pFileName, const char* pPath, const char* pDestination);
} IBaseFileSystem_VMT;

struct IBaseFileSystem
{
	IBaseFileSystem_VMT* vmt;
};

typedef enum KeyValuesPreloadType_t
{
	TYPE_VMT,
	TYPE_SOUNDEMITTER,
	TYPE_SOUNDSCAPE,
	NUM_PRELOAD_TYPES
} KeyValuesPreloadType_t;

typedef struct IFileSystem IFileSystem;

typedef struct IFileSystem_VMT
{
	IAppSystem_VMT vmt_IAppSystem;
	IBaseFileSystem_VMT vmt_IBaseFileSystem;

	bool (*IsSteam)(IFileSystem*); // const
	FilesystemMountRetval_t (*MountSteamContent)(IFileSystem*, int nExtraAppId);
	void (*AddSearchPath)(IFileSystem*, const char* pPath, const char* pathID, SearchPathAdd_t addType);
	bool (*RemoveSearchPath)(IFileSystem*, const char* pPath, const char* pathID);
	void (*RemoveAllSearchPaths)(IFileSystem*);
	void (*RemoveSearchPaths)(IFileSystem*, const char* szPathID);
	void (*MarkPathIDByRequestOnly)(IFileSystem*, const char* pPathID, bool bRequestOnly);
	const char* (*RelativePathToFullPath)(IFileSystem*, const char* pFileName, const char* pPathID, char* pDest, int maxLenInChars, PathTypeFilter_t pathFilter, PathTypeQuery_t* pPathType);
	int (*GetSearchPath)(IFileSystem*, const char* pathID, bool bGetPackFiles, char* pDest, int maxLenInChars);
	bool (*AddPackFile)(IFileSystem*, const char* fullpath, const char* pathID);
	void (*RemoveFile)(IFileSystem*, char const* pRelativePath, const char* pathID);
	bool (*RenameFile)(IFileSystem*, char const* pOldPath, char const* pNewPath, const char* pathID);
	void (*CreateDirHierarchy)(IFileSystem*, const char* path, const char* pathID);
	bool (*IsDirectory)(IFileSystem*, const char* pFileName, const char* pathID);
	void (*FileTimeToString)(IFileSystem*, char* pStrip, int maxCharsIncludingTerminator, long fileTime);
	void (*SetBufferSize)(IFileSystem*, FileHandle_t file, unsigned nBytes);
	bool (*IsOk)(IFileSystem*, FileHandle_t file);
	bool (*EndOfFile)(IFileSystem*, FileHandle_t file);
	char* (*ReadLine)(IFileSystem*, char* pOutput, int maxChars, FileHandle_t file);
	int (*FPrintf)(IFileSystem*, FileHandle_t file, const char* pFormat, ...);
	CSysModule* (*LoadModule)(IFileSystem*, const char* pFileName, const char* pPathID, bool bValidatedDllOnly);
	void (*UnloadModule)(IFileSystem*, CSysModule* pModule);
	const char* (*FindFirst)(IFileSystem*, const char* pWildCard, FileFindHandle_t* pHandle);
	const char* (*FindNext)(IFileSystem*, FileFindHandle_t handle);
	bool (*FindIsDirectory)(IFileSystem*, FileFindHandle_t handle);
	void (*FindClose)(IFileSystem*, FileFindHandle_t handle);
	const char* (*FindFirstEx)(IFileSystem*, const char* pWildCard, const char* pPathID, FileFindHandle_t* pHandle);
	const char* (*GetLocalPath)(IFileSystem*, const char* pFileName, char* pDest, int maxLenInChars);
	bool (*FullPathToRelativePath)(IFileSystem*, const char* pFullpath, char* pDest, int maxLenInChars);
	bool (*GetCurrentDirectory)(IFileSystem*, char* pDirectory, int maxlen);
	FileNameHandle_t (*FindOrAddFileName)(IFileSystem*, char const* pFileName);
	bool (*String)(IFileSystem*, const FileNameHandle_t* handle, char* buf, int buflen);
	FSAsyncStatus_t (*AsyncReadMultiple)(IFileSystem*, const FileAsyncRequest_t* pRequests, int nRequests, FSAsyncControl_t* phControls);
	FSAsyncStatus_t (*AsyncAppend)(IFileSystem*, const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, FSAsyncControl_t* pControl);
	FSAsyncStatus_t (*AsyncAppendFile)(IFileSystem*, const char* pAppendToFileName, const char* pAppendFromFileName, FSAsyncControl_t* pControl);
	void (*AsyncFinishAll)(IFileSystem*, int iToPriority);
	void (*AsyncFinishAllWrites)(IFileSystem*);
	FSAsyncStatus_t (*AsyncFlush)(IFileSystem*);
	bool (*AsyncSuspend)(IFileSystem*);
	bool (*AsyncResume)(IFileSystem*);
	void (*AsyncAddFetcher)(IFileSystem*, IAsyncFileFetch* pFetcher);
	void (*AsyncRemoveFetcher)(IFileSystem*, IAsyncFileFetch* pFetcher);
	FSAsyncStatus_t (*AsyncBeginRead)(IFileSystem*, const char* pszFile, FSAsyncFile_t* phFile);
	FSAsyncStatus_t (*AsyncEndRead)(IFileSystem*, FSAsyncFile_t hFile);
	FSAsyncStatus_t (*AsyncFinish)(IFileSystem*, FSAsyncControl_t hControl, bool wait);
	FSAsyncStatus_t (*AsyncGetResult)(IFileSystem*, FSAsyncControl_t hControl, void** ppData, int* pSize);
	FSAsyncStatus_t (*AsyncAbort)(IFileSystem*, FSAsyncControl_t hControl);
	FSAsyncStatus_t (*AsyncStatus)(IFileSystem*, FSAsyncControl_t hControl);
	FSAsyncStatus_t (*AsyncSetPriority)(IFileSystem*, FSAsyncControl_t hControl, int newPriority);
	void (*AsyncAddRef)(IFileSystem*, FSAsyncControl_t hControl);
	void (*AsyncRelease)(IFileSystem*, FSAsyncControl_t hControl);
	WaitForResourcesHandle_t (*WaitForResources)(IFileSystem*, const char* resourcelist);
	bool (*GetWaitForResourcesProgress)(IFileSystem*, WaitForResourcesHandle_t handle, float* progress /* out */, bool* complete /* out */);
	void (*CancelWaitForResources)(IFileSystem*, WaitForResourcesHandle_t handle);
	int (*HintResourceNeed)(IFileSystem*, const char* hintlist, int forgetEverything);
	bool (*IsFileImmediatelyAvailable)(IFileSystem*, const char* pFileName);
	void (*GetLocalCopy)(IFileSystem*, const char* pFileName);
	void (*PrintOpenedFiles)(IFileSystem*);
	void (*PrintSearchPaths)(IFileSystem*);
	void (*SetWarningFunc)(IFileSystem*, void (*pfnWarning)(const char* fmt, ...));
	void (*SetWarningLevel)(IFileSystem*, FileWarningLevel_t level);
	void (*AddLoggingFunc)(IFileSystem*, void (*pfnLogFunc)(const char* fileName, const char* accessType));
	void (*RemoveLoggingFunc)(IFileSystem*, FileSystemLoggingFunc_t logFunc);
	const FileSystemStatistics* (*GetFilesystemStatistics)(IFileSystem*);
	FileHandle_t (*OpenEx)(IFileSystem*, const char* pFileName, const char* pOptions, unsigned flags, const char* pathID, char** ppszResolvedFilename);
	int (*ReadEx)(IFileSystem*, void* pOutput, int sizeDest, int size, FileHandle_t file);
	int (*ReadFileEx)(IFileSystem*, const char* pFileName, const char* pPath, void** ppBuf, bool bNullTerminate, bool bOptimalAlloc, int nMaxBytes, int nStartingByte, FSAllocFunc_t pfnAlloc);
	FileNameHandle_t (*FindFileName)(IFileSystem*, char const* pFileName);
	void (*SetupPreloadData)(IFileSystem*);
	void (*DiscardPreloadData)(IFileSystem*);
	void (*LoadCompiledKeyValues)(IFileSystem*, KeyValuesPreloadType_t type, char const* archiveFile);
	KeyValues* (*LoadKeyValues)(IFileSystem*, KeyValuesPreloadType_t type, char const* filename, char const* pPathID);
	bool (*LoadKeyValues)(IFileSystem*, KeyValues* head, KeyValuesPreloadType_t type, char const* filename, char const* pPathID);
	bool (*ExtractRootKeyName)(IFileSystem*, KeyValuesPreloadType_t type, char* outbuf, size_t bufsize, char const* filename, char const* pPathID);
	FSAsyncStatus_t (*AsyncWrite)(IFileSystem*, const char* pFileName, const void* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend, FSAsyncControl_t* pControl);
	FSAsyncStatus_t (*AsyncWriteFile)(IFileSystem*, const char* pFileName, const CUtlBuffer* pSrc, int nSrcBytes, bool bFreeMemory, bool bAppend, FSAsyncControl_t* pControl);
	FSAsyncStatus_t (*AsyncReadMultipleCreditAlloc)(IFileSystem*, const FileAsyncRequest_t* pRequests, int nRequests, const char* pszFile, int line, FSAsyncControl_t* phControls);
	bool (*GetFileTypeForFullPath)(IFileSystem*, char const* pFullPath, wchar_t* buf, size_t bufSizeInBytes);
	bool (*ReadToBuffer)(IFileSystem*, FileHandle_t hFile, CUtlBuffer* buf, int nMaxBytes, FSAllocFunc_t pfnAlloc);
	bool (*GetOptimalIOConstraints)(IFileSystem*, FileHandle_t hFile, unsigned* pOffsetAlign, unsigned* pSizeAlign, unsigned* pBufferAlign);
	void* (*AllocOptimalReadBuffer)(IFileSystem*, FileHandle_t hFile, unsigned nSize, unsigned nOffset);
	void (*FreeOptimalReadBuffer)(IFileSystem*, void*);
	void (*BeginMapAccess)(IFileSystem*);
	void (*EndMapAccess)(IFileSystem*);
	bool (*FullPathToRelativePathEx)(IFileSystem*, const char* pFullpath, const char* pPathId, char* pDest, int maxLenInChars);
	int (*GetPathIndex)(IFileSystem*, const FileNameHandle_t* handle);
	long (*GetPathTime)(IFileSystem*, const char* pPath, const char* pPathID);
	DVDMode_t (*GetDVDMode)(IFileSystem*);
	void (*EnableWhitelistFileTracking)(IFileSystem*, bool bEnable, bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes);
	void (*RegisterFileWhitelist)(IFileSystem*, IPureServerWhitelist* pWhiteList, IFileList** pFilesToReload);
	void (*MarkAllCRCsUnverified)(IFileSystem*);
	void (*CacheFileCRCs)(IFileSystem*, const char* pPathname, ECacheCRCType eType, IFileList* pFilter);
	EFileCRCStatus (*CheckCachedFileHash)(IFileSystem*, const char* pPathID, const char* pRelativeFilename, int nFileFraction, FileHash_t* pFileHash);
	int (*GetUnverifiedFileHashes)(IFileSystem*, CUnverifiedFileHash* pFiles, int nMaxFiles);
	int (*GetWhitelistSpewFlags)(IFileSystem*);
	void (*SetWhitelistSpewFlags)(IFileSystem*, int flags);
	void (*InstallDirtyDiskReportFunc)(IFileSystem*, FSDirtyDiskReportFunc_t func);
	FileCacheHandle_t (*CreateFileCache)(IFileSystem*);
	void (*AddFilesToFileCache)(IFileSystem*, FileCacheHandle_t cacheId, const char** ppFileNames, int nFileNames, const char* pPathID);
	bool (*IsFileCacheFileLoaded)(IFileSystem*, FileCacheHandle_t cacheId, const char* pFileName);
	bool (*IsFileCacheLoaded)(IFileSystem*, FileCacheHandle_t cacheId);
	void (*DestroyFileCache)(IFileSystem*, FileCacheHandle_t cacheId);
	bool (*RegisterMemoryFile)(IFileSystem*, CMemoryFileBacking* pFile, CMemoryFileBacking** ppExistingFileWithRef);
	void (*UnregisterMemoryFile)(IFileSystem*, CMemoryFileBacking* pFile);
	void (*CacheAllVPKFileHashes)(IFileSystem*, bool bCacheAllVPKHashes, bool bRecalculateAndCheckHashes);
	bool (*CheckVPKFileHash)(IFileSystem*, int PackFileID, int nPackFileNumber, int nFileFraction, MD5Value_t* md5Value);
	void (*NotifyFileUnloaded)(IFileSystem*, const char* pszFilename, const char* pPathId);
	bool (*GetCaseCorrectFullPath_Ptr)(IFileSystem*, const char* pFullPath, char* pDest, int maxLenInChars);
} IFileSystem_VMT;

struct IFileSystem
{
	IFileSystem_VMT* vmt;
};

#define IFileSystem_REFL "IFileSystem"
#define IFileSystem_SIGNATURE "VFileSystem022"
#define IFileSystem_MODULE "filesystem_stdio"

#endif
