#if !defined(MiscIAppSystem_Included)
#define MiscIAppSystem_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Interfaces/Interface.h"

typedef enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,
	INIT_LAST_VAL,
} InitReturnVal_t;

typedef struct IAppSystem IAppSystem;

typedef struct IAppSystem_VMT
{
	bool (*Connect)(IAppSystem*, CreateInterfaceFn factory);
	void (*Disconnect)(IAppSystem*);
	void* (*QueryInterface)(IAppSystem*, const char* pInterfaceName);
	InitReturnVal_t (*Init)(IAppSystem*);
	void (*Shutdown)(IAppSystem*);
} IAppSystem_VMT;

struct IAppSystem
{
	IAppSystem_VMT* vmt;
};

#endif
