#if !defined(IntfIServerPlugin_Included)
#define IntfIServerPlugin_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Interfaces/Interface.h"
#include "Misc/Edict.h"
#include "Main/KeyValues.h"

typedef struct CCommand CCommand;

typedef enum
{
	PLUGIN_CONTINUE = 0,  // keep going
	PLUGIN_OVERRIDE,      // run the game dll function but use our return value instead
	PLUGIN_STOP,          // don't run the game dll function at all
} PLUGIN_RESULT;

typedef enum
{
	eQueryCvarValueStatusSPlugin_ValueIntact = 0,  // It got the value fine.
	eQueryCvarValueStatusSPlugin_CvarNotFound = 1,
	eQueryCvarValueStatusSPlugin_NotACvar = 2,      // There's a ConCommand, but it's not a ConVar.
	eQueryCvarValueStatusSPlugin_CvarProtected = 3  // The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
} EQueryCvarValueStatusSPlugin;

typedef int QueryCvarCookie_t;
#define InvalidQueryCvarCookie -1

#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_1 "ISERVERPLUGINCALLBACKS001"
#define INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_2 "ISERVERPLUGINCALLBACKS002"

typedef struct IServerPluginCallbacks IServerPluginCallbacks;

typedef struct IServerPluginCallbacks_VMT
{
	// Initialize the plugin to run
	// Return false if there is an error during startup.
	bool (*Load)(IServerPluginCallbacks*, CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);

	// Called when the plugin should be shutdown
	void (*Unload)(IServerPluginCallbacks*);

	// called when a plugins execution is stopped but the plugin is not unloaded
	void (*Pause)(IServerPluginCallbacks*);

	// called when a plugin should start executing again (sometime after a Pause() call)
	void (*UnPause)(IServerPluginCallbacks*);

	// Returns string describing current plugin.  e.g., Admin-Mod.
	const char* (*GetPluginDescription)(IServerPluginCallbacks*);

	// Called any time a new level is started (after GameInit() also on level transitions within a game)
	void (*LevelInit)(IServerPluginCallbacks*, char const* pMapName);

	// The server is about to activate
	void (*ServerActivate)(IServerPluginCallbacks*, edict_t* pEdictList, int edictCount, int clientMax);

	// The server should run physics/think on all edicts
	void (*GameFrame)(IServerPluginCallbacks*, bool simulating);

	// Called when a level is shutdown (including changing levels)
	void (*LevelShutdown)(IServerPluginCallbacks*);

	// Client is going active
	void (*ClientActive)(IServerPluginCallbacks*, edict_t* pEntity);

	// Client is disconnecting from server
	void (*ClientDisconnect)(IServerPluginCallbacks*, edict_t* pEntity);

	// Client is connected and should be put in the game
	void (*ClientPutInServer)(IServerPluginCallbacks*, edict_t* pEntity, char const* playername);

	// Sets the client index for the client who typed the command into their console
	void (*SetCommandClient)(IServerPluginCallbacks*, int index);

	// A player changed one/several replicated cvars (name etc)
	void (*ClientSettingsChanged)(IServerPluginCallbacks*, edict_t* pEdict);

	// Client is connecting to server ( set retVal to false to reject the connection )
	//	You can specify a rejection message by writing it into reject
	PLUGIN_RESULT (*ClientConnect)(IServerPluginCallbacks*, bool* bAllowConnect, edict_t* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen);

	// The client has typed a command at the console
	PLUGIN_RESULT (*ClientCommand)(IServerPluginCallbacks*, edict_t* pEntity, const CCommand* args);

	// A user has had their network id setup and validated
	PLUGIN_RESULT (*NetworkIDValidated)(IServerPluginCallbacks*, const char* pszUserName, const char* pszNetworkID);

	// This is called when a query from IServerPluginHelpers::StartQueryCvarValue is finished.
	// iCookie is the value returned by IServerPluginHelpers::StartQueryCvarValue.
	// Added with version 2 of the interface.
	void (*OnQueryCvarValueFinished)(IServerPluginCallbacks*, QueryCvarCookie_t iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatusSPlugin eStatus, const char* pCvarName, const char* pCvarValue);

	// added with version 3 of the interface.
	void (*OnEdictAllocated)(IServerPluginCallbacks*, edict_t* edict);
	void (*OnEdictFreed)(IServerPluginCallbacks*, const edict_t* edict);
} IServerPluginCallbacks_VMT;

struct IServerPluginCallbacks
{
	IServerPluginCallbacks_VMT* vmt;
};

#define IServerPluginCallbacks_REFL "IServerPluginCallbacks"
#define IServerPluginCallbacks_SIGNATURE "ISERVERPLUGINCALLBACKS003"
#define IServerPluginCallbacks_MODULE "engine"

typedef enum
{
	DIALOG_MSG = 0,    // just an on screen message
	DIALOG_MENU,       // an options menu
	DIALOG_TEXT,       // a richtext dialog
	DIALOG_ENTRY,      // an entry box
	DIALOG_ASKCONNECT  // Ask the client to connect to a specified IP address. Only the "time" and "title" keys are used.
} DIALOG_TYPE;

typedef struct IServerPluginHelpers IServerPluginHelpers;

typedef struct IServerPluginHelpers_VMT
{
	// creates an onscreen menu with various option buttons
	//	The keyvalues param can contain these fields:
	//	"title" - (string) the title to show in the hud and in the title bar
	//	"msg" - (string) a longer message shown in the GameUI
	//  "color" - (color) the color to display the message in the hud (white by default)
	//	"level" - (int) the priority of this message (closer to 0 is higher), only 1 message can be outstanding at a time
	//	"time" - (int) the time in seconds this message should stay active in the GameUI (min 10 sec, max 200 sec)
	//
	// For DIALOG_MENU add sub keys for each option with these fields:
	//  "command" - (string) client command to run if selected
	//  "msg" - (string) button text for this option
	//
	void (*CreateMessage)(IServerPluginHelpers*, edict_t* pEntity, DIALOG_TYPE type, KeyValues* data, IServerPluginCallbacks* plugin);
	void (*ClientCommand)(IServerPluginHelpers*, edict_t* pEntity, const char* cmd);

	// Call this to find out the value of a cvar on the client.
	//
	// It is an asynchronous query, and it will call IServerPluginCallbacks::OnQueryCvarValueFinished when
	// the value comes in from the client.
	//
	// Store the return value if you want to match this specific query to the OnQueryCvarValueFinished call.
	// Returns InvalidQueryCvarCookie if the entity is invalid.
	QueryCvarCookie_t (*StartQueryCvarValue)(IServerPluginHelpers*, edict_t* pEntity, const char* pName);
} IServerPluginHelpers_VMT;

struct IServerPluginHelpers
{
	IServerPluginHelpers_VMT* vmt;
};

#define IServerPluginHelpers_REFL "IServerPluginHelpers"
#define IServerPluginHelpers_SIGNATURE "ISERVERPLUGINHELPERS001"
#define IServerPluginHelpers_MODULE "engine"

#endif
