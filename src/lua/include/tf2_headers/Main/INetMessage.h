#if !defined(MainINetMessage_Included)
#define MainINetMessage_Included

#include "Misc/BitBuf.h"
#include "Main/UtlVector.h"
#include "Types.h"
#include "Misc/CommonMacros.h"

#define MAX_OSPATH 260  // max length of a filesystem pathname

// shared commands used by all streams, handled by stream layer
#define net_NOP 0          // nop command used for padding
#define net_Disconnect 1   // disconnect, last message in connection
#define net_File 2         // file transmission message request/deny
#define net_Tick 3         // send last world tick
#define net_StringCmd 4    // a string command
#define net_SetConVar 5    // sends one/multiple convar settings
#define net_SignonState 6  // signals current signon state
// server to client
#define svc_Print 7               // print text to console
#define svc_ServerInfo 8          // first message from server about game, map etc
#define svc_SendTable 9           // sends a sendtable description for a game class
#define svc_ClassInfo 10          // Info about classes (first byte is a CLASSINFO_ define).
#define svc_SetPause 11           // tells client if server paused or unpaused
#define svc_CreateStringTable 12  // inits shared string tables
#define svc_UpdateStringTable 13  // updates a string table
#define svc_VoiceInit 14          // inits used voice codecs & quality
#define svc_VoiceData 15          // Voicestream data from the server
//#define svc_HLTV 16               // HLTV control messages
#define svc_Sounds 17             // starts playing sound
#define svc_SetView 18            // sets entity as point of view
#define svc_FixAngle 19           // sets/corrects players viewangle
#define svc_CrosshairAngle 20     // adjusts crosshair in auto aim mode to lock on traget
#define svc_BSPDecal 21           // add a static decal to the worl BSP
//#define svc_TerrainMod 22         // modification to the terrain/displacement. NOTE: This is now unused!
//  Message from server side to client side entity
#define svc_UserMessage 23     // a game specific message
#define svc_EntityMessage 24   // a message for an entity
#define svc_GameEvent 25       // global game event fired
#define svc_PacketEntities 26  // non-delta compressed entities
#define svc_TempEntities 27    // non-reliable event object
#define svc_Prefetch 28        // only sound indices for now
#define svc_Menu 29            // display a menu from a plugin
#define svc_GameEventList 30   // list of known games events and fields
#define svc_GetCvarValue 31    // Server wants to know the value of a cvar on the client
#define svc_CmdKeyValues 32    // Server submits KeyValues command for the client
#define svc_SetPauseTimed 33   // Timed pause - to avoid breaking demos
#define SVC_LASTMSG 33         // last known server messages
// client to server
#define clc_ClientInfo 8         // client info (table CRC etc)
#define clc_Move 9               // [CUserCmd]
#define clc_VoiceData 10         // Voicestream data from a client
#define clc_BaselineAck 11       // client acknowledges a new baseline seqnr
#define clc_ListenEvents 12      // client acknowledges a new baseline seqnr
#define clc_RespondCvarValue 13  // client is responding to a svc_GetCvarValue message.
#define clc_FileCRCCheck 14      // client is sending a file's CRC to the server to be verified.
#define clc_SaveReplay 15        // client is sending a save replay request to the server.
#define clc_CmdKeyValues 16
#define clc_FileMD5Check 17          // client is sending a file's MD5 to the server to be verified.
#define CLC_LASTMSG 17               //	last known client message
#define RES_FATALIFMISSING (1 << 0)  // Disconnect if we can't get this file.
#define RES_PRELOAD (1 << 1)         // Load on client rather than just reserving name
#define SIGNONSTATE_NONE 0           // no state yet, about to connect
#define SIGNONSTATE_CHALLENGE 1      // client challenging server, all OOB packets
#define SIGNONSTATE_CONNECTED 2      // client is connected to server, netchans ready
#define SIGNONSTATE_NEW 3            // just got serverinfo and string tables
#define SIGNONSTATE_PRESPAWN 4       // received signon buffers
#define SIGNONSTATE_SPAWN 5          // ready to receive entity packets
#define SIGNONSTATE_FULL 6           // we are fully connected, first non-delta packet received
#define SIGNONSTATE_CHANGELEVEL 7    // server is changing level, please wait
// matchmaking
#define mm_Heartbeat 16         // send a mm_Heartbeat
#define mm_ClientInfo 17        // information about a player
#define mm_JoinResponse 18      // response to a matchmaking join request
#define mm_RegisterResponse 19  // response to a matchmaking join request
#define mm_Migrate 20           // tell a client to migrate
#define mm_Mutelist 21          // send mutelist info to other clients
#define mm_Checkpoint 22        // game state checkpoints (start, connect, etc)
#define MM_LASTMSG 22           // last known matchmaking message

