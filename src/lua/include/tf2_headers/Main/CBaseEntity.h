#if !defined(MainCBaseEntity_Included)
#define MainCBaseEntity_Included

#include "Definitions.h"
#include "Misc/CommonMacros.h"

//#include "Interfaces/IVModelInfo.h"
#include "Main/CCollisionProperty.h"
#include "Main/IServerEntity.h"
#include "Misc/VariantT.h"
#include "Misc/Datamap.h"
#include "Misc/ServerNetworkProperty.h"
#include "Misc/PredictableId.h"
#include "Misc/TakeDamageInfo.h"
#include "Misc/ThreadTools.h"
//#include "Interfaces/IEngineTrace.h"

typedef struct CCheckTransmitInfo CCheckTransmitInfo;
typedef struct ITraceFilter ITraceFilter;

//typedef struct CDamageModifier CDamageModifier;
typedef struct CDmgAccumulator CDmgAccumulator;

//typedef struct CSoundParameters CSoundParameters;

typedef struct AI_CriteriaSet AI_CriteriaSet;
typedef struct IResponseSystem IResponseSystem;
typedef struct IEntitySaveUtils IEntitySaveUtils;
//typedef struct CRecipientFilter CRecipientFilter;
typedef struct CStudioHdr CStudioHdr;

// Matching the high level concept is significantly better than other criteria
// FIXME:  Could do this in the script file by making it required and bumping up weighting there instead...
#define CONCEPT_WEIGHT 5.0f

// saverestore.h declarations
//typedef struct CSaveRestoreData CSaveRestoreData;
//typedef struct typedescription_t typedescription_t;
typedef struct ISave ISave;
typedef struct IRestore IRestore;
//typedef struct CBaseEntity CBaseEntity;
//typedef struct CEntityMapData CEntityMapData;
typedef struct CBaseCombatWeapon CBaseCombatWeapon;
typedef struct IPhysicsObject IPhysicsObject;
typedef struct IPhysicsShadowController IPhysicsShadowController;
typedef struct CBaseCombatCharacter CBaseCombatCharacter;
//typedef struct CTeam CTeam;
//typedef struct Vector Vector;
typedef struct gamevcollisionevent_t gamevcollisionevent_t;
typedef struct CBaseAnimating CBaseAnimating;
typedef struct CBasePlayer CBasePlayer;
typedef struct IServerVehicle IServerVehicle;
//typedef struct solid_t solid_t;
typedef struct notify_system_event_params_t notify_system_event_params_t;
typedef struct CAI_BaseNPC CAI_BaseNPC;
//typedef struct CAI_Senses CAI_Senses;
//typedef struct CSquadNPC CSquadNPC;
//typedef struct variant_t variant_t;
typedef struct CEventAction CEventAction;
//typedef struct KeyValueData_s KeyValueData;
//typedef struct CUserCmd CUserCmd;
//typedef struct CSkyCamera CSkyCamera;
typedef struct CEntityMapData CEntityMapData;
typedef struct INextBot INextBot;
typedef struct IHasAttributes IHasAttributes;

typedef CUtlVector EntityList_t;

#if defined(HL2_DLL)

// For CLASSIFY
typedef enum Class_T
{
	CLASS_NONE = 0,
	CLASS_PLAYER,
	CLASS_PLAYER_ALLY,
	CLASS_PLAYER_ALLY_VITAL,
	CLASS_ANTLION,
	CLASS_BARNACLE,
	CLASS_BULLSEYE,
	// CLASS_BULLSQUID,
	CLASS_CITIZEN_PASSIVE,
	CLASS_CITIZEN_REBEL,
	CLASS_COMBINE,
	CLASS_COMBINE_GUNSHIP,
	CLASS_CONSCRIPT,
	CLASS_HEADCRAB,
	// CLASS_HOUNDEYE,
	CLASS_MANHACK,
	CLASS_METROPOLICE,
	CLASS_MILITARY,
	CLASS_SCANNER,
	CLASS_STALKER,
	CLASS_VORTIGAUNT,
	CLASS_ZOMBIE,
	CLASS_PROTOSNIPER,
	CLASS_MISSILE,
	CLASS_FLARE,
	CLASS_EARTH_FAUNA,
	CLASS_HACKED_ROLLERMINE,
	CLASS_COMBINE_HUNTER,

	NUM_AI_CLASSES
} Class_T;

#elif defined(HL1_DLL)

typedef enum Class_T
{
	CLASS_NONE = 0,
	CLASS_MACHINE,
	CLASS_PLAYER,
	CLASS_HUMAN_PASSIVE,
	CLASS_HUMAN_MILITARY,
	CLASS_ALIEN_MILITARY,
	CLASS_ALIEN_MONSTER,
	CLASS_ALIEN_PREY,
	CLASS_ALIEN_PREDATOR,
	CLASS_INSECT,
	CLASS_PLAYER_ALLY,
	CLASS_PLAYER_BIOWEAPON,
	CLASS_ALIEN_BIOWEAPON,

	NUM_AI_CLASSES
} Class_T;

#elif defined(INVASION_DLL)

typedef enum Class_T
{
	CLASS_NONE = 0,
	CLASS_PLAYER,
	CLASS_PLAYER_ALLY,
	CLASS_PLAYER_ALLY_VITAL,
	CLASS_ANTLION,
	CLASS_BARNACLE,
	CLASS_BULLSEYE,
	// CLASS_BULLSQUID,
	CLASS_CITIZEN_PASSIVE,
	CLASS_CITIZEN_REBEL,
	CLASS_COMBINE,
	CLASS_COMBINE_GUNSHIP,
	CLASS_CONSCRIPT,
	CLASS_HEADCRAB,
	// CLASS_HOUNDEYE,
	CLASS_MANHACK,
	CLASS_METROPOLICE,
	CLASS_MILITARY,
	CLASS_SCANNER,
	CLASS_STALKER,
	CLASS_VORTIGAUNT,
	CLASS_ZOMBIE,
	CLASS_PROTOSNIPER,
	CLASS_MISSILE,
	CLASS_FLARE,
	CLASS_EARTH_FAUNA,
	NUM_AI_CLASSES
} Class_T;

