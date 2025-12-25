#if !defined(IntfIVEngineClient_Included)
#define IntfIVEngineClient_Included

#include "Interfaces/Interface.h"
#include "Main/UtlVector.h"
#include "Misc/BaseTypes.h"
#include "Misc/BitBuf.h"
#include "Misc/ButtonCode.h"
#include "Misc/ChecksumCRC.h"
#include "Misc/ClientClass.h"
#include "Misc/CViewSetup.h"
#include "Misc/Datamap.h"
#include "Misc/Modes.h"
#include "Definitions.h"

typedef struct model_t model_t;
typedef struct CSentence CSentence;
typedef struct IMaterial IMaterial;
// typedef struct CAudioSource CAudioSource;
typedef struct SurfInfo SurfInfo;
typedef struct ISpatialQuery ISpatialQuery;
typedef struct IMaterialSystem IMaterialSystem;
typedef struct CPhysCollide CPhysCollide;
typedef struct INetChannelInfo INetChannelInfo;
typedef struct client_textmessage_t client_textmessage_t;
typedef struct IAchievementMgr IAchievementMgr;
typedef struct CGamestatsData CGamestatsData;
typedef struct KeyValues KeyValues;

typedef struct player_info_s_client
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
} player_info_t;

typedef struct AudioState_t
{
	Vector m_Origin;
	QAngle m_Angles;
	bool m_bIsUnderwater;
} AudioState_t;

typedef enum SkyboxVisibility_t
{
	SKYBOX_NOT_VISIBLE = 0,
	SKYBOX_3DSKYBOX_VISIBLE,
	SKYBOX_2DSKYBOX_VISIBLE,
} SkyboxVisibility_t;

typedef struct OcclusionParams_t
{
	float m_flMaxOccludeeArea;
	float m_flMinOccluderArea;
} OcclusionParams_t;

typedef struct IVEngineClient IVEngineClient;

