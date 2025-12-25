#if !defined(IntfCGlobalVarsBase_Included)
#define IntfCGlobalVarsBase_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

typedef struct CSaveRestoreData CSaveRestoreData;

typedef struct CGlobalVarsBase
{
	float realtime;
	int framecount;
	float absoluteframetime;
	float curtime;
	float frametime;
	int maxClients;
	int tickcount;
	float interval_per_tick;
	float interpolation_amount;
	int simTicksThisFrame;
	int network_protocol;
	CSaveRestoreData* pSaveData;
	bool m_bClient;
	int nTimestampNetworkingBase;
	int nTimestampRandomizeWindow;
} CGlobalVarsBase;

#define CGlobalVarsBase_REFL "CGlobalVarsBase"

// String : "Client.dll Init() in library client failed"
// A reference is used as an argument in the function, XREF it and find a no arguments function
// The signature is not unique, use multiple ones or an offset

#if (defined(__linux__) || defined(__unix__))
#define CGlobalVarsBase_SIGNATURE "48 8D 05 ? ? ? ? C3 ? ? ? ? ? ? ? ? 48 8D 05 ? ? ? ? 48 8B 38 48 8B 07 FF 60 ? 48 8D 05 ? ? ? ? 48 8B 38 48 8B 07 FF A0 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?"
#elif (defined(_WIN32) || defined(_WIN64))
#define CGlobalVarsBase_SIGNATURE "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B CA"
#endif
#define CGlobalVarsBase_MODULE "engine"

#endif