#elif defined(CSTRIKE_DLL)

typedef enum Class_T
{
	CLASS_NONE = 0,
	CLASS_PLAYER,
	CLASS_PLAYER_ALLY,
	NUM_AI_CLASSES
} Class_T;

#else

typedef enum Class_T
{
	CLASS_NONE = 0,
	CLASS_PLAYER,
	CLASS_PLAYER_ALLY,
	NUM_AI_CLASSES
} Class_T;

#endif

typedef struct CBaseEntity CBaseEntity;

//
// Structure passed to input handlers.
//
typedef struct inputdata_t
{
	CBaseEntity* pActivator;  // The entity that initially caused this chain of output events.
	CBaseEntity* pCaller;     // The entity that fired this particular output.
	variant_t value;          // The data parameter for this output.
	int nOutputID;            // The unique ID of the output that was fired.
} inputdata_t;

// Serializable list of context as set by entity i/o and used for deducing proper
// speech state, et al.
typedef struct ResponseContext_t
{
	//DECLARE_SIMPLE_DATADESC();

	const char* m_iszName;
	const char* m_iszValue;
	float m_fExpirationTime;  // when to expire context (0 == never)
} ResponseContext_t;

typedef enum EntityEvent_t
{
	ENTITY_EVENT_WATER_TOUCH = 0,  // No data needed
	ENTITY_EVENT_WATER_UNTOUCH,    // No data needed
	ENTITY_EVENT_PARENT_CHANGED,   // No data needed
} EntityEvent_t;

typedef enum
{
	USE_OFF = 0,
	USE_ON = 1,
	USE_SET = 2,
	USE_TOGGLE = 3
} USE_TYPE;