typedef struct IVEngineClient_VMT
{
	int (*GetIntersectingSurfaces)(IVEngineClient*, const model_t* model, const Vector* vCenter, const float radius, const bool bOnlyVisibleSurfaces, SurfInfo* pInfos, const int nMaxInfos);
	Vector (*GetLightForPoint)(IVEngineClient*, const Vector* pos, bool bClamp);
	IMaterial* (*TraceLineMaterialAndLighting)(IVEngineClient*, const Vector* start, const Vector* end, Vector* diffuseLightColor, Vector* baseColor);
	const char* (*ParseFile)(IVEngineClient*, const char* data, char* token, int maxlen);
	bool (*CopyLocalFile)(IVEngineClient*, const char* source, const char* destination);
	void (*GetScreenSize)(IVEngineClient*, int* width, int* height);
	void (*ServerCmd)(IVEngineClient*, const char* szCmdString, bool bReliable);
	void (*ClientCmd)(IVEngineClient*, const char* szCmdString);
	bool (*GetPlayerInfo)(IVEngineClient*, int ent_num, player_info_t* pinfo);
	int (*GetPlayerForUserID)(IVEngineClient*, int userID);
	client_textmessage_t* (*TextMessageGet)(IVEngineClient*, const char* pName);
	bool (*Con_IsVisible)(IVEngineClient*);
	int (*GetLocalPlayer)(IVEngineClient*);
	const model_t* (*LoadModel)(IVEngineClient*, const char* pName, bool bProp);
	float (*Time)(IVEngineClient*);
	float (*GetLastTimeStamp)(IVEngineClient*);
	CSentence* (*GetSentence)(IVEngineClient*, CAudioSource* pAudioSource);
	float (*GetSentenceLength)(IVEngineClient*, CAudioSource* pAudioSource);
	bool (*IsStreaming)(IVEngineClient*, CAudioSource* pAudioSource); // const
	void (*GetViewAngles)(IVEngineClient*, QAngle* va);
	void (*SetViewAngles)(IVEngineClient*, QAngle* va);
	int (*GetMaxClients)(IVEngineClient*);
	const char* (*Key_LookupBinding)(IVEngineClient*, const char* pBinding);
	const char* (*Key_BindingForKey)(IVEngineClient*, ButtonCode_t code);
	void (*StartKeyTrapMode)(IVEngineClient*);
	bool (*CheckDoneKeyTrapping)(IVEngineClient*, ButtonCode_t* code);
	bool (*IsInGame)(IVEngineClient*);
	bool (*IsConnected)(IVEngineClient*);
	bool (*IsDrawingLoadingImage)(IVEngineClient*);
	void (*Con_NPrintf)(IVEngineClient*, int pos, const char* fmt, ...);
	void (*Con_NXPrintf)(IVEngineClient*, const struct con_nprint_s* info, const char* fmt, ...);
	int (*IsBoxVisible)(IVEngineClient*, const Vector* mins, const Vector* maxs);
	int (*IsBoxInViewCluster)(IVEngineClient*, const Vector* mins, const Vector* maxs);
	bool (*CullBox)(IVEngineClient*, const Vector* mins, const Vector* maxs);
	void (*Sound_ExtraUpdate)(IVEngineClient*);
	const char* (*GetGameDirectory)(IVEngineClient*);
	const VMatrix* (*WorldToScreenMatrix)(IVEngineClient*);
	const VMatrix* (*WorldToViewMatrix)(IVEngineClient*);
	int (*GameLumpVersion)(IVEngineClient*, int lumpId); // const
	int (*GameLumpSize)(IVEngineClient*, int lumpId); // const
	bool (*LoadGameLump)(IVEngineClient*, int lumpId, void* pBuffer, int size);
	int (*LevelLeafCount)(IVEngineClient*); // const
	ISpatialQuery* (*GetBSPTreeQuery)(IVEngineClient*);
	void (*LinearToGamma)(IVEngineClient*, float* linear, float* gamma);
	float (*LightStyleValue)(IVEngineClient*, int style);
	void (*ComputeDynamicLighting)(IVEngineClient*, const Vector* pt, const Vector* pNormal, Vector* color);
	void (*GetAmbientLightColor)(IVEngineClient*, Vector* color);
	int (*GetDXSupportLevel)(IVEngineClient*);
	bool (*SupportsHDR)(IVEngineClient*);
	void (*Mat_Stub)(IVEngineClient*, IMaterialSystem* pMatSys);
	void (*GetChapterName)(IVEngineClient*, char* pchBuff, int iMaxLength);
	char const* (*GetLevelName)(IVEngineClient*);
	int (*GetLevelVersion)(IVEngineClient*);
	struct IVoiceTweak_s* (*GetVoiceTweakAPI)(IVEngineClient*);
	void (*EngineStats_BeginFrame)(IVEngineClient*);
	void (*EngineStats_EndFrame)(IVEngineClient*);
	void (*FireEvents)(IVEngineClient*);
	int (*GetLeavesArea)(IVEngineClient*, int* pLeaves, int nLeaves);
	bool (*DoesBoxTouchAreaFrustum)(IVEngineClient*, const Vector* mins, const Vector* maxs, int iArea);
	void (*SetAudioState)(IVEngineClient*, const AudioState_t* state);
	int (*SentenceGroupPick)(IVEngineClient*, int groupIndex, char* name, int nameBufLen);
	int (*SentenceGroupPickSequential)(IVEngineClient*, int groupIndex, char* name, int nameBufLen, int sentenceIndex, int reset);
	int (*SentenceIndexFromName)(IVEngineClient*, const char* pSentenceName);
	const char* (*SentenceNameFromIndex)(IVEngineClient*, int sentenceIndex);
	int (*SentenceGroupIndexFromName)(IVEngineClient*, const char* pGroupName);
	const char* (*SentenceGroupNameFromIndex)(IVEngineClient*, int groupIndex);
	float (*SentenceLength)(IVEngineClient*, int sentenceIndex);
	void (*ComputeLighting)(IVEngineClient*, const Vector* pt, const Vector* pNormal, bool bClamp, Vector* color, Vector* pBoxColors);
	void (*ActivateOccluder)(IVEngineClient*, int nOccluderIndex, bool bActive);
	bool (*IsOccluded)(IVEngineClient*, const Vector* vecAbsMins, const Vector* vecAbsMaxs);
	void* (*SaveAllocMemory)(IVEngineClient*, size_t num, size_t size);
	void (*SaveFreeMemory)(IVEngineClient*, void* pSaveMem);
	INetChannelInfo* (*GetNetChannelInfo)(IVEngineClient*);
	void (*DebugDrawPhysCollide)(IVEngineClient*, const CPhysCollide* pCollide, IMaterial* pMaterial, matrix3x4* transform, const color32* color);
	void (*CheckPoint)(IVEngineClient*, const char* pName);
	void (*DrawPortals)(IVEngineClient*);
	bool (*IsPlayingDemo)(IVEngineClient*);
	bool (*IsRecordingDemo)(IVEngineClient*);
	bool (*IsPlayingTimeDemo)(IVEngineClient*);
	int (*GetDemoRecordingTick)(IVEngineClient*);
	int (*GetDemoPlaybackTick)(IVEngineClient*);
	int (*GetDemoPlaybackStartTick)(IVEngineClient*);
	float (*GetDemoPlaybackTimeScale)(IVEngineClient*);
	int (*GetDemoPlaybackTotalTicks)(IVEngineClient*);
	bool (*IsPaused)(IVEngineClient*);
	bool (*IsTakingScreenshot)(IVEngineClient*);
	bool (*IsHLTV)(IVEngineClient*);
	bool (*IsLevelMainMenuBackground)(IVEngineClient*);
	void (*GetMainMenuBackgroundName)(IVEngineClient*, char* dest, int destlen);
	void (*GetVideoModes)(IVEngineClient*, int* nCount, vmode_s** pModes);
	void (*SetOcclusionParameters)(IVEngineClient*, const OcclusionParams_t* params);
	void (*GetUILanguage)(IVEngineClient*, char* dest, int destlen);
	SkyboxVisibility_t (*IsSkyboxVisibleFromPoint)(IVEngineClient*, const Vector* vecPoint);
	const char* (*GetMapEntitiesString)(IVEngineClient*);
	bool (*IsInEditMode)(IVEngineClient*);
	float (*GetScreenAspectRatio)(IVEngineClient*);
	bool (*REMOVED_SteamRefreshLogin)(IVEngineClient*, const char* password, bool isSecure);
	bool (*REMOVED_SteamProcessCall)(IVEngineClient*, bool* finished);
	unsigned int (*GetEngineBuildNumber)(IVEngineClient*);
	const char* (*GetProductVersionString)(IVEngineClient*);
	void (*GrabPreColorCorrectedFrame)(IVEngineClient*, int x, int y, int width, int height);
	bool (*IsHammerRunning)(IVEngineClient*); // const
	void (*ExecuteClientCmd)(IVEngineClient*, const char* szCmdString);
	bool (*MapHasHDRLighting)(IVEngineClient*);
	int (*GetAppID)(IVEngineClient*);
	Vector (*GetLightForPointFast)(IVEngineClient*, const Vector* pos, bool bClamp);
	void (*ClientCmd_Unrestricted)(IVEngineClient*, const char* szCmdString);
	void (*SetRestrictServerCommands)(IVEngineClient*, bool bRestrict);
	void (*SetRestrictClientCommands)(IVEngineClient*, bool bRestrict);
	void (*SetOverlayBindProxy)(IVEngineClient*, int iOverlayID, void* pBindProxy);
	bool (*CopyFrameBufferToMaterial)(IVEngineClient*, const char* pMaterialName);
	void (*ChangeTeam)(IVEngineClient*, const char* pTeamName);
	void (*ReadConfiguration)(IVEngineClient*, const bool readDefault);
	void (*SetAchievementMgr)(IVEngineClient*, IAchievementMgr* pAchievementMgr);
	IAchievementMgr* (*GetAchievementMgr)(IVEngineClient*);
	bool (*MapLoadFailed)(IVEngineClient*);
	void (*SetMapLoadFailed)(IVEngineClient*, bool bState);
	bool (*IsLowViolence)(IVEngineClient*);
	const char* (*GetMostRecentSaveGame)(IVEngineClient*);
	void (*SetMostRecentSaveGame)(IVEngineClient*, const char* lpszFilename);
	void (*StartXboxExitingProcess)(IVEngineClient*);
	bool (*IsSaveInProgress)(IVEngineClient*);
	uint (*OnStorageDeviceAttached)(IVEngineClient*);
	void (*OnStorageDeviceDetached)(IVEngineClient*);
	void (*ResetDemoInterpolation)(IVEngineClient*);
	void (*SetGamestatsData)(IVEngineClient*, CGamestatsData* pGamestatsData);
	CGamestatsData* (*GetGamestatsData)(IVEngineClient*);
	void (*ServerCmdKeyValues)(IVEngineClient*, KeyValues* pKeyValues);
	bool (*IsSkippingPlayback)(IVEngineClient*);
	bool (*IsLoadingDemo)(IVEngineClient*);
	bool (*IsPlayingDemoALocallyRecordedDemo)(IVEngineClient*);
	const char* (*Key_LookupBindingExact)(IVEngineClient*, const char* pBinding);
	void (*AddPhonemeFile)(IVEngineClient*, const char* pszPhonemeFile);
	float (*GetPausedExpireTime)(IVEngineClient*);
	bool (*StartDemoRecording)(IVEngineClient*, const char* pszFilename, const char* pszFolder);
	void (*StopDemoRecording)(IVEngineClient*);
	void (*TakeScreenshot)(IVEngineClient*, const char* pszFilename, const char* pszFolder);
} IVEngineClient_VMT;

struct IVEngineClient
{
	IVEngineClient_VMT* vmt;
};

#define IVEngineClient_REFL "IVEngineClient"
#define IVEngineClient_SIGNATURE "VEngineClient014"
#define IVEngineClient_MODULE "engine"

#endif
