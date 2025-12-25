#if !defined(IntfIBaseClient_Included)
#define IntfIBaseClient_Included

#include "Interfaces/Interface.h"
#include "Main/UtlVector.h"
#include "Types.h"
#include "Misc/BitBuf.h"
#include "Misc/ButtonCode.h"
#include "Misc/ChecksumCRC.h"
#include "Misc/ClientClass.h"
#include "Misc/CViewSetup.h"
#include "Misc/Datamap.h"
#include "Misc/Modes.h"
#include "Misc/IConVar.h"

// typedef struct cmodel_t cmodel_t;
typedef struct ScreenFade_t ScreenFade_t;
typedef struct ScreenShake_t ScreenShake_t;
typedef struct CEngineSprite CEngineSprite;
typedef struct CGlobalVarsBase CGlobalVarsBase;
typedef struct CSaveRestoreData CSaveRestoreData;
typedef struct IFileList IFileList;
typedef struct CRenamedRecvTableInfo CRenamedRecvTableInfo;
typedef struct CMouthInfo CMouthInfo;

typedef enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
} ClientFrameStage_t;

typedef struct IBaseClientDLL IBaseClientDLL;

typedef struct IBaseClientDLL_VMT
{
	int (*Init)(IBaseClientDLL*, CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase* pGlobals);
	void (*PostInit)(IBaseClientDLL*);
	void (*Shutdown)(IBaseClientDLL*);
	bool (*ReplayInit)(IBaseClientDLL*, CreateInterfaceFn replayFactory);
	bool (*ReplayPostInit)(IBaseClientDLL*);
	void (*LevelInitPreEntity)(IBaseClientDLL*, char const* pMapName);
	void (*LevelInitPostEntity)(IBaseClientDLL*);
	void (*LevelShutdown)(IBaseClientDLL*);
	ClientClass* (*GetAllClasses)(IBaseClientDLL*);
	int (*HudVidInit)(IBaseClientDLL*);
	void (*HudProcessInput)(IBaseClientDLL*, bool bActive);
	void (*HudUpdate)(IBaseClientDLL*, bool bActive);
	void (*HudReset)(IBaseClientDLL*);
	void (*HudText)(IBaseClientDLL*, const char* message);
	void (*IN_ActivateMouse)(IBaseClientDLL*);
	void (*IN_DeactivateMouse)(IBaseClientDLL*);
	void (*IN_Accumulate)(IBaseClientDLL*);
	void (*IN_ClearStates)(IBaseClientDLL*);
	bool (*IN_IsKeyDown)(IBaseClientDLL*, const char* name, bool* isdown);
	void (*IN_OnMouseWheeled)(IBaseClientDLL*, int nDelta);
	int (*IN_KeyEvent)(IBaseClientDLL*, int eventcode, ButtonCode_t keynum, const char* pszCurrentBinding);
	void (*CreateMove)(IBaseClientDLL*, int sequence_number, float input_sample_frametime, bool active);
	void (*ExtraMouseSample)(IBaseClientDLL*, float frametime, bool active);
	bool (*WriteUsercmdDeltaToBuffer)(IBaseClientDLL*, bf_write* buf, int from, int to, bool isnewcommand);
	void (*EncodeUserCmdToBuffer)(IBaseClientDLL*, bf_write* buf, int slot);
	void (*DecodeUserCmdFromBuffer)(IBaseClientDLL*, bf_read* buf, int slot);
	void (*View_Render)(IBaseClientDLL*, vrect_t* rect);
	void (*RenderView)(IBaseClientDLL*, const CViewSetup* view, int nClearFlags, int whatToDraw);
	void (*View_Fade)(IBaseClientDLL*, ScreenFade_t* pSF);
	void (*SetCrosshairAngle)(IBaseClientDLL*, const QAngle* angle);
	void (*InitSprite)(IBaseClientDLL*, CEngineSprite* pSprite, const char* loadname);
	void (*ShutdownSprite)(IBaseClientDLL*, CEngineSprite* pSprite);
	int (*GetSpriteSize)(IBaseClientDLL*); // const
	void (*VoiceStatus)(IBaseClientDLL*, int entindex, qboolean bTalking);
	void (*InstallStringTableCallback)(IBaseClientDLL*, char const* tableName);
	void (*FrameStageNotify)(IBaseClientDLL*, ClientFrameStage_t curStage);
	bool (*DispatchUserMessage)(IBaseClientDLL*, int msg_type, bf_read* msg_data);
	CSaveRestoreData* (*SaveInit)(IBaseClientDLL*, int size);
	void (*SaveWriteFields)(IBaseClientDLL*, CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	void (*SaveReadFields)(IBaseClientDLL*, CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, int);
	void (*PreSave)(IBaseClientDLL*, CSaveRestoreData*);
	void (*Save)(IBaseClientDLL*, CSaveRestoreData*);
	void (*WriteSaveHeaders)(IBaseClientDLL*, CSaveRestoreData*);
	void (*ReadRestoreHeaders)(IBaseClientDLL*, CSaveRestoreData*);
	void (*Restore)(IBaseClientDLL*, CSaveRestoreData*, bool);
	void (*DispatchOnRestore)(IBaseClientDLL*);
	CStandardRecvProxies* (*GetStandardRecvProxies)(IBaseClientDLL*);
	void (*WriteSaveGameScreenshot)(IBaseClientDLL*, const char* pFilename);
	void (*EmitSentenceCloseCaption)(IBaseClientDLL*, char const* tokenstream);
	void (*EmitCloseCaption)(IBaseClientDLL*, char const* captionname, float duration);
	bool (*CanRecordDemo)(IBaseClientDLL*, char* errorMsg, int length); // const
	void (*OnDemoRecordStart)(IBaseClientDLL*, char const* pDemoBaseName);
	void (*OnDemoRecordStop)(IBaseClientDLL*);
	void (*OnDemoPlaybackStart)(IBaseClientDLL*, char const* pDemoBaseName);
	void (*OnDemoPlaybackStop)(IBaseClientDLL*);
	bool (*ShouldDrawDropdownConsole)(IBaseClientDLL*);
	int (*GetScreenWidth)(IBaseClientDLL*);
	int (*GetScreenHeight)(IBaseClientDLL*);
	void (*WriteSaveGameScreenshotOfSize)(IBaseClientDLL*, const char* pFilename, int width, int height, bool bCreatePowerOf2Padded, bool bWriteVTF);
	bool (*GetPlayerView)(IBaseClientDLL*, CViewSetup* playerView);
	void (*SetupGameProperties)(IBaseClientDLL*, CUtlVector* contexts, CUtlVector* properties);
	uint (*GetPresenceID)(IBaseClientDLL*, const char* pIDName);
	const char* (*GetPropertyIdString)(IBaseClientDLL*, const uint id);
	void (*GetPropertyDisplayString)(IBaseClientDLL*, uint id, uint value, char* pOutput, int nBytes);
	void (*StartStatsReporting)(IBaseClientDLL*, HANDLE handle, bool bArbitrated);
	void (*InvalidateMdlCache)(IBaseClientDLL*);
	void (*IN_SetSampleTime)(IBaseClientDLL*, float frametime);
	void (*ReloadFilesInList)(IBaseClientDLL*, IFileList* pFilesToReload);
	bool (*HandleUiToggle)(IBaseClientDLL*);
	bool (*ShouldAllowConsole)(IBaseClientDLL*);
	CRenamedRecvTableInfo* (*GetRenamedRecvTableInfos)(IBaseClientDLL*);
	CMouthInfo* (*GetClientUIMouthInfo)(IBaseClientDLL*);
	void (*FileReceived)(IBaseClientDLL*, const char* fileName, unsigned int transferID);
	const char* (*TranslateEffectForVisionFilter)(IBaseClientDLL*, const char* pchEffectType, const char* pchEffectName);
	void (*ClientAdjustStartSoundParams)(IBaseClientDLL*, struct StartSoundParams_t* params);
	bool (*DisconnectAttempt)(IBaseClientDLL*);
	bool (*IsConnectedUserInfoChangeAllowed)(IBaseClientDLL*, IConVar* pCvar);
} IBaseClientDLL_VMT;

struct IBaseClientDLL
{
	IBaseClientDLL_VMT* vmt;
};

#define IBaseClientDLL_REFL "IBaseClientDLL"
#define IBaseClientDLL_SIGNATURE "VClient017"
#define IBaseClientDLL_MODULE "client"

#endif