typedef void (*BASEPTR)(CBaseEntity*);
typedef void (*ENTITYFUNCPTR)(CBaseEntity*, CBaseEntity* pOther);
typedef void (*USEPTR)(CBaseEntity*, CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

// Things that toggle (buttons/triggers/doors) need this
typedef enum TOGGLE_STATE
{
	TS_AT_TOP,
	TS_AT_BOTTOM,
	TS_GOING_UP,
	TS_GOING_DOWN
} TOGGLE_STATE;

// Debug overlay bits
typedef enum DebugOverlayBits_t
{
	OVERLAY_TEXT_BIT = 0x00000001,         // show text debug overlay for this entity
	OVERLAY_NAME_BIT = 0x00000002,         // show name debug overlay for this entity
	OVERLAY_BBOX_BIT = 0x00000004,         // show bounding box overlay for this entity
	OVERLAY_PIVOT_BIT = 0x00000008,        // show pivot for this entity
	OVERLAY_MESSAGE_BIT = 0x00000010,      // show messages for this entity
	OVERLAY_ABSBOX_BIT = 0x00000020,       // show abs bounding box overlay
	OVERLAY_RBOX_BIT = 0x00000040,         // show the rbox overlay
	OVERLAY_SHOW_BLOCKSLOS = 0x00000080,   // show entities that block NPC LOS
	OVERLAY_ATTACHMENTS_BIT = 0x00000100,  // show attachment points
	OVERLAY_AUTOAIM_BIT = 0x00000200,      // Display autoaim radius

	OVERLAY_NPC_SELECTED_BIT = 0x00001000,     // the npc is current selected
	OVERLAY_NPC_NEAREST_BIT = 0x00002000,      // show the nearest node of this npc
	OVERLAY_NPC_ROUTE_BIT = 0x00004000,        // draw the route for this npc
	OVERLAY_NPC_TRIANGULATE_BIT = 0x00008000,  // draw the triangulation for this npc
	OVERLAY_NPC_ZAP_BIT = 0x00010000,          // destroy the NPC
	OVERLAY_NPC_ENEMIES_BIT = 0x00020000,      // show npc's enemies
	OVERLAY_NPC_CONDITIONS_BIT = 0x00040000,   // show NPC's current conditions
	OVERLAY_NPC_SQUAD_BIT = 0x00080000,        // show npc squads
	OVERLAY_NPC_TASK_BIT = 0x00100000,         // show npc task details
	OVERLAY_NPC_FOCUS_BIT = 0x00200000,        // show line to npc's enemy and target
	OVERLAY_NPC_VIEWCONE_BIT = 0x00400000,     // show npc's viewcone
	OVERLAY_NPC_KILL_BIT = 0x00800000,         // kill the NPC, running all appropriate AI.

	OVERLAY_WC_CHANGE_ENTITY = 0x01000000,  // object changed during WC edit
	OVERLAY_BUDDHA_MODE = 0x02000000,       // take damage but don't die

	OVERLAY_NPC_STEERING_REGULATIONS = 0x04000000,  // Show the steering regulations associated with the NPC

	OVERLAY_TASK_TEXT_BIT = 0x08000000,  // show task and schedule names when they start

	OVERLAY_PROP_DEBUG = 0x10000000,

	OVERLAY_NPC_RELATION_BIT = 0x20000000,  // show relationships between target and all children

	OVERLAY_VIEWOFFSET = 0x40000000,  // show view offset
} DebugOverlayBits_t;

typedef enum notify_system_event_t
{
	NOTIFY_EVENT_TELEPORT = 0,
	NOTIFY_EVENT_DESTROY,
} notify_system_event_t;

typedef struct TimedOverlay_t TimedOverlay_t;
typedef int* HSCRIPT;

typedef struct thinkfunc_t_server
{
	BASEPTR m_pfnThink;
	const char* m_iszContext;
	int m_nNextThinkTick;
	int m_nLastThinkTick;

	//DECLARE_SIMPLE_DATADESC();
} thinkfunc_t_server;

typedef struct EmitSound_t EmitSound_t;
//typedef struct rotatingpushmove_t rotatingpushmove_t;

// think function handling
typedef enum thinkmethods_t_server
{
	THINK_FIRE_ALL_FUNCTIONS_SERVER,
	THINK_FIRE_BASE_ONLY_SERVER,
	THINK_FIRE_ALL_BUT_BASE_SERVER,
} thinkmethods_t_server;

typedef struct CScriptScope
{
	HSCRIPT m_hScope;
	int m_flags;
	CUtlVector m_FuncHandles;
} CScriptScope;

typedef struct CUtlLinkedList__m_DamageModifiers
{
	void* m_Memory; // M
	int m_Head; // I
	int m_Tail; // I
	int m_FirstFree; // I
	int m_ElementCount; // I
	int m_NumAlloced; // I
	uintptr_t m_LastAlloc; // typename M::Iterator_t

	void* m_pElements; // ListElem_t*
} CUtlLinkedList__m_DamageModifiers;

typedef struct CBaseEntityOutput
{
	variant_t m_Value;
	CEventAction* m_ActionList;
} CBaseEntityOutput;

typedef CBaseEntityOutput COutputEvent;
typedef struct ScriptClassDesc_t ScriptClassDesc_t;

// All virtuals are used or implemented in some way.
// What's unclear is marked with "// in vtable".
// Cross reference with a VMT dumper.

typedef struct CBaseEntity_VMT
{
	IServerEntity_VMT vmt_IServerEntity;

	// prediction system
	//DECLARE_PREDICTABLE();
	// network data
	DECLARE_SERVERCLASS(CBaseEntity);
	// data description
	DECLARE_DATADESC(CBaseEntity);
	// script description
	//DECLARE_ENT_SCRIPTDESC();
	ScriptClassDesc_t* (*GetScriptDesc)(CBaseEntity*);

	// IHandleEntity overrides.
	// IServerUnknown overrides
	// IServerEntity overrides.

	void (*SetModelIndexOverride)(CBaseEntity*, int index, int nValue);

	// virtual methods for derived classes to override
	bool (*TestCollision)(CBaseEntity*, const Ray_t* ray, unsigned int mask, trace_t* trace);
	bool (*TestHitboxes)(CBaseEntity*, const Ray_t* ray, unsigned int fContentsMask, trace_t* tr);
	void (*ComputeWorldSpaceSurroundingBox)(CBaseEntity*, Vector* pWorldMins, Vector* pWorldMaxs);

	// Called by physics to see if we should avoid a collision test....
	bool (*ShouldCollide)(CBaseEntity*, int collisionGroup, int contentsMask); // const

	// virtual methods; you can override these

	// Owner entity.
	// FIXME: These are virtual only because of CNodeEnt
	void (*SetOwnerEntity)(CBaseEntity*, CBaseEntity* pOwner);
	void (*SetScriptOwnerEntity)(CBaseEntity*, HSCRIPT pOwner);

	// Only CBaseEntity implements these. CheckTransmit calls the virtual ShouldTransmit to see if the
	// entity wants to be sent. If so, it calls SetTransmit, which will mark any dependents for transmission too.
	int (*ShouldTransmit)(CBaseEntity*, const CCheckTransmitInfo* pInfo);

	// Do NOT call this directly. Use DispatchUpdateTransmitState.
	int (*UpdateTransmitState)(CBaseEntity*);

	// This marks the entity for transmission and passes the SetTransmit call to any dependents.
	void (*SetTransmit)(CBaseEntity*, CCheckTransmitInfo* pInfo, bool bAlways);

	const char* (*GetTracerType)(CBaseEntity*);

	// initialization
	void (*Spawn)(CBaseEntity*); // ?? Might be in the wrong place
	void (*Precache)(CBaseEntity*); // {} // in vtable

	void (*SetModel)(CBaseEntity*, const char* szModelName);

	// Notification on model load. May be called multiple times for dynamic models.
	// Implementations must call BaseClass::OnNewModel and pass return value through.
	CStudioHdr* (*OnNewModel)(CBaseEntity*);

	void (*PostConstructor)(CBaseEntity*, const char* szClassname);
	void (*PostClientActive)(CBaseEntity*);
	void (*ParseMapData)(CBaseEntity*, CEntityMapData* mapData);
	bool (*KeyValueChar)(CBaseEntity*, const char* szKeyName, const char* szValue);
	bool (*KeyValueFloat)(CBaseEntity*, const char* szKeyName, float flValue);
	bool (*KeyValueVec)(CBaseEntity*, const char* szKeyName, const Vector* vecValue);
	bool (*GetKeyValue)(CBaseEntity*, const char* szKeyName, char* szValue, int iMaxLen);

	// Activate - called for each entity after each load game and level load
	void (*Activate)(CBaseEntity*);

	// Set the movement parent. Your local origin and angles will become relative to this parent.
	// If iAttachment is a valid attachment on the parent, then your local origin and angles
	// are relative to the attachment on this entity. If iAttachment == -1, it'll preserve the
	// current m_iParentAttachment.
	void (*SetParent)(CBaseEntity*, CBaseEntity* pNewParent, int iAttachment);

	// capabilities
	int (*ObjectCaps)(CBaseEntity*);

	// handles an input (usually caused by outputs)
	// returns true if the the value in the pass in should be set, false if the input is to be ignored
	bool (*AcceptInput)(CBaseEntity*, const char* szInputName, CBaseEntity* pActivator, CBaseEntity* pCaller, variant_t Value, int outputID);

	// Returns the origin at which to play an inputted dispatcheffect
	void (*GetInputDispatchEffectPosition)(CBaseEntity*, const char* sInputString, Vector* pOrigin, QAngle* pAngles);

	// Debug Overlays
	void (*DrawDebugGeometryOverlays)(CBaseEntity*);
	int (*DrawDebugTextOverlays)(CBaseEntity*);

	// save/restore
	// only overload these if you have special data to serialize
	int (*Save)(CBaseEntity*, ISave* save);
	int (*Restore)(CBaseEntity*, IRestore* restore);
	bool (*ShouldSavePhysics)(CBaseEntity*);

	// handler to reset stuff before you are restored
	// NOTE: Always chain to base class when implementing this!
	void (*OnSave)(CBaseEntity*, IEntitySaveUtils* pSaveUtils);

	// handler to reset stuff after you are restored
	// called after all entities have been loaded from all affected levels
	// called before activate
	// NOTE: Always chain to base class when implementing this!
	void (*OnRestore)(CBaseEntity*);

	// returns the edict index the entity requires when used in save/restore (eg players, world)
	// -1 means it doesn't require any special index
	int (*RequiredEdictIndex)(CBaseEntity*); // { return -1; } // in vtable

	// interface function pts
	//virtual void MoveDone(void)
	//{
	//	if (m_pfnMoveDone) (this->*m_pfnMoveDone)();
	//};
	void (*MoveDone)(CBaseEntity*); // in vtable

	// Think function handling
	//virtual void Think(void)
	//{
	//	if (m_pfnThink) (this->*m_pfnThink)();
	//};
	void (*Think)(CBaseEntity*); // in vtable

	// was pev->nextthink
	//CNetworkVarForDerived(int, m_nNextThinkTick);
	void (*NetworkStateChanged_m_nNextThinkTick)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_nNextThinkTick)(CBaseEntity*, void* pVar); // {} // in vtable

	// Returns a CBaseAnimating if the entity is derived from CBaseAnimating.
	CBaseAnimating* (*GetBaseAnimating)(CBaseEntity*); // { return 0; } // in vtable

	IResponseSystem* (*GetResponseSystem)(CBaseEntity*);
	void (*DispatchResponse)(CBaseEntity*, const char* conceptName);

	// Classify - returns the type of group (i.e, "houndeye", or "human military" so that NPCs with different classnames
	// still realize that they are teammates. (overridden for NPCs that form groups)
	Class_T (*Classify)(CBaseEntity*);
	void (*DeathNotice)(CBaseEntity*, CBaseEntity* pVictim); // {}  // NPC maker children use this to tell the NPC maker that they have died. // in vtable
	bool (*ShouldAttractAutoAim)(CBaseEntity*, CBaseEntity* pAimingEnt); // { return ((GetFlags() * FL_AIMTARGET) != 0); } // in vtable
	float (*GetAutoAimRadius)(CBaseEntity*);
	Vector (*GetAutoAimCenter)(CBaseEntity*); // { return WorldSpaceCenter(); } // in vtable

	ITraceFilter* (*GetBeamTraceFilter)(CBaseEntity*);

	// Call this to do a TraceAttack on an entity, performs filtering. Don't call TraceAttack() directly except when chaining up to base class
	//void DispatchTraceAttack(const CTakeDamageInfo* info, const Vector* vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL);
	bool (*PassesDamageFilter)(CBaseEntity*, const CTakeDamageInfo* info);

	void (*TraceAttack)(CBaseEntity*, const CTakeDamageInfo* info, const Vector* vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator);

	bool (*CanBeHitByMeleeAttack)(CBaseEntity*, CBaseEntity* pAttacker); // { return true; }

	// returns the amount of damage inflicted
	int (*OnTakeDamage)(CBaseEntity*, const CTakeDamageInfo* info);

	void (*AdjustDamageDirection)(CBaseEntity*, const CTakeDamageInfo* info, Vector* dir, CBaseEntity* pEnt); // {} // in vtable

	int (*TakeHealth)(CBaseEntity*, float flHealth, int bitsDamageType);

	bool (*IsAlive)(CBaseEntity*);
	// Entity killed (only fired once)
	void (*Event_Killed)(CBaseEntity*, const CTakeDamageInfo* info);

	// Notifier that I've killed some other entity. (called from Victim's Event_Killed).
	void (*Event_KilledOther)(CBaseEntity*, CBaseEntity* pVictim, const CTakeDamageInfo* info); // { return; } // in vtable

	// UNDONE: Make this data?
	int (*BloodColor)(CBaseEntity*);

	//void TraceBleed(float flDamage, const Vector* vecDir, trace_t* ptr, int bitsDamageType);
	bool (*IsTriggered)(CBaseEntity*, CBaseEntity* pActivator); // { return true; } // in vtable
	bool (*IsNPC)(CBaseEntity*); // const { return false; } // in vtable
#ifdef NEXT_BOT
	bool (*IsNextBot)(CBaseEntity*); // const { return false; } // in vtable
#endif
	CBaseCombatCharacter* (*MyCombatCharacterPointer)(CBaseEntity*); // { return NULL; } // in vtable
	INextBot* (*MyNextBotPointer)(CBaseEntity*); // { return NULL; } // in vtable
	float (*GetDelay)(CBaseEntity*); // { return 0; } // in vtable
	bool (*IsMoving)(CBaseEntity*);
	char const* (*DamageDecal)(CBaseEntity*, int bitsDamageType, int gameMaterial);
	void (*DecalTrace)(CBaseEntity*, trace_t* pTrace, char const* decalName);
	void (*ImpactTrace)(CBaseEntity*, trace_t* pTrace, int iDamageType, const char* pCustomImpactName);

	bool (*OnControls)(CBaseEntity*, CBaseEntity* pControls); // { return false; } // in vtable
	bool (*HasTarget)(CBaseEntity*, const char* targetname); // in vtable
	bool (*IsPlayer)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsNetClient)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsTemplate)(CBaseEntity*); // { return false; } // in vtable
	bool (*IsBaseObject)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsBaseTrain)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsCombatItem)(CBaseEntity*); // const { return false; } // in vtable