typedef struct INetChannel INetChannel;

enum
{
	GENERIC = 0,   // must be first and is default group
	LOCALPLAYER,   // bytes for local player entity update
	OTHERPLAYERS,  // bytes for other players update
	ENTITIES,      // all other entity bytes
	SOUNDS,        // game sounds
	EVENTS,        // event messages
	USERMESSAGES,  // user messages
	ENTMESSAGES,   // entity messages
	VOICE,         // voice data
	STRINGTABLE,   // a stringtable update
	MOVE,          // client move cmds
	STRINGCMD,     // string command
	SIGNON,        // various signondata
	TOTAL,         // must be last and is not a real group
};

typedef struct INetMessage INetMessage;

typedef struct INetMessage_VMT
{
	//virtual ~INetMessage() {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*SetNetChannel)(INetMessage*, INetChannel* netchan);
	void (*SetReliable)(INetMessage*, bool state);
	bool (*Process)(INetMessage*);
	bool (*ReadFromBuffer)(INetMessage*, bf_read* buffer);
	bool (*WriteToBuffer)(INetMessage*, bf_write* buffer);
	bool (*IsReliable)(INetMessage*); // const
	int (*GetType)(INetMessage*); // const
	int (*GetGroup)(INetMessage*); // const
	const char* (*GetName)(INetMessage*); // const
	INetChannel* (*GetNetChannel)(INetMessage*); // const
	const char* (*ToString)(INetMessage*); // const
} INetMessage_VMT;

struct INetMessage
{
	INetMessage_VMT* vmt;
};

typedef INetMessage_VMT CNetMessage_VMT;

// All protected, still inherited

typedef struct CNetMessage
{
	//virtual ~CNetMessage() {};
	CNetMessage_VMT* vmt;

	bool m_bReliable;
	INetChannel* m_NetChannel;
	byte pad0[8];
} CNetMessage;

typedef struct CNetMessageInherited
{
	bool m_bReliable;
	INetChannel* m_NetChannel;
	byte pad0[8];
} CNetMessageInherited;

typedef struct CLC_Move// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	int m_nBackupCommands;
	int m_nNewCommands;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
} CLC_Move;

typedef struct CVar_s
{
	char Name[MAX_OSPATH];
	char Value[MAX_OSPATH];
} CVar_t;

typedef struct NET_SetConVar// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	CUtlVector m_ConVars; // CVar_t
} NET_SetConVar;

typedef struct NET_SignonState// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	int m_nSignonState;
	int m_nSpawnCount;
} NET_SignonState;

typedef struct NET_Tick// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	int m_nTick;
	float m_flHostFrameTime;
	float m_flHostFrameTimeStdDeviation;
} NET_Tick;

typedef struct CLC_VoiceData// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
	uint64_t m_xuid;
} CLC_VoiceData;

typedef struct CLC_RespondCvarValue// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;
	int m_iCookie;
	const char* m_szCvarName;
	const char* m_szCvarValue;
	int m_eStatusCode;

	char m_szCvarNameBuffer[256];
	char m_szCvarValueBuffer[256];
} CLC_RespondCvarValue;

typedef struct SVC_FixAngle// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	byte pad1[8];
	bool m_bRelative;
	QAngle m_Angle;
} SVC_FixAngle;

typedef struct CLC_BaselineAck// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	void* m_pMessageHandler;

	int m_nBaselineTick;
	int m_nBaselineNr;
} CLC_BaselineAck;

typedef struct SVC_PacketEntities// : public CNetMessage
{
	CNetMessage_VMT* vmt;

	CNetMessageInherited m_CNetMessage;

	byte pad1[8];
	int m_nMaxEntries;
	int m_nUpdatedEntries;
	bool m_bIsDelta;
	bool m_bUpdateBaseline;
	int m_nBaseline;
	int m_nDeltaFrom;
	int m_nLength;
	bf_read m_DataIn;
	bf_write m_DataOut;
} SVC_PacketEntities;

#endif
