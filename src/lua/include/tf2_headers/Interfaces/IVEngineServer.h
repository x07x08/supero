#if !defined(IntfIVEngineServer_Included)
#define IntfIVEngineServer_Included

#include "Main/CCollisionProperty.h"
#include "Main/IAchievementMgr.h"
#include "Main/INetChannelInfo.h"
#include "Main/IServerNetworkable.h"
#include "Misc/BitBuf.h"
#include "Misc/ChecksumCRC.h"
#include "Misc/Edict.h"
#include "Misc/ServerClass.h"
#include "Misc/VPlane.h"
// #include "../Steam/SteamClientPublic.h"
#include "Definitions.h"
#include "Main/KeyValues.h"

#if !defined(PRINTF_FORMAT_STRING)
#define PRINTF_FORMAT_STRING
#endif

typedef struct IRecipientFilter IRecipientFilter;
typedef struct client_textmessage_t client_textmessage_t;
typedef struct IScratchPad3D IScratchPad3D;
typedef struct CCheckTransmitInfo CCheckTransmitInfo;
typedef struct CGamestatsData CGamestatsData;
typedef struct bbox_t bbox_t;
typedef struct IServer IServer;
typedef int QueryCvarCookie_t;

#if !defined(CSTEAMID_DEFINED)
typedef struct CSteamID
{
	uint64_t bits;
} CSteamID;
#endif

typedef struct player_info_s_server
{
	char name[MAX_PLAYER_NAME_LENGTH];
	int userID;
	char guid[SIGNED_GUID_LEN + 1];
	uint32 friendsID;
	char friendsName[MAX_PLAYER_NAME_LENGTH];
	bool fakeplayer;
	bool ishltv;
	bool isreplay;
	CRC32_t customFiles[MAX_CUSTOM_FILES];
	unsigned char filesDownloaded;
} PlayerInfo_t;

typedef enum eFindMapResult
{
	eFindMap_Found,
	eFindMap_NotFound,
	eFindMap_FuzzyMatch,
	eFindMap_NonCanonical,
	eFindMap_PossiblyAvailable
} eFindMapResult;

typedef struct IVEngineServer IVEngineServer;