#ifdef TF_DLL
	bool (*IsProjectileCollisionTarget)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsFuncLOD)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsBaseProjectile)(CBaseEntity*); // const { return false; } // in vtable
#endif  // TF_DLL

	bool (*IsBaseCombatWeapon)(CBaseEntity*); // const { return false; } // in vtable
	bool (*IsWearable)(CBaseEntity*); // const { return false; } // in vtable
	CBaseCombatWeapon* (*MyCombatWeaponPointer)(CBaseEntity*); // { return NULL; } // in vtable

	// If this is a vehicle, returns the vehicle interface
	IServerVehicle* (*GetServerVehicle)(CBaseEntity*); // { return NULL; } // in vtable

	// UNDONE: Make this data instead of procedural?
	bool (*IsViewable)(CBaseEntity*); // is this something that would be looked at (model, sprite, etc.)?

	// Team Handling
	void (*ChangeTeam)(CBaseEntity*, int iTeamNum); // Assign this entity to a team.

	// Entity events... these are events targetted to a particular entity
	// Each event defines its own well-defined event data structure
	void (*OnEntityEvent)(CBaseEntity*, EntityEvent_t event, void* pEventData);

	// UNDONE: Do these three functions actually need to be virtual???
	bool (*CanStandOn)(CBaseEntity*, CBaseEntity* pSurface); // const { return (pSurface ** !pSurface->IsStandable()) ? false : true; } // in vtable
	bool (*CanStandOnEdict)(CBaseEntity*, edict_t* ent); // const { return CanStandOn(GetContainingEntity(ent)); } // in vtable
	CBaseEntity* (*GetEnemy)(CBaseEntity*); // { return NULL; } // in vtable
	CBaseEntity* (*GetEnemy_const)(CBaseEntity*); // const { return NULL; } // in vtable

	void (*Use)(CBaseEntity*, CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void (*StartTouch)(CBaseEntity*, CBaseEntity* pOther);
	void (*Touch)(CBaseEntity*, CBaseEntity* pOther);
	void (*EndTouch)(CBaseEntity*, CBaseEntity* pOther);
	void (*StartBlocked)(CBaseEntity*, CBaseEntity* pOther); // {} // in vtable
	void (*Blocked)(CBaseEntity*, CBaseEntity* pOther);
	void (*EndBlocked)(CBaseEntity*); // {} // in vtable

	// Physics simulation
	void (*PhysicsSimulate)(CBaseEntity*);

	void (*UpdateOnRemove)(CBaseEntity*);
	void (*StopLoopingSounds)(CBaseEntity*); // {} // in vtable

	bool (*SUB_AllowedToFade)(CBaseEntity*);

	// change position, velocity, orientation instantly
	// passing NULL means no change
	void (*Teleport)(CBaseEntity*, const Vector* newPosition, const QAngle* newAngles, const Vector* newVelocity);
	// notify that another entity (that you were watching) was teleported
	void (*NotifySystemEvent)(CBaseEntity*, CBaseEntity* pNotify, notify_system_event_t eventType, const notify_system_event_params_t* params);

	//int ShouldToggle(USE_TYPE useType, int currentState);

	// UNDONE: Move these virtuals to CBaseCombatCharacter?
	void (*MakeTracer)(CBaseEntity*, const Vector* vecTracerSrc, const trace_t* tr, int iTracerType);
	int (*GetTracerAttachment)(CBaseEntity*);
	void (*FireBullets)(CBaseEntity*, const FireBulletsInfo_t* info);
	void (*DoImpactEffect)(CBaseEntity*, trace_t* tr, int nDamageType); // give shooter a chance to do a custom impact.

	// OLD VERSION! Use the struct version
	//void FireBullets(int cShots, const Vector* vecSrc, const Vector* vecDirShooting, const Vector* vecSpread, float flDistance, int iAmmoType, int iTracerFreq = 4, int firingEntID = -1, int attachmentID = -1, int iDamage = 0, CBaseEntity* pAttacker = NULL, bool bFirstShotAccurate = false, bool bPrimaryAttack = true);
	void (*ModifyFireBulletsDamage)(CBaseEntity*, CTakeDamageInfo* dmgInfo); // {} // in vtable

	CBaseEntity* (*Respawn)(CBaseEntity*); // { return NULL; } // in vtable

	bool (*IsLockedByMaster)(CBaseEntity*); // { return false; } // in vtable

	// Health accessors.
	int (*GetMaxHealth)(CBaseEntity*); // const { return m_iMaxHealth; } // in vtable

	void (*ModifyOrAppendCriteria)(CBaseEntity*, AI_CriteriaSet* set);

	//CNetworkVarForDerived(int, m_iMaxHealth);  // CBaseEntity doesn't care about changes to this variable, but there are derived classes that do.
	void (*NetworkStateChanged_m_iMaxHealth)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_iMaxHealth)(CBaseEntity*, void* pVar); // {} // in vtable
	//CNetworkVarForDerived(int, m_iHealth);
	void (*NetworkStateChanged_m_iHealth)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_iHealth)(CBaseEntity*, void* pVar); // {} // in vtable

	//CNetworkVarForDerived(char, m_lifeState);
	void (*NetworkStateChanged_m_lifeState)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_lifeState)(CBaseEntity*, void* pVar); // {} // in vtable
	//CNetworkVarForDerived(char, m_takedamage);
	void (*NetworkStateChanged_m_takedamage)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_takedamage)(CBaseEntity*, void* pVar); // {} // in vtable

	// VSCRIPT
	void (*RunVScripts)(CBaseEntity*);

	// Damage accessors
	int (*GetDamageType)(CBaseEntity*); // const
	float (*GetDamage)(CBaseEntity*); // { return 0; } // in vtable
	void (*SetDamage)(CBaseEntity*, float flDamage); // {} // in vtable

	Vector (*EyePosition)(CBaseEntity*); // position of eyes

	const QAngle* (*EyeAngles)(CBaseEntity*); // Direction of eyes in world space
	const QAngle* (*LocalEyeAngles)(CBaseEntity*); // Direction of eyes
	Vector (*EarPosition)(CBaseEntity*); // position of ears

	Vector (*BodyTarget)(CBaseEntity*, const Vector* posSrc, bool bNoisy); // position to shoot at
	Vector (*HeadTarget)(CBaseEntity*, const Vector* posSrc);
	void (*GetVectors)(CBaseEntity*, Vector* forward, Vector* right, Vector* up); // const

	const Vector* (*GetViewOffset)(CBaseEntity*); // const
	void (*SetViewOffset)(CBaseEntity*, const Vector* v);

	Vector (*GetSmoothedVelocity)(CBaseEntity*);

	// FIXME: Figure out what to do about this
	void (*GetVelocity)(CBaseEntity*, Vector* vVelocity, AngularImpulse* vAngVelocity);

	bool (*FVisibleEntity)(CBaseEntity*, CBaseEntity* pEntity, int traceMask, CBaseEntity** ppBlocker);
	bool (*FVisible)(CBaseEntity*, const Vector* vecTarget, int traceMask, CBaseEntity** ppBlocker);

	bool (*CanBeSeenBy)(CBaseEntity*, CAI_BaseNPC* pNPC); // { return true; }  // allows entities to be 'invisible' to NPC senses. // in vtable

	// This function returns a value that scales all damage done by this entity.
	// Use CDamageModifier to hook in damage modifiers on a guy.
	float (*GetAttackDamageScale)(CBaseEntity*, CBaseEntity* pVictim);
	// This returns a value that scales all damage done to this entity
	// Use CDamageModifier to hook in damage modifiers on a guy.
	float (*GetReceivedDamageScale)(CBaseEntity*, CBaseEntity* pAttacker);

	// Gets the velocity we impart to a player standing on us
	void (*GetGroundVelocityToApply)(CBaseEntity*, Vector* vecGroundVel); // { vecGroundVel = vec3_origin; } // in vtable

	bool (*PhysicsSplash)(CBaseEntity*, const Vector* centerPoint, const Vector* normal, float rawSpeed, float scaledSpeed); // { return false; } // in vtable
	void (*Splash)(CBaseEntity*); // {} // in vtable

	// NOTE: The world space center *may* move when the entity rotates.
	const Vector* (*WorldSpaceCenter)(CBaseEntity*); // const

	// Used by the PAS filters to ask the entity where in world space the sounds it emits come from.
	// This is used right now because if you have something sitting on an incline, using our axis-aligned
	// bounding boxes can return a position in solid space, so you won't hear sounds emitted by the object.
	// For now, we're hacking around it by moving the sound emission origin up on certain objects like vehicles.
	//
	// When OBBs get in, this can probably go away.
	Vector (*GetSoundEmissionOrigin)(CBaseEntity*); // const

	void (*ModifyEmitSoundParams)(CBaseEntity*, EmitSound_t* params);

	bool (*IsDeflectable)(CBaseEntity*); // { return false; } // in vtable
	void (*Deflected)(CBaseEntity*, CBaseEntity* pDeflectedBy, Vector* vecDir); // {} // in vtable

	bool (*CreateVPhysics)(CBaseEntity*);

	// Force a non-solid (ie. solid_trigger) physics object to collide with other entities.
	bool (*ForceVPhysicsCollide)(CBaseEntity*, CBaseEntity* pEntity); // { return false; } // in vtable

	// destroy and remove the physics object for this entity
	void (*VPhysicsDestroyObject)(CBaseEntity*);

	void (*VPhysicsUpdate)(CBaseEntity*, IPhysicsObject* pPhysics);

	// react physically to damage (called from CBaseEntity::OnTakeDamage() by default)
	int (*VPhysicsTakeDamage)(CBaseEntity*, const CTakeDamageInfo* info);
	void (*VPhysicsShadowCollision)(CBaseEntity*, int index, gamevcollisionevent_t* pEvent);
	void (*VPhysicsShadowUpdate)(CBaseEntity*, IPhysicsObject* pPhysics); // {}
	void (*VPhysicsCollision)(CBaseEntity*, int index, gamevcollisionevent_t* pEvent);
	void (*VPhysicsFriction)(CBaseEntity*, IPhysicsObject* pObject, float energy, int surfaceProps, int surfacePropsHit);

	// update the shadow so it will coincide with the current AI position at some time
	// in the future (or 0 for now)
	void (*UpdatePhysicsShadowToCurrentPosition)(CBaseEntity*, float deltaTime);
	int (*VPhysicsGetObjectList)(CBaseEntity*, IPhysicsObject** pList, int listMax);
	bool (*VPhysicsIsFlesh)(CBaseEntity*);

	// Callbacks for the physgun/cannon picking up an entity
	CBasePlayer* (*HasPhysicsAttacker)(CBaseEntity*, float dt); // { return NULL; } // in vtable

	// UNDONE: Make this data?
	unsigned int (*PhysicsSolidMaskForEntity)(CBaseEntity*); // const

	void (*ResolveFlyCollisionCustom)(CBaseEntity*, trace_t* trace, Vector* vecVelocity);

	// Implement this if you use MOVETYPE_CUSTOM
	void (*PerformCustomPhysics)(CBaseEntity*, Vector* pNewPosition, Vector* pNewVelocity, QAngle* pNewAngles, QAngle* pNewAngVelocity);

	// origin and angles to use in step calculations
	Vector (*GetStepOrigin)(CBaseEntity*); // const
	QAngle (*GetStepAngles)(CBaseEntity*); // const

	bool (*ShouldDrawWaterImpacts)(CBaseEntity*); // { return true; } // in vtable

	// was pev->flags
	//CNetworkVarForDerived(int, m_fFlags);
	void (*NetworkStateChanged_m_fFlags)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_fFlags)(CBaseEntity*, void* pVar); // {} // in vtable

	//CNetworkVarForDerived(unsigned char, m_nWaterLevel);
	void (*NetworkStateChanged_m_nWaterLevel)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_nWaterLevel)(CBaseEntity*, void* pVar); // {} // in vtable

	//CNetworkHandleForDerived(CBaseEntity, m_hGroundEntity);
	void (*NetworkStateChanged_m_hGroundEntity)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_hGroundEntity)(CBaseEntity*, void* pVar); // {} // in vtable

	// Velocity of the thing we're standing on (world space)
	//CNetworkVarForDerived(Vector, m_vecBaseVelocity);
	void (*NetworkStateChanged_m_vecBaseVelocity)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_vecBaseVelocity)(CBaseEntity*, void* pVar); // {} // in vtable

	// was pev->friction
	//CNetworkVarForDerived(float, m_flFriction);
	void (*NetworkStateChanged_m_flFriction)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_flFriction)(CBaseEntity*, void* pVar); // {} // in vtable

	//CNetworkVectorForDerived(m_vecVelocity);
	void (*NetworkStateChanged_m_vecVelocity)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_vecVelocity)(CBaseEntity*, void* pVar); // {} // in vtable

	// was pev->view_ofs ( FIXME:  Move somewhere up the hierarch, CBaseAnimating, etc. )
	//CNetworkVectorForDerived(m_vecViewOffset);
	void (*NetworkStateChanged_m_vecViewOffset)(CBaseEntity*); // {} // in vtable
	void (*NetworkStateChanged_m_vecViewOffset)(CBaseEntity*, void* pVar); // {} // in vtable

	bool (*ShouldBlockNav)(CBaseEntity*); // const { return true; } // in vtable

	bool (*ShouldForceTransmitsForTeam)(CBaseEntity*, int iTeam); // { return false; } // in vtable

	bool (*IsTruceValidForEnt)(CBaseEntity*); // const { return m_bTruceValidForEnt; } // in vtable

