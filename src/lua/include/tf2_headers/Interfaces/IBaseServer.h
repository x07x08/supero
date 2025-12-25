#if !defined(IntfIBaseServer_Included)
#define IntfIBaseServer_Included

#include "Types.h"
#include "Interfaces/Interface.h"
#include "Misc/Datamap.h"
#include "Misc/DTSend.h"
#include "Misc/Edict.h"
#include "Misc/ServerClass.h"

typedef struct CGlobalVars CGlobalVars;
typedef struct CSaveRestoreData CSaveRestoreData;
typedef struct IServerGCLobby IServerGCLobby;
typedef int QueryCvarCookie_t;

typedef enum EQueryCvarValueStatusBServer
{
	eQueryCvarValueStatusBServer_ValueIntact = 0,
	eQueryCvarValueStatusBServer_CvarNotFound = 1,
	eQueryCvarValueStatusBServer_NotACvar = 2,
	eQueryCvarValueStatusBServer_CvarProtected = 3
} EQueryCvarValueStatusBServer;

typedef enum ePrepareLevelResourcesResult
{
	ePrepareLevelResources_Prepared,
	ePrepareLevelResources_InProgress
} ePrepareLevelResourcesResult;

typedef enum eCanProvideLevelResult
{
	eCanProvideLevel_CannotProvide,
	eCanProvideLevel_CanProvide,
	eCanProvideLevel_Possibly
} eCanProvideLevelResult;

typedef struct IBaseServerDLL IBaseServerDLL;

typedef struct IBaseServerDLL_VMT
{
	bool (*DLLInit)(IBaseServerDLL*, CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars* pGlobals);
	bool (*ReplayInit)(IBaseServerDLL*, CreateInterfaceFn fnReplayFactory);
	bool (*GameInit)(IBaseServerDLL*);
	bool (*LevelInit)(IBaseServerDLL*, char const* pMapName, char const* pMapEntities, char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background);
	void (*ServerActivate)(IBaseServerDLL*, edict_t* pEdictList, int edictCount, int clientMax);
	void (*GameFrame)(IBaseServerDLL*, bool simulating);
	void (*PreClientUpdate)(IBaseServerDLL*, bool simulating);
	void (*LevelShutdown)(IBaseServerDLL*);
	void (*GameShutdown)(IBaseServerDLL*);
	void (*DLLShutdown)(IBaseServerDLL*);
	float (*GetTickInterval)(IBaseServerDLL*); // const
	ServerClass* (*GetAllServerClasses)(IBaseServerDLL*);
	const char* (*GetGameDescription)(IBaseServerDLL*);
	void (*CreateNetworkStringTables)(IBaseServerDLL*);
	CSaveRestoreData* (*SaveInit)(IBaseServerDLL*, int size);
	void (*SaveWriteFields)(IBaseServerDLL*, CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	void (*SaveReadFields)(IBaseServerDLL*, CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	void (*SaveGlobalState)(IBaseServerDLL*, CSaveRestoreData*);
	void (*RestoreGlobalState)(IBaseServerDLL*, CSaveRestoreData*);
	void (*PreSave)(IBaseServerDLL*, CSaveRestoreData*);
	void (*Save)(IBaseServerDLL*, CSaveRestoreData*);
	void (*GetSaveComment)(IBaseServerDLL*, char* comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime);
	void (*WriteSaveHeaders)(IBaseServerDLL*, CSaveRestoreData*);
	void (*ReadRestoreHeaders)(IBaseServerDLL*, CSaveRestoreData*);
	void (*Restore)(IBaseServerDLL*, CSaveRestoreData*, bool);
	bool (*IsRestoring)(IBaseServerDLL*);
	int (*CreateEntityTransitionList)(IBaseServerDLL*, CSaveRestoreData*, int);
	void (*BuildAdjacentMapList)(IBaseServerDLL*);
	bool (*GetUserMessageInfo)(IBaseServerDLL*, int msg_type, char* name, int maxnamelength, int* size);
	CStandardSendProxies* (*GetStandardSendProxies)(IBaseServerDLL*);
	void (*PostInit)(IBaseServerDLL*);
	void (*Think)(IBaseServerDLL*, bool finalTick);
	void (*PreSaveGameLoaded)(IBaseServerDLL*, char const* pSaveName, bool bCurrentlyInGame);
	bool (*ShouldHideServer)(IBaseServerDLL*);
	void (*InvalidateMdlCache)(IBaseServerDLL*);
	void (*OnQueryCvarValueFinished)(IBaseServerDLL*, QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatusBServer eStatus, const char* pCvarName, const char* pCvarValue);
	void (*GameServerSteamAPIActivated)(IBaseServerDLL*);
	void (*GameServerSteamAPIShutdown)(IBaseServerDLL*);
	void (*SetServerHibernation)(IBaseServerDLL*, bool bHibernating);
	IServerGCLobby* (*GetServerGCLobby)(IBaseServerDLL*);
	const char* (*GetServerBrowserMapOverride)(IBaseServerDLL*);
	const char* (*GetServerBrowserGameData)(IBaseServerDLL*);
	void (*Status)(IBaseServerDLL*, void (*print)(const char* fmt, ...));
	void (*PrepareLevelResources)(IBaseServerDLL*, char* pszMapName, size_t nMapNameSize, char* pszMapFile, size_t nMapFileSize);
	ePrepareLevelResourcesResult (*AsyncPrepareLevelResources)(IBaseServerDLL*, char* pszMapName, size_t nMapNameSize, char* pszMapFile, size_t nMapFileSize, float* flProgress);
	eCanProvideLevelResult (*CanProvideLevel)(IBaseServerDLL*, char* pMapName, int nMapNameMax);
	bool (*IsManualMapChangeOkay)(IBaseServerDLL*, const char** pszReason);
} IBaseServerDLL_VMT;

struct IBaseServerDLL
{
	IBaseServerDLL_VMT* vmt;
};

#define IBaseServerDLL_REFL "IBaseServerDLL"
#define IBaseServerDLL_SIGNATURE "ServerGameDLL012"
#define IBaseServerDLL_MODULE "server"

#endif
