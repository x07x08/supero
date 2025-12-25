#if !defined(MiscConVar_Included)
#define MiscConVar_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Misc/IConVar.h"
#include "Main/UtlVector.h"
#include "Misc/CommonMacros.h"

typedef struct CCommand CCommand;
typedef struct ConCommand ConCommand;
typedef struct ConCommandBase ConCommandBase;
typedef struct ConCommandBaseInherited ConCommandBaseInherited;
typedef struct IConCommandBaseAccessor IConCommandBaseAccessor;
typedef struct ICommandCallback ICommandCallback;
typedef struct ICommandCompletionCallback ICommandCompletionCallback;
typedef struct ConVarRef ConVarRef;
typedef struct CCvar CCvar;
// struct characterset_t;

typedef struct ConVar ConVar;

typedef struct IConCommandBaseAccessor_VMT
{
	bool (*RegisterConCommandBase)(IConCommandBaseAccessor*, ConCommandBase* pVar);
} IConCommandBaseAccessor_VMT;

struct IConCommandBaseAccessor
{
	IConCommandBaseAccessor_VMT* vmt;
};

typedef void (*FnCommandCallbackVoid_t)(void);
typedef void (*FnCommandCallback_t)(const CCommand* command);

#define COMMAND_COMPLETION_MAXITEMS 64
#define COMMAND_COMPLETION_ITEM_LENGTH 64

typedef int (*FnCommandCompletionCallback)(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH]);

typedef struct ICommandCallback_VMT
{
	void (*CommandCallback)(ICommandCallback*, const CCommand* command);
} ICommandCallback_VMT;

struct ICommandCallback
{
	ICommandCallback_VMT* vmt;
};

typedef struct ICommandCompletionCallback_VMT
{
	int (*CommandCompletionCallback)(ICommandCompletionCallback*, const char* pPartial, CUtlVector* commands);
} ICommandCompletionCallback_VMT;

struct ICommandCompletionCallback
{
	ICommandCompletionCallback_VMT* vmt;
};

struct ConVarRef
{
	IConVar* m_pConVar;
	ConVar* m_pConVarState;
};

typedef struct ConCommandBase_VMT
{
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	bool (*IsCommand)(ConCommandBase*); // const
#if defined(__linux__)
	bool (*IsFlagSet)(ConCommandBase*, int flag); // const
#endif
	void (*AddFlags)(ConCommandBase*, int flags);
#if defined(__linux__)
	const char* (*GetName)(ConCommandBase*); // const
#endif
	const char* (*GetHelpText)(ConCommandBase*); // const
	bool (*IsRegistered)(ConCommandBase*); // const
	void* (*GetDLLIdentifier)(ConCommandBase*); // const
	void (*CreateBase)(ConCommandBase*, const char* pName, const char* pHelpString, int flags);
	void (*Init)(ConCommandBase*);
} ConCommandBase_VMT;

struct ConCommandBase
{
	ConCommandBase_VMT* vmt;

	ConCommandBase* m_pNext;
	bool m_bRegistered;
	const char* m_pszName;
	const char* m_pszHelpString;
	int m_nFlags;

	ConCommandBase* s_pConCommandBases;
	IConCommandBaseAccessor* s_pAccessor;
};

struct ConCommandBaseInherited
{
	ConCommandBase* m_pNext;
	bool m_bRegistered;
	const char* m_pszName;
	const char* m_pszHelpString;
	int m_nFlags;

	// ConCommandBase* s_pConCommandBases;
	// IConCommandBaseAccessor* s_pAccessor;
};

enum
{
	COMMAND_MAX_ARGC = 64,
	COMMAND_MAX_LENGTH = 512
};

struct CCommand
{
	int m_nArgc;
	int m_nArgv0Size;
	char m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];
};

typedef struct ConCommand_VMT
{
	ConCommandBase_VMT vmt_ConCommandBase;

	//bool (*IsCommand)(ConCommand*);
	int (*AutoCompleteSuggest)(ConCommand*, const char* partial, CUtlVector* commands);
	bool (*CanAutoComplete)(ConCommand*);
	void (*Dispatch)(ConCommand*, const CCommand* command);
} ConCommand_VMT;

struct ConCommand
{
	ConCommand_VMT* vmt;

	ConCommandBaseInherited m_ConCommandBase;

	union
	{
		FnCommandCallbackVoid_t m_fnCommandCallbackV1;
		FnCommandCallback_t m_fnCommandCallback;
		ICommandCallback* m_pCommandCallback;
	};

	union
	{
		FnCommandCompletionCallback m_fnCompletionCallback;
		ICommandCompletionCallback* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback;
	bool m_bUsingNewCommandCallback;
	bool m_bUsingCommandCallbackInterface;
};

typedef struct IConVar_VMT_CCMDBase
{
	void (*SetValueChar)(IConVar*, const char* pValue);
	void (*SetValueFloat)(IConVar*, float flValue);
	void (*SetValueInt)(IConVar*, int nValue);
} IConVar_VMT_CCMDBase;

typedef struct ConVar_VMT
{
	ConCommandBase_VMT vmt_ConCommandBase;
#if defined(__linux__)
	IConVar_VMT_CCMDBase vmt_IConVar;
#endif

	void (*InternalSetValue)(ConVar*, const char* value);
	void (*InternalSetFloatValue)(ConVar*, float fNewValue, bool bForce);
	void (*InternalSetIntValue)(ConVar*, int nValue);
	bool (*ClampValue)(ConVar*, float* value);
	void (*ChangeStringValue)(ConVar*, const char* tempVal, float flOldValue);
} ConVar_VMT;

struct ConVar
{
	ConVar_VMT* vmt;

	ConCommandBaseInherited m_ConCommandBase;

	char pad[4];

	ConVar* m_pParent;
	const char* m_pszDefaultValue;
	char* m_pszString;
	int m_StringLength;
	float m_fValue;
	int m_nValue; // 88
	bool m_bHasMin;
	float m_fMinVal;
	bool m_bHasMax;
	float m_fMaxVal;
	FnChangeCallback_t m_fnChangeCallback;
	bool m_bHasCompMin;
	float m_fCompMinVal;
	bool m_bHasCompMax;
	float m_fCompMaxVal;
	bool m_bCompetitiveRestrictions;
};

#endif