#ifdef TF_DLL
	float (*GetDefaultItemChargeMeterValue)(CBaseEntity*); // const { return 100.f; } // in vtable
#endif  // TF_DLL

	bool (*BCanCallVote)(CBaseEntity*); // { return true; } // in vtable
} CBaseEntity_VMT;

// Everything but the VMT is wrong
struct CBaseEntity
{
	CBaseEntity_VMT* vmt;

	void (*m_pfnMoveDone)(CBaseEntity*);
	void (*m_pfnThink)(CBaseEntity*);

	// NOTE: Keep this near vtable so it's in cache with vtable.
	CServerNetworkProperty m_Network;

	// members
	const char* m_iClassname;   // identifier for entity creation and save/restore
	const char* m_iGlobalname;  // identifier for carrying entity across level transitions
	const char* m_iParent;      // the name of the entities parent; linked into m_pParent during Activate()

	int m_iHammerID;  // Hammer unique edit id number


	// was pev->speed
	float m_flSpeed;
	// was pev->renderfx
	//CNetworkVar(unsigned char, m_nRenderFX);
	unsigned char m_nRenderFX;
	// was pev->rendermode
	//CNetworkVar(unsigned char, m_nRenderMode);
	unsigned char m_nRenderMode;
	//CNetworkVar(short, m_nModelIndex);
	short m_nModelIndex;

#ifdef TF_DLL
	//CNetworkArray(int, m_nModelIndexOverrides, MAX_VISION_MODES);  // used to override the base model index on the client if necessary
	int m_nModelIndexOverrides[MAX_VISION_MODES];
#endif

