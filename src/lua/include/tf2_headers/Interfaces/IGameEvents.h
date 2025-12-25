#if !defined(IntfIGameEvents_Included)
#define IntfIGameEvents_Included

#include "Interfaces/Interface.h"
#include "Misc/BitBuf.h"
#include "Misc/CommonMacros.h"

#define MAX_EVENT_NAME_LENGTH 32
#define MAX_EVENT_BITS 9
#define MAX_EVENT_NUMBER (1 << MAX_EVENT_BITS)
#define MAX_EVENT_BYTES 1024

typedef struct KeyValues KeyValues;
typedef struct CGameEvent CGameEvent;

typedef struct IGameEvent IGameEvent;

typedef struct IGameEvent_VMT
{
	//virtual ~IGameEvent() {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	const char* (*GetName)(IGameEvent*); // const
	bool (*IsReliable)(IGameEvent*); // const
	bool (*IsLocal)(IGameEvent*); // const
	bool (*IsEmpty)(IGameEvent*, const char* keyName);
	bool (*GetBool)(IGameEvent*, const char* keyName, bool defaultValue);
	int (*GetInt)(IGameEvent*, const char* keyName, int defaultValue);
	float (*GetFloat)(IGameEvent*, const char* keyName, float defaultValue);
	const char* (*GetString)(IGameEvent*, const char* keyName, const char* defaultValue);
	void (*SetBool)(IGameEvent*, const char* keyName, bool value);
	void (*SetInt)(IGameEvent*, const char* keyName, int value);
	void (*SetFloat)(IGameEvent*, const char* keyName, float value);
	void (*SetString)(IGameEvent*, const char* keyName, const char* value);
} IGameEvent_VMT;

struct IGameEvent
{
	IGameEvent_VMT* vmt;
};

typedef struct IGameEventListener2 IGameEventListener2;

typedef struct IGameEventListener2_VMT
{
	//virtual ~IGameEventListener2(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*FireGameEvent)(IGameEventListener2*, IGameEvent* event);
} IGameEventListener2_VMT;

struct IGameEventListener2
{
	IGameEventListener2_VMT* vmt;
};

typedef struct IGameEventManager2 IGameEventManager2;

typedef struct IGameEventManager2_VMT
{
	//virtual ~IGameEventManager2(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	int (*LoadEventsFromFile)(IGameEventManager2*, const char* filename);
	void (*Reset)(IGameEventManager2*);
	bool (*AddListener)(IGameEventManager2*, IGameEventListener2* listener, const char* name, bool bServerSide);
	bool (*FindListener)(IGameEventManager2*, IGameEventListener2* listener, const char* name);
	void (*RemoveListener)(IGameEventManager2*, IGameEventListener2* listener);
	IGameEvent* (*CreateEvent)(IGameEventManager2*, const char* name, bool bForce);
	bool (*FireEvent)(IGameEventManager2*, IGameEvent* event, bool bDontBroadcast);
	bool (*FireEventClientSide)(IGameEventManager2*, IGameEvent* event);
	IGameEvent* (*DuplicateEvent)(IGameEventManager2*, IGameEvent* event);
	void (*FreeEvent)(IGameEventManager2*, IGameEvent* event);
	bool (*SerializeEvent)(IGameEventManager2*, IGameEvent* event, bf_write* buf);
	IGameEvent* (*UnserializeEvent)(IGameEventManager2*, bf_read* buf);
} IGameEventManager2_VMT;

struct IGameEventManager2// : public IBaseInterface
{
	IGameEventManager2_VMT* vmt;
};

typedef struct IGameEventListener IGameEventListener;

typedef struct IGameEventListener_VMT
{
	//virtual ~IGameEventListener(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*FireGameEvent)(IGameEventListener*, KeyValues* event);
} IGameEventListener_VMT;

struct IGameEventListener
{
	IGameEventListener_VMT* vmt;
};

typedef struct IGameEventManager IGameEventManager;

typedef struct IGameEventManager_VMT
{
	//virtual ~IGameEventManager(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	int (*LoadEventsFromFile)(IGameEventManager*, const char* filename);
	void (*Reset)(IGameEventManager*);
	KeyValues* (*GetEvent)(IGameEventManager*, const char* name);
	bool (*AddListener)(IGameEventManager*, IGameEventListener* listener, const char* event, bool bIsServerSide);
	bool (*AddGlobalListener)(IGameEventManager*, IGameEventListener* listener, bool bIsServerSide);
	void (*RemoveListener)(IGameEventManager*, IGameEventListener* listener);
	bool (*FireEvent)(IGameEventManager*, KeyValues* event);
	bool (*FireEventServerOnly)(IGameEventManager*, KeyValues* event);
	bool (*FireEventClientOnly)(IGameEventManager*, KeyValues* event);
	bool (*SerializeKeyValues)(IGameEventManager*, KeyValues* event, bf_write* buf, CGameEvent* eventtype);
	KeyValues* (*UnserializeKeyValue)(IGameEventManager*, bf_read* msg);
} IGameEventManager_VMT;

struct IGameEventManager// : public IBaseInterface
{
	IGameEventManager_VMT* vmt;
};

#define IGameEventManager2_REFL "IGameEventManager2"
#define IGameEventManager2_SIGNATURE "GAMEEVENTSMANAGER002"
#define IGameEventManager2_MODULE "engine"

#endif
