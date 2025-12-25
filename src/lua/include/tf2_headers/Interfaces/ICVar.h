#if !defined(IntfICVar_Included)
#define IntfICVar_Included

#include "Misc/IAppSystem.h"
#include "Misc/ConVar.h"
#include "Types.h"
#include "Misc/CommonMacros.h"

typedef struct ConCommandBase;
typedef struct ConCommand;

typedef int CVarDLLIdentifier_t;

typedef struct IConsoleDisplayFunc IConsoleDisplayFunc;

typedef struct IConsoleDisplayFunc_VMT
{
	void (*ColorPrint)(IConsoleDisplayFunc*, const Color_t* clr, const char* pMessage);
	void (*Print)(IConsoleDisplayFunc*, const char* pMessage);
	void (*DPrint)(IConsoleDisplayFunc*, const char* pMessage);
} IConsoleDisplayFunc_VMT;

struct IConsoleDisplayFunc
{
	IConsoleDisplayFunc_VMT* vmt;
};

typedef struct ICvarQuery ICvarQuery;

typedef struct ICvarQuery_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	bool (*AreConVarsLinkable)(ICvarQuery*, const ConVar* child, const ConVar* parent);
} ICvarQuery_VMT;

struct ICvarQuery
{
	ICvarQuery_VMT* vmt;
};

typedef struct ICvar ICvar;

typedef struct ICvar_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	CVarDLLIdentifier_t (*AllocateDLLIdentifier)(ICvar*);
	void (*RegisterConCommand)(ICvar*, ConCommandBase* pCommandBase);
	void (*UnregisterConCommand)(ICvar*, ConCommandBase* pCommandBase);
	void (*UnregisterConCommands)(ICvar*, CVarDLLIdentifier_t id);
	const char* (*GetCommandLineValue)(ICvar*, const char* pVariableName);
	ConCommandBase* (*FindCommandBase)(ICvar*, const char* name);
	const ConCommandBase* (*FindCommandBaseConst)(ICvar*, const char* name);
	ConVar* (*FindVar)(ICvar*, const char* var_name);
	const ConVar* (*FindVarConst)(ICvar*, const char* var_name);
	ConCommand* (*FindCommand)(ICvar*, const char* name);
	const ConCommand* (*FindCommandConst)(ICvar*, const char* name);
	ConCommandBase* (*GetCommands)(ICvar*);
	const ConCommandBase* (*GetCommandsConst)(ICvar*);
	void (*InstallGlobalChangeCallback)(ICvar*, FnChangeCallback_t callback);
	void (*RemoveGlobalChangeCallback)(ICvar*, FnChangeCallback_t callback);
	void (*CallGlobalChangeCallbacks)(ICvar*, ConVar* var, const char* pOldString, float flOldValue);
	void (*InstallConsoleDisplayFunc)(ICvar*, IConsoleDisplayFunc* pDisplayFunc);
	void (*RemoveConsoleDisplayFunc)(ICvar*, IConsoleDisplayFunc* pDisplayFunc);
	void (*ConsoleColorPrintf)(ICvar*, const Color_t* clr, const char* pFormat, ...); // const
	void (*ConsolePrintf)(ICvar*, const char* pFormat, ...); // const
	void (*ConsoleDPrintf)(ICvar*, const char* pFormat, ...); // const
	void (*RevertFlaggedConVars)(ICvar*, int nFlag);
	void (*InstallCVarQuery)(ICvar*, ICvarQuery* pQuery);
	bool (*IsMaterialThreadSetAllowed)(ICvar*); // const
	void (*QueueMaterialThreadSetValueChar)(ICvar*, ConVar* pConVar, const char* pValue);
	void (*QueueMaterialThreadSetValueInt)(ICvar*, ConVar* pConVar, int nValue);
	void (*QueueMaterialThreadSetValueFloat)(ICvar*, ConVar* pConVar, float flValue);
	bool (*HasQueuedMaterialThreadConVarSets)(ICvar*); // const
	int (*ProcessQueuedMaterialThreadConVarSets)(ICvar*);

	//	class ICVarIteratorInternal
	//	{
	//	   public:
	//		virtual ~ICVarIteratorInternal() {}
	//		virtual void SetFirst(void) = 0;
	//		virtual void Next(void) = 0;
	//		virtual bool IsValid(void) = 0;
	//		virtual ConCommandBase* Get(void) = 0;
	//	};
	//
	//	class Iterator
	//	{
	//	   public:
	//		inline Iterator(ICvar* icvar);
	//		inline ~Iterator(void);
	//		inline void SetFirst(void);
	//		inline void Next(void);
	//		inline bool IsValid(void);
	//		inline ConCommandBase* Get(void);
	//
	//	   private:
	//		ICVarIteratorInternal* m_pIter;
	//	};
	//
	//	virtual ICVarIteratorInternal* FactoryInternalIterator(void) = 0;
} ICvar_VMT;

struct ICvar
{
	ICvar_VMT* vmt;
};

#define ICvar_REFL "ICvar"
#define ICvar_SIGNATURE "VEngineCvar004"
#define ICvar_MODULE VSTDLIB_DEFINE

#endif