	// was pev->rendercolor
	//CNetworkColor32(m_clrRender);
	color32 m_clrRender;

	// was pev->animtime:  consider moving to CBaseAnimating
	float m_flPrevAnimTime;
	//CNetworkVar(float, m_flAnimTime);  // this is the point in time that the client will interpolate to position,angle,frame,etc.
	float m_flAnimTime;
	//CNetworkVar(float, m_flSimulationTime);
	float m_flSimulationTime;

	//CNetworkVar(int, m_ubInterpolationFrame);
	int m_ubInterpolationFrame;

	int m_nLastThinkTick;

#if !defined(NO_ENTITY_PREDICTION)
	// Certain entities (projectiles) can be created on the client and thus need a matching id number
	//CNetworkVar(CPredictableId, m_PredictableID);
	CPredictableId m_PredictableID;
#endif

	// used so we know when things are no longer touching
	int touchStamp;


	CUtlVector m_aThinkFunctions;

#ifdef _DEBUG
	int m_iCurrentThinkContext;
#endif

	CUtlVector m_ResponseContexts;

	// Map defined context sets
	const char* m_iszResponseContext;


	//CBaseEntity(CBaseEntity&);
	//CBaseEntity* __ptr;

	// was pev->nextthink
	//CNetworkVarForDerived(int, m_nNextThinkTick);
	int m_nNextThinkTick;
	// was pev->effects
	//CNetworkVar(int, m_fEffects);
	int m_fEffects;


