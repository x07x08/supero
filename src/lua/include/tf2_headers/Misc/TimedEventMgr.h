#if !defined(MiscTimedEventMgr_Included)
#define MiscTimedEventMgr_Included

// Incomplete

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

typedef struct CTimedEventMgr CTimedEventMgr;

typedef struct IEventRegisterCallback IEventRegisterCallback;

typedef struct IEventRegisterCallback_VMT
{
	void (*FireEvent)(IEventRegisterCallback*);
} IEventRegisterCallback_VMT;

struct IEventRegisterCallback
{
	IEventRegisterCallback_VMT* vmt;
};

typedef struct CEventRegister CEventRegister;

struct CEventRegister
{
	CTimedEventMgr* m_pEventMgr;
	float m_flNextEventTime;
	float m_flUpdateInterval;
	IEventRegisterCallback* m_pCallback;
	bool m_bRegistered;
};

#endif
