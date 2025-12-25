#if !defined(MiscEdict_Included)
#define MiscEdict_Included

#define ENTREF_MASK 1 << 31
#define MAX_EDICT_BITS 11
#define MAX_EDICTS (1 << MAX_EDICT_BITS)
#define NUM_ENT_ENTRY_BITS (MAX_EDICT_BITS + 2)
#define NUM_ENT_ENTRIES (1 << NUM_ENT_ENTRY_BITS)
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF

#define FL_EDICT_CHANGED (1 << 0)
#define FL_EDICT_FREE (1 << 1)
#define FL_EDICT_FULL (1 << 2)
#define FL_EDICT_FULLCHECK (0 << 0) // call ShouldTransmit() each time, this is a fake flag
#define FL_EDICT_ALWAYS (1 << 3)
#define FL_EDICT_DONTSEND (1 << 4)
#define FL_EDICT_PVSCHECK (1 << 5)
// Used by local network backdoor.
#define FL_EDICT_PENDING_DORMANT_CHECK (1 << 6)
#define FL_EDICT_DIRTY_PVS_INFORMATION (1 << 7)
// This is used internally to edict_t to remember that it's carrying a
// "full change list" - all its properties might have changed their value.
#define FL_FULL_EDICT_CHANGED (1 << 8)

typedef struct IServerNetworkable IServerNetworkable;
typedef struct IServerUnknown IServerUnknown;

typedef struct CEdictChangeInfo CEdictChangeInfo;
typedef struct CSharedEdictChangeInfo CSharedEdictChangeInfo;
typedef struct IChangeInfoAccessor IChangeInfoAccessor;

#define MAX_CHANGE_OFFSETS 19
#define MAX_EDICT_CHANGE_INFOS 100

struct CEdictChangeInfo
{
	unsigned short m_ChangeOffsets[MAX_CHANGE_OFFSETS];
	unsigned short m_nChangeOffsets;
};

struct CSharedEdictChangeInfo
{
	unsigned short m_iSerialNumber;
	CEdictChangeInfo m_ChangeInfos[MAX_EDICT_CHANGE_INFOS];
	unsigned short m_nChangeInfos;
};

struct IChangeInfoAccessor
{
	unsigned short m_iChangeInfo;
	unsigned short m_iChangeInfoSerialNumber;
};

typedef struct CBaseEdict
{
#ifdef _XBOX
	unsigned short m_fStateFlags;
#else
	int m_fStateFlags;
#endif

#if VALVE_LITTLE_ENDIAN
	short m_NetworkSerialNumber;
	short m_EdictIndex;
#else
	short m_EdictIndex;
	short m_NetworkSerialNumber;
#endif

	IServerNetworkable* m_pNetworkable;
	IServerUnknown* m_pUnk;
} CBaseEdict;

typedef struct edict_t
{
	CBaseEdict m_CBaseEdict;

	float freetime;
} edict_t;

#endif