	void (*m_pfnTouch)(CBaseEntity*, CBaseEntity* pOther);
	void (*m_pfnUse)(CBaseEntity*, CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void (*m_pfnBlocked)(CBaseEntity*, CBaseEntity* pOther);

	// NOTE: m_pAttributes needs to be set in the leaf class constructor.
	IHasAttributes* m_pAttributes;

	CBaseEntity* m_pLink;  // used for temporary link-list operations.


	// variables promoted from edict_t
	const char* m_target;
	//CNetworkVarForDerived(int, m_iMaxHealth);  // CBaseEntity doesn't care about changes to this variable, but there are derived classes that do.
	int m_iMaxHealth;
	//CNetworkVarForDerived(int, m_iHealth);
	int m_iHealth;

	//CNetworkVarForDerived(char, m_lifeState);
	char m_lifeState;
	//CNetworkVarForDerived(char, m_takedamage);
	char m_takedamage;

	// Damage filtering
	const char* m_iszDamageFilterName;  // The name of the entity to use as our damage filter.
	EHANDLE m_hDamageFilter;            // The entity that controls who can damage us.

	// Debugging / devolopment fields
	int m_debugOverlays;              // For debug only (bitfields)
	TimedOverlay_t* m_pTimedOverlay;  // For debug only

	const char* m_iszVScripts;
	const char* m_iszScriptThinkFunction;
	CScriptScope m_ScriptScope;
	HSCRIPT m_hScriptInstance;
	const char* m_iszScriptId;
	void* m_pScriptModelKeyValues; // CScriptKeyValues


	// Which frame did I simulate?
	int m_nSimulationTick;

	// FIXME: Make this private! Still too many references to do so...
	//CNetworkVar(int, m_spawnflags);
	int m_spawnflags;


	int m_iEFlags;  // entity flags EFL_*
	// was pev->flags
	//CNetworkVarForDerived(int, m_fFlags);
	int m_fFlags;

	const char* m_iName;  // name used to identify this entity

	// Damage modifiers
	//CUtlLinkedList<CDamageModifier*, int> m_DamageModifiers;
	CUtlLinkedList__m_DamageModifiers m_DamageModifiers;

	EHANDLE m_pParent;  // for movement hierarchy
	byte m_nTransmitStateOwnedCounter;
	//CNetworkVar(unsigned char, m_iParentAttachment);  // 0 if we're relative to the parent's absorigin and absangles.
	unsigned char m_iParentAttachment;
	//CNetworkVar(unsigned char, m_MoveType);           // One of the MOVETYPE_ defines.
	unsigned char m_MoveType;
	//CNetworkVar(unsigned char, m_MoveCollide);
	unsigned char m_MoveCollide;

	// Our immediate parent in the movement hierarchy.
	// FIXME: clarify m_pParent vs. m_pMoveParent
	//CNetworkHandle(CBaseEntity, m_hMoveParent);
	CBaseEntity* m_hMoveParent;
	// cached child list
	EHANDLE m_hMoveChild;
	// generated from m_pMoveParent
	EHANDLE m_hMovePeer;

	//CNetworkVarEmbedded(CCollisionProperty, m_Collision);
	CCollisionProperty m_Collision;

	//CNetworkHandle(CBaseEntity, m_hOwnerEntity);   // only used to point to an edict it won't collide with
	CBaseEntity* m_hOwnerEntity;
	//CNetworkHandle(CBaseEntity, m_hEffectEntity);  // Fire/Dissolve entity.
	CBaseEntity* m_hEffectEntity;

	//CNetworkVar(int, m_CollisionGroup);  // used to cull collision tests
	int m_CollisionGroup;
	IPhysicsObject* m_pPhysicsObject;    // pointer to the entity's physics object (vphysics.dll)

	//CNetworkVar(float, m_flShadowCastDistance);
	float m_flShadowCastDistance;
	float m_flDesiredShadowCastDistance;

	// Team handling
	int m_iInitialTeamNum;         // Team number of this entity's team read from file
	//CNetworkVar(int, m_iTeamNum);  // Team number of this entity's team.
	int m_iTeamNum;

	// Sets water type + level for physics objects
	unsigned char m_nWaterTouch;
	unsigned char m_nSlimeTouch;
	unsigned char m_nWaterType;
	//CNetworkVarForDerived(unsigned char, m_nWaterLevel);
	unsigned char m_nWaterLevel;
	float m_flNavIgnoreUntilTime;

	//CNetworkHandleForDerived(CBaseEntity, m_hGroundEntity);
	CBaseEntity* m_hGroundEntity;
	float m_flGroundChangeTime;  // Time that the ground entity changed

	const char* m_ModelName;

	// Velocity of the thing we're standing on (world space)
	//CNetworkVarForDerived(Vector, m_vecBaseVelocity);
	Vector m_vecBaseVelocity;

	// Global velocity
	Vector m_vecAbsVelocity;

	// Local angular velocity
	QAngle m_vecAngVelocity;

	// Global angular velocity
	// QAngle m_vecAbsAngVelocity; // Pre-removed

	// local coordinate frame of entity
	matrix3x4_t m_rgflCoordinateFrame;

	// Physics state
	EHANDLE m_pBlocker;

	// was pev->gravity;
	//CNetworkVar(float, m_flGravity);  // rename to m_flGravityScale;
	float m_flGravity;
	// was pev->friction
	//CNetworkVarForDerived(float, m_flFriction);
	float m_flFriction;
	//CNetworkVar(float, m_flElasticity);
	float m_flElasticity;

	// was pev->ltime
	float m_flLocalTime;
	// local time at the beginning of this frame
	float m_flVPhysicsUpdateLocalTime;
	// local time the movement has ended
	float m_flMoveDoneTime;

	// A counter to help quickly build a list of potentially pushed objects for physics
	int m_nPushEnumCount;

	Vector m_vecAbsOrigin;
	//CNetworkVectorForDerived(m_vecVelocity);
	Vector m_vecVelocity;

	// Adrian
	//CNetworkVar(unsigned char, m_iTextureFrameIndex);
	unsigned char m_iTextureFrameIndex;

	//CNetworkVar(bool, m_bSimulatedEveryTick);
	bool m_bSimulatedEveryTick;
	//CNetworkVar(bool, m_bAnimatedEveryTick);
	bool m_bAnimatedEveryTick;
	//CNetworkVar(bool, m_bAlternateSorting);
	bool m_bAlternateSorting;

	// User outputs. Fired when the "FireInputX" input is triggered.
	COutputEvent m_OnUser1;
	COutputEvent m_OnUser2;
	COutputEvent m_OnUser3;
	COutputEvent m_OnUser4;

	QAngle m_angAbsRotation;

	//CNetworkVector(m_vecOrigin);
	Vector m_vecOrigin;
	//CNetworkQAngle(m_angRotation);
	QAngle m_angRotation;
	CBaseHandle m_RefEHandle;

	// was pev->view_ofs ( FIXME:  Move somewhere up the hierarch, CBaseAnimating, etc. )
	//CNetworkVectorForDerived(m_vecViewOffset);
	Vector m_vecViewOffset;


	// dynamic model state tracking
	bool m_bDynamicModelAllowed;
	bool m_bDynamicModelPending;
	bool m_bDynamicModelSetBounds;

#if !defined(NO_ENTITY_PREDICTION)
	//CNetworkVar(bool, m_bIsPlayerSimulated);
	bool m_bIsPlayerSimulated;
	// Player who is driving my simulation
	CHandle m_hPlayerSimulationOwner;
#endif

	int m_fDataObjectTypes;

	bool m_bForcePurgeFixedupStrings;  // For template entites so we don't leak strings.

	CThreadFastMutex m_CalcAbsolutePositionMutex;

	bool m_bTruceValidForEnt;
};

#endif