typedef struct IVEngineServer_VMT
{
	void (*ChangeLevel)(IVEngineServer*, const char* s1, const char* s2);
	int (*IsMapValid)(IVEngineServer*, const char* filename);
	bool (*IsDedicatedServer)(IVEngineServer*);
	int (*IsInEditMode)(IVEngineServer*);
	int (*PrecacheModel)(IVEngineServer*, const char* s, bool preload);
	int (*PrecacheSentenceFile)(IVEngineServer*, const char* s, bool preload);
	int (*PrecacheDecal)(IVEngineServer*, const char* name, bool preload);
	int (*PrecacheGeneric)(IVEngineServer*, const char* s, bool preload);
	bool (*IsModelPrecached)(IVEngineServer*, char const* s); // const
	bool (*IsDecalPrecached)(IVEngineServer*, char const* s); // const
	bool (*IsGenericPrecached)(IVEngineServer*, char const* s); // const
	int (*GetClusterForOrigin)(IVEngineServer*, const Vec3* org);
	int (*GetPVSForCluster)(IVEngineServer*, int cluster, int outputpvslength, unsigned char* outputpvs);
	bool (*CheckOriginInPVS)(IVEngineServer*, const Vec3* org, const unsigned char* checkpvs, int checkpvssize);
	bool (*CheckBoxInPVS)(IVEngineServer*, const Vec3* mins, const Vec3* maxs, const unsigned char* checkpvs, int checkpvssize);
	int (*GetPlayerUserId)(IVEngineServer*, const edict_t* e);
	const char* (*GetPlayerNetworkIDString)(IVEngineServer*, const edict_t* e);
	int (*GetEntityCount)(IVEngineServer*);
	int (*IndexOfEdict)(IVEngineServer*, const edict_t* pEdict);
	edict_t* (*PEntityOfEntIndex)(IVEngineServer*, int iEntIndex);
	INetChannelInfo* (*GetPlayerNetInfo)(IVEngineServer*, int playerIndex);
	edict_t* (*CreateEdict)(IVEngineServer*, int iForceEdictIndex);
	void (*RemoveEdict)(IVEngineServer*, edict_t* e);
	void* (*PvAllocEntPrivateData)(IVEngineServer*, long cb);
	void (*FreeEntPrivateData)(IVEngineServer*, void* pEntity);
	void* (*SaveAllocMemory)(IVEngineServer*, size_t num, size_t size);
	void (*SaveFreeMemory)(IVEngineServer*, void* pSaveMem);
	void (*EmitAmbientSound)(IVEngineServer*, int entindex, const Vec3* pos, const char* samp, float vol, soundlevel_t soundlevel, int fFlags, int pitch, float delay);
	void (*FadeClientVolume)(IVEngineServer*, const edict_t* pEdict, float fadePercent, float fadeOutSeconds, float holdTime, float fadeInSeconds);
	int (*SentenceGroupPick)(IVEngineServer*, int groupIndex, char* name, int nameBufLen);
	int (*SentenceGroupPickSequential)(IVEngineServer*, int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset);
	int (*SentenceIndexFromName)(IVEngineServer*, const char* pSentenceName);
	const char* (*SentenceNameFromIndex)(IVEngineServer*, int sentenceIndex);
	int (*SentenceGroupIndexFromName)(IVEngineServer*, const char* pGroupName);
	const char* (*SentenceGroupNameFromIndex)(IVEngineServer*, int groupIndex);
	float (*SentenceLength)(IVEngineServer*, int sentenceIndex);
	void (*ServerCommand)(IVEngineServer*, const char* str);
	void (*ServerExecute)(IVEngineServer*);
	void (*ClientCommand)(IVEngineServer*, edict_t* pEdict, PRINTF_FORMAT_STRING const char* szFmt, ...);
	void (*LightStyle)(IVEngineServer*, int style, const char* val);
	void (*StaticDecal)(IVEngineServer*, const Vec3* originInEntitySpace, int decalIndex, int entityIndex, int modelIndex, bool lowpriority);
	void (*Message_DetermineMulticastRecipients)(IVEngineServer*, bool usepas, const Vec3* origin, void* playerbits); // CBitVec<ABSOLUTE_PLAYER_LIMIT>*
	bf_write* (*EntityMessageBegin)(IVEngineServer*, int ent_index, ServerClass* ent_class, bool reliable);
	bf_write* (*UserMessageBegin)(IVEngineServer*, IRecipientFilter* filter, int msg_type);
	void (*MessageEnd)(IVEngineServer*);
	void (*ClientPrintf)(IVEngineServer*, edict_t* pEdict, const char* szMsg);
	void (*Con_NPrintf)(IVEngineServer*, int pos, PRINTF_FORMAT_STRING const char* fmt, ...);
	void (*Con_NXPrintf)(IVEngineServer*, const struct con_nprint_s* info, PRINTF_FORMAT_STRING const char* fmt, ...);
	void (*SetView)(IVEngineServer*, const edict_t* pClient, const edict_t* pViewent);
	float (*Time)(IVEngineServer*);
	void (*CrosshairAngle)(IVEngineServer*, const edict_t* pClient, float pitch, float yaw);
	void (*GetGameDir)(IVEngineServer*, char* szGetGameDir, int maxlength);
	int (*CompareFileTime)(IVEngineServer*, const char* filename1, const char* filename2, int* iCompare);
	bool (*LockNetworkStringTables)(IVEngineServer*, bool lock);
	edict_t* (*CreateFakeClient)(IVEngineServer*, const char* netname);
	const char* (*GetClientConVarValue)(IVEngineServer*, int clientIndex, const char* name);
	const char* (*ParseFile)(IVEngineServer*, const char* data, char* token, int maxlen);
	bool (*CopyFile)(IVEngineServer*, const char* source, const char* destination);
	void (*ResetPVS)(IVEngineServer*, byte* pvs, int pvssize);
	void (*AddOriginToPVS)(IVEngineServer*, const Vec3* origin);
	void (*SetAreaPortalState)(IVEngineServer*, int portalNumber, int isOpen);
	void (*PlaybackTempEntity)(IVEngineServer*, IRecipientFilter* filter, float delay, const void* pSender, const SendTable* pST, int classID);
	int (*CheckHeadnodeVisible)(IVEngineServer*, int nodenum, const byte* pvs, int vissize);
	int (*CheckAreasConnected)(IVEngineServer*, int area1, int area2);
	int (*GetArea)(IVEngineServer*, const Vec3* origin);
	void (*GetAreaBits)(IVEngineServer*, int area, unsigned char* bits, int buflen);
	bool (*GetAreaPortalPlane)(IVEngineServer*, Vec3 const* vViewOrigin, int portalKey, VPlane* pPlane);
	bool (*LoadGameState)(IVEngineServer*, char const* pMapName, bool createPlayers);
	void (*LoadAdjacentEnts)(IVEngineServer*, const char* pOldLevel, const char* pLandmarkName);
	void (*ClearSaveDir)(IVEngineServer*);
	const char* (*GetMapEntitiesString)(IVEngineServer*);
	client_textmessage_t* (*TextMessageGet)(IVEngineServer*, const char* pName);
	void (*LogPrint)(IVEngineServer*, const char* msg);
	void (*BuildEntityClusterList)(IVEngineServer*, edict_t* pEdict, PVSInfo_t* pPVSInfo);
	void (*SolidMoved)(IVEngineServer*, edict_t* pSolidEnt, ICollideable* pSolidCollide, const Vec3* pPrevAbsOrigin, bool testSurroundingBoundsOnly);
	void (*TriggerMoved)(IVEngineServer*, edict_t* pTriggerEnt, bool testSurroundingBoundsOnly);
	ISpatialPartition* (*CreateSpatialPartition)(IVEngineServer*, const Vec3* worldmin, const Vec3* worldmax);
	void (*DestroySpatialPartition)(IVEngineServer*, ISpatialPartition*);
	void (*DrawMapToScratchPad)(IVEngineServer*, IScratchPad3D* pPad, unsigned long iFlags);
	const void* (*GetEntityTransmitBitsForClient)(IVEngineServer*, int iClientIndex); // CBitVec<MAX_EDICTS>*
	bool (*IsPaused)(IVEngineServer*);
	void (*ForceExactFile)(IVEngineServer*, const char* s);
	void (*ForceModelBounds)(IVEngineServer*, const char* s, const Vec3* mins, const Vec3* maxs);
	void (*ClearSaveDirAfterClientLoad)(IVEngineServer*);
	void (*SetFakeClientConVarValue)(IVEngineServer*, edict_t* pEntity, const char* cvar, const char* value);
	void (*ForceSimpleMaterial)(IVEngineServer*, const char* s);
	int (*IsInCommentaryMode)(IVEngineServer*);
	void (*SetAreaPortalStates)(IVEngineServer*, const int* portalNumbers, const int* isOpen, int nPortals);
	void (*NotifyEdictFlagsChange)(IVEngineServer*, int iEdict);
	const CCheckTransmitInfo* (*GetPrevCheckTransmitInfo)(IVEngineServer*, edict_t* pPlayerEdict);
	CSharedEdictChangeInfo* (*GetSharedEdictChangeInfo)(IVEngineServer*);
	void (*AllowImmediateEdictReuse)(IVEngineServer*);
	bool (*IsInternalBuild)(IVEngineServer*);
	IChangeInfoAccessor* (*GetChangeAccessor)(IVEngineServer*, const edict_t* pEdict);
	char const* (*GetMostRecentlyLoadedFileName)(IVEngineServer*);
	char const* (*GetSaveFileName)(IVEngineServer*);
	void (*MultiplayerEndGame)(IVEngineServer*);
	void (*ChangeTeam)(IVEngineServer*, const char* pTeamName);
	void (*CleanUpEntityClusterList)(IVEngineServer*, PVSInfo_t* pPVSInfo);
	void (*SetAchievementMgr)(IVEngineServer*, IAchievementMgr* pAchievementMgr);
	IAchievementMgr* (*GetAchievementMgr)(IVEngineServer*);
	int (*GetAppID)(IVEngineServer*);
	bool (*IsLowViolence)(IVEngineServer*);
	QueryCvarCookie_t (*StartQueryCvarValue)(IVEngineServer*, edict_t* pPlayerEntity, const char* pName);
	void (*InsertServerCommand)(IVEngineServer*, const char* str);
	bool (*GetPlayerInfo)(IVEngineServer*, int ent_num, PlayerInfo_t* pinfo);
	bool (*IsClientFullyAuthenticated)(IVEngineServer*, edict_t* pEdict);
	void (*SetDedicatedServerBenchmarkMode)(IVEngineServer*, bool bBenchmarkMode);
	void (*SetGamestatsData)(IVEngineServer*, CGamestatsData* pGamestatsData);
	CGamestatsData* (*GetGamestatsData)(IVEngineServer*);
	const CSteamID* (*GetClientSteamID)(IVEngineServer*, edict_t* pPlayerEdict);
	const CSteamID* (*GetGameServerSteamID)(IVEngineServer*);
	void (*ClientCommandKeyValues)(IVEngineServer*, edict_t* pEdict, KeyValues* pCommand);
	const CSteamID* (*GetClientSteamIDByPlayerIndex)(IVEngineServer*, int entnum);
	int (*GetClusterCount)(IVEngineServer*);
	int (*GetAllClusterBounds)(IVEngineServer*, bbox_t* pBBoxList, int maxBBox);
	edict_t* (*CreateFakeClientEx)(IVEngineServer*, const char* netname, bool bReportFakeClient);
	int (*GetServerVersion)(IVEngineServer*); // const
	float (*GetServerTime)(IVEngineServer*); // const
	IServer* (*GetIServer)(IVEngineServer*);
	bool (*IsPlayerNameLocked)(IVEngineServer*, const edict_t* pEdict);
	bool (*CanPlayerChangeName)(IVEngineServer*, const edict_t* pEdict);
	eFindMapResult (*FindMap)(IVEngineServer*, char* pMapName, int nMapNameMax);
	void (*SetPausedForced)(IVEngineServer*, bool bPaused, float flDuration);
} IVEngineServer_VMT;

struct IVEngineServer
{
	IVEngineServer_VMT* vmt;
};

#define IVEngineServer_REFL "IVEngineServer"
#define IVEngineServer_SIGNATURE "VEngineServer023"
#define IVEngineServer_MODULE "engine"

#endif
