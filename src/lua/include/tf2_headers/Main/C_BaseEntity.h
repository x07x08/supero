#if !defined(MainC_BaseEntity_Included)
#define MainC_BaseEntity_Included

#include "Definitions.h"
#include "Misc/CommonMacros.h"

#include "Interfaces/IVModelInfo.h"
//#include "Interfaces/IVModelRender.h"
#include "Main/IClientEntity.h"
#include "Main/CCollisionProperty.h"
#include "Misc/CInterpolatedVar.h"
#include "Misc/PredictableId.h"
#include "Misc/ParticleProperty.h"
#include "Misc/TakeDamageInfo.h"
#include "Misc/ThreadTools.h"

typedef struct ITraceFilter ITraceFilter;
typedef int RenderGroup_t;
typedef struct CNewParticleEffect CNewParticleEffect;
typedef struct C_BaseCombatWeapon C_BaseCombatWeapon;

typedef struct C_Team C_Team;
typedef struct IPhysicsObject IPhysicsObject;
typedef struct IClientVehicle IClientVehicle;
//typedef struct CPredictionCopy CPredictionCopy;
//typedef struct C_BasePlayer C_BasePlayer;
//typedef struct studiohdr_t studiohdr_t;
typedef struct CStudioHdr CStudioHdr;
typedef struct CDamageModifier CDamageModifier;
//typedef struct IRecipientFilter IRecipientFilter;
//typedef struct CUserCmd CUserCmd;
//typedef struct solid_t solid_t;
typedef struct ISave ISave;
typedef struct IRestore IRestore;
typedef struct C_BaseAnimating C_BaseAnimating;
typedef struct C_AI_BaseNPC C_AI_BaseNPC;
typedef struct EmitSound_t EmitSound_t;
//typedef struct C_RecipientFilter C_RecipientFilter;
//typedef struct CTakeDamageInfo CTakeDamageInfo;
typedef struct C_BaseCombatCharacter C_BaseCombatCharacter;
typedef struct CEntityMapData CEntityMapData;
//typedef struct ConVar ConVar;
typedef struct CDmgAccumulator CDmgAccumulator;
typedef struct IHasAttributes IHasAttributes;

//typedef struct CSoundParameters CSoundParameters;

typedef unsigned int AimEntsListHandle_t;
typedef unsigned int HTOOLHANDLE;

#define INVALID_AIMENTS_LIST_HANDLE (AimEntsListHandle_t) ~0

//extern void RecvProxy_IntToColor32(const CRecvProxyData* pData, void* pStruct, void* pOut);
//extern void RecvProxy_LocalVelocity(const CRecvProxyData* pData, void* pStruct, void* pOut);

typedef enum CollideType_t
{
	ENTITY_SHOULD_NOT_COLLIDE = 0,
	ENTITY_SHOULD_COLLIDE,
	ENTITY_SHOULD_RESPOND
} CollideType_t;

typedef struct VarMapEntry_t
{
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;  // Set to false when this var doesn't
	                                       // need Interpolate() called on it anymore.
	void* data;
	IInterpolatedVar* watcher;
} VarMapEntry_t;

typedef struct VarMapping_t
{
	CUtlVector m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
} VarMapping_t;

#define DECLARE_INTERPOLATION()

// How many data slots to use when in multiplayer.
#define MULTIPLAYER_BACKUP 90

//typedef struct serialentity_t serialentity_t;

//typedef CHandle EHANDLE;  // The client's version of EHANDLE.

typedef struct C_BaseEntity C_BaseEntity;

typedef void (*BASEPTR)(C_BaseEntity*);
typedef void (*ENTITYFUNCPTR)(C_BaseEntity*, C_BaseEntity* pOther);

// For entity creation on the client
typedef C_BaseEntity* (*DISPATCHFUNCTION)(void);

#if !defined(NO_ENTITY_PREDICTION)

typedef struct PredictionContext
{
	bool m_bActive;
	int m_nCreationCommandNumber;
	char const* m_pszCreationModule;
	int m_nCreationLineNumber;
	// The entity to whom we are attached
	CHandle m_hServerEntity;
} PredictionContext;
#endif

typedef struct thinkfunc_t_client
{
	BASEPTR m_pfnThink;
	const char* m_iszContext; // string_t
	int m_nNextThinkTick;
	int m_nLastThinkTick;
} thinkfunc_t_client;

//#define CREATE_PREDICTED_ENTITY(className) \
//	C_BaseEntity::CreatePredictedEntityByName(className, __FILE__, __LINE__);

// Entity flags that only exist on the client.
#define ENTCLIENTFLAG_GETTINGSHADOWRENDERBOUNDS 0x0001  // Tells us if we're getting the real ent render bounds or the shadow render bounds.
#define ENTCLIENTFLAG_DONTUSEIK 0x0002                  // Don't use IK on this entity even if its model has IK.
#define ENTCLIENTFLAG_ALWAYS_INTERPOLATE 0x0004         // Used by view models.

enum
{
	SLOT_ORIGINALDATA = -1,
};

// Toggle the visualization of the entity's abs/bbox
enum
{
	VISUALIZE_COLLISION_BOUNDS = 0x1,
	VISUALIZE_SURROUNDING_BOUNDS = 0x2,
	VISUALIZE_RENDER_BOUNDS = 0x4,
};

// think function handling
typedef enum thinkmethods_t_client
{
	THINK_FIRE_ALL_FUNCTIONS_CLIENT,
	THINK_FIRE_BASE_ONLY_CLIENT,
	THINK_FIRE_ALL_BUT_BASE_CLIENT,
} thinkmethods_t_client;

// Two part guts of Interpolate(). Shared with C_BaseAnimating.
enum
{
	INTERPOLATE_STOP = 0,
	INTERPOLATE_CONTINUE
};

// All virtuals are used or implemented in some way.
// What's unclear is marked with "// in vtable".

// Incorrect
typedef struct C_BaseEntity_VMT
{
	IClientEntity_VMT vmt_IClientEntity;

	DECLARE_DATADESC(C_BaseEntity);
	//DECLARE_CLIENTCLASS(C_BaseEntity); // IClientNetworkable implements it
	int (*YouForgotToImplementOrDeclareClientClass)(C_BaseEntity*);
	//DECLARE_PREDICTABLE();
	datamap_t* (*GetPredDescMap)(C_BaseEntity*); // 15 WIN32

	// FireBullets uses shared code for prediction.
	void (*FireBullets)(C_BaseEntity*, const FireBulletsInfo_t* info);
	void (*ModifyFireBulletsDamage)(C_BaseEntity*, CTakeDamageInfo* dmgInfo); // {} // in vtable
	bool (*ShouldDrawUnderwaterBulletBubbles)(C_BaseEntity*);
	bool (*ShouldDrawWaterImpacts)(C_BaseEntity*); // { return true; } // in vtable
	bool (*HandleShotImpactingWater)(C_BaseEntity*, const FireBulletsInfo_t* info,
	                                      const Vector* vecEnd,
	                                      ITraceFilter* pTraceFilter,
	                                      Vector* pVecTracerDest);
	ITraceFilter* (*GetBeamTraceFilter)(C_BaseEntity*);
	void (*DispatchTraceAttack)(C_BaseEntity*, const CTakeDamageInfo* info, const Vector* vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator);
	void (*TraceAttack)(C_BaseEntity*, const CTakeDamageInfo* info, const Vector* vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator);
	void (*DoImpactEffect)(C_BaseEntity*, trace_t* tr, int nDamageType);
	void (*MakeTracer)(C_BaseEntity*, const Vector* vecTracerSrc, const trace_t* tr, int iTracerType);
	int (*GetTracerAttachment)(C_BaseEntity*);
	int (*BloodColor)(C_BaseEntity*);
	const char* (*GetTracerType)(C_BaseEntity*);

	void (*Spawn)(C_BaseEntity*);
	void (*SpawnClientEntity)(C_BaseEntity*);
	void (*Precache)(C_BaseEntity*);
	void (*Activate)(C_BaseEntity*);

	void (*ParseMapData)(C_BaseEntity*, CEntityMapData* mapData);
	bool (*KeyValueChar)(C_BaseEntity*, const char* szKeyName, const char* szValue);
	bool (*KeyValueFloat)(C_BaseEntity*, const char* szKeyName, float flValue);
	bool (*KeyValueVec)(C_BaseEntity*, const char* szKeyName, const Vector* vecValue);
	bool (*GetKeyValue)(C_BaseEntity*, const char* szKeyName, char* szValue, int iMaxLen);

	// Called by the CLIENTCLASS macros.
	bool (*Init)(C_BaseEntity*, int entnum, int iSerialNum);

	C_BaseAnimating* (*GetBaseAnimating)(C_BaseEntity*); // { return NULL; } // in vtable ??
	void (*SetClassname)(C_BaseEntity*, const char* className);

	// IClientUnknown overrides.
	// Methods of IClientRenderable

	Vector (*GetObserverCamOrigin)(C_BaseEntity*); // { return GetRenderOrigin(); }  // Return the origin for player observers tracking this target // in vtable ??

	bool (*TestCollision)(C_BaseEntity*, const Ray_t* ray, unsigned int fContentsMask, trace_t* tr);
	bool (*TestHitboxes)(C_BaseEntity*, const Ray_t* ray, unsigned int fContentsMask, trace_t* tr);

	// This function returns a value that scales all damage done by this entity.
	// Use CDamageModifier to hook in damage modifiers on a guy.
	float (*GetAttackDamageScale)(C_BaseEntity*);

	// IClientNetworkable implementation.

	void (*ValidateModelIndex)(C_BaseEntity*);

	// pvs info. NOTE: Do not override these!!
	void (*SetDormant)(C_BaseEntity*, bool bDormant);
	//bool (*IsDormant)(C_BaseEntity*);

	int (*GetEFlags)(C_BaseEntity*); // const
	void (*SetEFlags)(C_BaseEntity*, int iEFlags);

	// IClientThinkable.

	bool (*ShouldSavePhysics)(C_BaseEntity*);

	// save/restore stuff
	void (*OnSave)(C_BaseEntity*);
	void (*OnRestore)(C_BaseEntity*);
	// capabilities for save/restore
	int (*ObjectCaps)(C_BaseEntity*);
	// only overload these if you have special data to serialize
	int (*Save)(C_BaseEntity*, ISave* save);
	int (*Restore)(C_BaseEntity*, IRestore* restore);

	// Called after spawn, and in the case of self-managing objects, after load
	bool (*CreateVPhysics)(C_BaseEntity*);

	// destroy and remove the physics object for this entity
	void (*VPhysicsDestroyObject)(C_BaseEntity*);

	// Purpose: My physics object has been updated, react or extract data
	void (*VPhysicsUpdate)(C_BaseEntity*, IPhysicsObject* pPhysics);
	int (*VPhysicsGetObjectList)(C_BaseEntity*, IPhysicsObject** pList, int listMax);
	bool (*VPhysicsIsFlesh)(C_BaseEntity*);

	// IClientEntity implementation.

	// Add entity to visible entities list?
	void (*AddEntity)(C_BaseEntity*);

	const Vector* (*GetPrevLocalOrigin)(C_BaseEntity*); // const
	const QAngle* (*GetPrevLocalAngles)(C_BaseEntity*); // const

	int (*CalcOverrideModelIndex)(C_BaseEntity*); // { return -1; } // in vtable

	// These methods return a *world-aligned* box relative to the absorigin of the entity.
	// This is used for collision purposes and is *not* guaranteed
	// to surround the entire entity's visual representation
	// NOTE: It is illegal to ask for the world-aligned bounds for
	// SOLID_BSP objects
	const Vector* (*WorldAlignMins)(C_BaseEntity*); // const
	const Vector* (*WorldAlignMaxs)(C_BaseEntity*); // const

	// NOTE: These use the collision OBB to compute a reasonable center point for the entity
	const Vector* (*WorldSpaceCenter)(C_BaseEntity*); // const

	// Used when the collision prop is told to ask game code for the world-space surrounding box
	void (*ComputeWorldSpaceSurroundingBox)(C_BaseEntity*, Vector* pVecWorldMins, Vector* pVecWorldMaxs);
	float (*GetHealthBarHeightOffset)(C_BaseEntity*); // const { return 0.f; } // in vtable

	SolidType_t (*GetSolid)(C_BaseEntity*); // const

	int (*GetSolidFlags)(C_BaseEntity*); // const

	// Attachments
	//int (*LookupAttachment)(C_BaseEntity*, const char* pAttachmentName); // { return -1; }
	//bool (*GetAttachmentMatrix)(C_BaseEntity*, int number, matrix3x4_t* matrix);
	bool (*GetAttachment)(C_BaseEntity*, int number, Vector* origin);
	//bool (*GetAttachmentFull)(C_BaseEntity*, int number, Vector* origin, QAngle* angles);
	bool (*GetAttachmentVelocity)(C_BaseEntity*, int number, Vector* originVel, Quaternion* angleVel);

	// Team handling
	C_Team* (*GetTeam)(C_BaseEntity*); // const
	int (*GetTeamNumber)(C_BaseEntity*); // const
	void (*ChangeTeam)(C_BaseEntity*, int iTeamNum);  // Assign this entity to a team.
	int (*GetRenderTeamNumber)(C_BaseEntity*);
	bool (*InSameTeam)(C_BaseEntity*, const C_BaseEntity* pEntity); // const  // Returns true if the specified entity is on the same team as this one
	bool (*InLocalTeam)(C_BaseEntity*);

	// ID Target handling
	bool (*IsValidIDTarget)(C_BaseEntity*); // { return false; } // in vtable
	const char* (*GetIDString)(C_BaseEntity*); // in vtable

	// See CSoundEmitterSystem
	void (*ModifyEmitSoundParams)(C_BaseEntity*, EmitSound_t* params);

	// This can be used to setup the entity as a client-only entity.
	// Override this to perform per-entity clientside setup
	bool (*InitializeAsClientEntity)(C_BaseEntity*, const char* pszModelName, RenderGroup_t renderGroup);

	// This function gets called on all client entities once per simulation phase.
	// It dispatches events like OnDataChanged(), and calls the legacy function AddEntity().
	void (*Simulate)(C_BaseEntity*);

	// If this is a vehicle, returns the vehicle interface
	IClientVehicle* (*GetClientVehicle)(C_BaseEntity*); // { return NULL; } // in vtable

	// Returns the aiment render origin + angles
	void (*GetAimEntOrigin)(C_BaseEntity*, IClientEntity* pAttachedTo, Vector* pAbsOrigin, QAngle* pAbsAngles);

	// get network origin from previous update
	const Vector* (*GetOldOrigin)(C_BaseEntity*);

	RenderGroup_t (*GetRenderGroup)(C_BaseEntity*);

	void (*GetToolRecordingState)(C_BaseEntity*, KeyValues* msg);
	void (*CleanupToolRecordingState)(C_BaseEntity*, KeyValues* msg);

	// The value returned by here determines whether or not (and how) the entity
	// is put into the spatial partition.
	CollideType_t (*GetCollideType)(C_BaseEntity*);

	void (*UpdateVisibility)(C_BaseEntity*);

	// Returns true if the entity changes its position every frame on the server but it doesn't
	// set animtime. In that case, the client returns true here so it copies the server time to
	// animtime in OnDataChanged and the position history is correct for interpolation.
	bool (*IsSelfAnimating)(C_BaseEntity*);

	// Set appropriate flags and store off data when these fields are about to change
	void (*OnLatchInterpolatedVariables)(C_BaseEntity*, int flags);

	// Initialize things given a new model.
	CStudioHdr* (*OnNewModel)(C_BaseEntity*);
	void (*OnNewParticleEffect)(C_BaseEntity*, const char* pszParticleName, CNewParticleEffect* pNewParticleEffect);

	void (*ResetLatched)(C_BaseEntity*);

	// Interpolate the position for rendering
	bool (*Interpolate)(C_BaseEntity*, float currentTime);

	// Is this a submodel of the world ( *1 etc. in name ) ( brush models only )
	bool (*IsSubModel)(C_BaseEntity*);
	// Deal with EF_* flags
	void (*CreateLightEffects)(C_BaseEntity*);

	// Reset internal fields
	void (*Clear)(C_BaseEntity*);
	// Helper to draw raw brush models
	int (*DrawBrushModel)(C_BaseEntity*, bool bTranslucent, int nFlags, bool bTwoPass);

	// returns the material animation start time
	float (*GetTextureAnimationStartTime)(C_BaseEntity*);
	// Indicates that a texture animation has wrapped
	void (*TextureAnimationWrapped)(C_BaseEntity*);

	// Set the next think time. Pass in CLIENT_THINK_ALWAYS to have Think() called each frame.
	void (*SetNextClientThink)(C_BaseEntity*, float nextThinkTime);

	// anything that has health can override this...
	void (*SetHealth)(C_BaseEntity*, int iHealth); // {} // in vtable
	int (*GetHealth)(C_BaseEntity*); // const { return 0; } // in vtable
	int (*GetMaxHealth)(C_BaseEntity*); // const { return 1; } // in vtable
	bool (*IsVisibleToTargetID)(C_BaseEntity*); // const { return false; } // in vtable
	bool (*IsHealthBarVisible)(C_BaseEntity*); // const { return false; } // in vtable

	// A method to apply a decal to an entity
	void (*AddDecal)(C_BaseEntity*, const Vector* rayStart, const Vector* rayEnd, const Vector* decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t* tr, int maxLODToDecal);

	void (*AddColoredDecal)(C_BaseEntity*, const Vector* rayStart, const Vector* rayEnd, const Vector* decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t* tr, Color_t cColor, int maxLODToDecal);

	bool (*IsClientCreated)(C_BaseEntity*); // const

	void (*UpdateOnRemove)(C_BaseEntity*);

	void (*SUB_Remove)(C_BaseEntity*);

	void (*SetPredictable)(C_BaseEntity*, bool state);
	bool (*PredictionErrorShouldResetLatchedForAllPredictables)(C_BaseEntity*); // { return true; }  // legacy behavior is that any prediction error causes all predictables to reset latched // in vtable

	int (*RestoreData)(C_BaseEntity*, const char* context, int slot, int type);

	char const* (*DamageDecal)(C_BaseEntity*, int bitsDamageType, int gameMaterial);
	void (*DecalTrace)(C_BaseEntity*, trace_t* pTrace, char const* decalName);
	void (*ImpactTrace)(C_BaseEntity*, trace_t* pTrace, int iDamageType, const char* pCustomImpactName);

	bool (*ShouldPredict)(C_BaseEntity*); // in vtable
	// interface function pointers
	void (*Think)(C_BaseEntity*); // in vtable

	// Sorry folks, here lies TF2-specific stuff that really has no other place to go
	bool (*CanBePoweredUp)(C_BaseEntity*); // { return false; } // in vtable
	bool (*AttemptToPowerup)(C_BaseEntity*, int iPowerup, float flTime, float flAmount, C_BaseEntity* pAttacker, CDamageModifier* pDamageModifier); // { return false; } // in vtable

	bool (*IsCurrentlyTouching)(C_BaseEntity*); // const

	void (*StartTouch)(C_BaseEntity*, C_BaseEntity* pOther);
	void (*Touch)(C_BaseEntity*, C_BaseEntity* pOther);
	void (*EndTouch)(C_BaseEntity*, C_BaseEntity* pOther);

	unsigned int (*PhysicsSolidMaskForEntity)(C_BaseEntity*); // const

	void (*PhysicsSimulate)(C_BaseEntity*);
	bool (*IsAlive)(C_BaseEntity*);

	bool (*IsPlayer)(C_BaseEntity*); // in vtable
	bool (*IsBaseCombatCharacter)(C_BaseEntity*); // in vtable
	C_BaseCombatCharacter* (*MyCombatCharacterPointer)(C_BaseEntity*); // { return NULL; } // in vtable
	bool (*IsNPC)(C_BaseEntity*); // { return false; } // in vtable
	bool (*IsNextBot)(C_BaseEntity*); // { return false; } // in vtable
	// TF2 specific
	bool (*IsBaseObject)(C_BaseEntity*); // const { return false; } // in vtable
	bool (*IsBaseCombatWeapon)(C_BaseEntity*); // const { return false; } // in vtable
	C_BaseCombatWeapon* (*MyCombatWeaponPointer)(C_BaseEntity*); // { return NULL; } // in vtable
	bool (*IsCombatItem)(C_BaseEntity*); // const { return false; } // in vtable

	bool (*IsBaseTrain)(C_BaseEntity*); // const { return false; } // in vtable

	// Returns the eye point + angles (used for viewing + shooting)
	Vector (*EyePosition)(C_BaseEntity*);
	const QAngle* (*EyeAngles)(C_BaseEntity*);       // Direction of eyes
	const QAngle* (*LocalEyeAngles)(C_BaseEntity*);  // Direction of eyes in local space (pl.v_angle)

	// position of ears
	Vector (*EarPosition)(C_BaseEntity*);

	// Called by physics to see if we should avoid a collision test....
	bool (*ShouldCollide)(C_BaseEntity*, int collisionGroup, int contentsMask); // const

	const Vector* (*GetViewOffset)(C_BaseEntity*); // const
	void (*SetViewOffset)(C_BaseEntity*, const Vector* v);

	void (*ClientAdjustStartSoundParams)(C_BaseEntity*, EmitSound_t* params); // {} // in vtable
	void (*ClientAdjustStartSoundParamsEx)(C_BaseEntity*, StartSoundParams_t* params); // {} // in vtable

	// overrideable rules if an entity should interpolate
	bool (*ShouldInterpolate)(C_BaseEntity*);

	// Allow entities to perform client-side fades
	unsigned char (*GetClientSideFade)(C_BaseEntity*); // { return 255; } // in vtable

	// Bloat the culling bbox past the parent ent's bbox in local space if EF_BONEMERGE_FASTCULL is set.
	void (*BoneMergeFastCullBloat)(C_BaseEntity*, Vector* localMins, Vector* localMaxs, const Vector* thisEntityMins, const Vector* thisEntityMaxs); // const

	// Called after predicted entity has been acknowledged so that no longer needed entity can
	//  be deleted
	// Return true to force deletion right now, regardless of isbeingremoved
	bool (*OnPredictedEntityRemove)(C_BaseEntity*, bool isbeingremoved, C_BaseEntity* predicted);

	C_BaseEntity* (*GetShadowUseOtherEntity)(C_BaseEntity*); // const
	void (*SetShadowUseOtherEntity)(C_BaseEntity*, C_BaseEntity* pEntity);

	bool (*AddRagdollToFadeQueue)(C_BaseEntity*); // { return true; } // in vtable

	bool (*IsDeflectable)(C_BaseEntity*); // { return false; } // in vtable

	// Allow studio models to tell us what their m_nBody value is
	int (*GetStudioBody)(C_BaseEntity*); // { return 0; } // in vtable

	// Implement this if you use MOVETYPE_CUSTOM
	void (*PerformCustomPhysics)(C_BaseEntity*, Vector* pNewPosition, Vector* pNewVelocity, QAngle* pNewAngles, QAngle* pNewAngVelocity);

#ifdef TF_CLIENT_DLL
	// TF prevents drawing of any entity attached to players that aren't items in the inventory of the player.
	// This is to prevent servers creating fake cosmetic items and attaching them to players.

	bool (*ValidateEntityAttachedToPlayer)(C_BaseEntity*, bool* bShouldRetry);
#endif
} C_BaseEntity_VMT;

// Everything but the VMT is wrong
struct C_BaseEntity
{
	C_BaseEntity_VMT* vmt;

	const char* m_iClassname;


	VarMapping_t m_VarMap;


	// interface function pointers
	void (*m_pfnThink)(C_BaseEntity*);
	void (*m_pfnTouch)(C_BaseEntity*, C_BaseEntity* pOther);


	// NOTE: m_pAttributes needs to be set in the leaf class constructor.
	IHasAttributes* m_pAttributes;


	// Determine what entity this corresponds to
	int index;

	// Render information
	unsigned char m_nRenderFX;
	unsigned char m_nRenderFXBlend;

	// Entity flags that are only for the client (ENTCLIENTFLAG_ defines).
	unsigned short m_EntClientFlags;

	//CNetworkColor32(m_clrRender);
	color32 m_clrRender;

	// Model for rendering
	const model_t* model;

	// Time animation sequence or frame was last changed
	float m_flAnimTime;
	float m_flOldAnimTime;

	float m_flSimulationTime;
	float m_flOldSimulationTime;

	float m_flCreateTime;

	byte m_ubInterpolationFrame;
	byte m_ubOldInterpolationFrame;

	// Effects to apply
	int m_fEffects;
	unsigned char m_nRenderMode;
	unsigned char m_nOldRenderMode;

	// Used to store the state we were added to the BSP as, so it can
	// reinsert the entity if the state changes.
	ClientRenderHandle_t m_hRender;  // link into spatial partition

	// Interpolation says don't draw yet
	bool m_bReadyToDraw;


	int m_nNextThinkTick;
	int m_nLastThinkTick;

	// Object model index
	short m_nModelIndex;

#ifdef TF_CLIENT_DLL
	int m_nModelIndexOverrides[MAX_VISION_MODES];
#endif

	char m_takedamage;
	char m_lifeState;

	int m_iHealth;

	// was pev->speed
	float m_flSpeed;

	// Team Handling
	int m_iTeamNum;

#if !defined(NO_ENTITY_PREDICTION)
	// Certain entities (projectiles) can be created on the client
	CPredictableId m_PredictableID;
	PredictionContext* m_pPredictionContext;
#endif

	// used so we know when things are no longer touching
	int touchStamp;

	int m_nFXComputeFrame;

	// FIXME: Should I move the functions handling these out of C_ClientEntity
	// and into C_BaseEntity? Then we could make these private.
	// Client handle
	CBaseHandle m_RefEHandle;  // Reference ehandle. Used to generate ehandles off this entity.

	// Set by tools if this entity should route "info" to various tools listening to HTOOLENTITIES
#ifndef NO_TOOLFRAMEWORK
	bool m_bEnabledInToolView;
	bool m_bToolRecording;
	HTOOLHANDLE m_ToolHandle;
	int m_nLastRecordedFrame;
	bool m_bRecordInTools;  // should this entity be recorded in the tools (we exclude some things like models for menus)
#endif

	// pointer to the entity's physics object (vphysics.dll)
	IPhysicsObject* m_pPhysicsObject;

#if !defined(NO_ENTITY_PREDICTION)
	bool m_bPredictionEligible;
#endif

	int m_nSimulationTick;

	// Think contexts
	//int GetIndexForThinkContext(const char* pszContext);
	CUtlVector m_aThinkFunctions;
	int m_iCurrentThinkContext;

	// Object eye position
	Vector m_vecViewOffset;

#if defined(SIXENSE)
	Vector m_vecEyeOffset;
	QAngle m_EyeAngleOffset;
#endif

	// Object velocity
	Vector m_vecVelocity;
	CInterpolatedVar m_iv_vecVelocity;

	Vector m_vecAbsVelocity;

	// was pev->avelocity
	QAngle m_vecAngVelocity;

	//QAngle m_vecAbsAngVelocity; // Pre-removed

#if !defined(NO_ENTITY_PREDICTION)
	// It's still in the list for "fixup purposes" and simulation, but don't try to render it any more...
	bool m_bDormantPredictable;

	// So we can clean it up
	int m_nIncomingPacketEntityBecameDormant;
#endif

	// The spawn time of the entity
	float m_flSpawnTime;

	// Timestamp of message arrival
	float m_flLastMessageTime;

	// Base velocity
	Vector m_vecBaseVelocity;

	// Gravity multiplier
	float m_flGravity;

	// Model instance data..
	ModelInstanceHandle_t m_ModelInstance;

	// Shadow data
	ClientShadowHandle_t m_ShadowHandle;

	// A random value used by material proxies for each model instance.
	float m_flProxyRandomValue;

	ClientThinkHandle_t m_hThink;

	int m_iEFlags;  // entity flags EFL_*

	// Object movetype
	unsigned char m_MoveType;
	unsigned char m_MoveCollide;
	unsigned char m_iParentAttachment;  // 0 if we're relative to the parent's absorigin and absangles.
	unsigned char m_iOldParentAttachment;

	unsigned char m_nWaterLevel;
	unsigned char m_nWaterType;
	// For client/server entities, true if the entity goes outside the PVS.
	// Unused for client only entities.
	bool m_bDormant;
	// Prediction system
	bool m_bPredictable;

	// Hierarchy
	CHandle m_pMoveParent;
	CHandle m_pMoveChild;
	CHandle m_pMovePeer;
	CHandle m_pMovePrevPeer;

	// The moveparent received from networking data
	CHandle m_hNetworkMoveParent;
	CHandle m_hOldMoveParent;

	const char* m_ModelName;

	//CNetworkVarEmbedded(CCollisionProperty, m_Collision);
	CCollisionProperty m_Collision;
	//CNetworkVarEmbedded(CParticleProperty, m_Particles);
	CParticleProperty m_Particles;

	// Physics state
	float m_flElasticity;

	float m_flShadowCastDistance;
	EHANDLE m_ShadowDirUseOtherEntity;

	EHANDLE m_hGroundEntity;
	float m_flGroundChangeTime;

	// Friction.
	float m_flFriction;

	Vector m_vecAbsOrigin;

	// Object orientation
	QAngle m_angAbsRotation;

	Vector m_vecOldOrigin;
	QAngle m_vecOldAngRotation;

	Vector m_vecOrigin;
	CInterpolatedVar m_iv_vecOrigin;
	QAngle m_angRotation;
	CInterpolatedVar m_iv_angRotation;

	// Specifies the entity-to-world transform
	matrix3x4_t m_rgflCoordinateFrame;

	// Last values to come over the wire. Used for interpolation.
	Vector m_vecNetworkOrigin;
	QAngle m_angNetworkAngles;

	// Behavior flags
	int m_fFlags;

	// used to cull collision tests
	int m_CollisionGroup;

#if !defined(NO_ENTITY_PREDICTION)
	// For storing prediction results and pristine network state
	byte* m_pIntermediateData[MULTIPLAYER_BACKUP];
	byte* m_pOriginalData;
	int m_nIntermediateDataCount;

	bool m_bIsPlayerSimulated;
#endif

	//CNetworkVar(bool, m_bSimulatedEveryTick);
	bool m_bSimulatedEveryTick;
	//CNetworkVar(bool, m_bAnimatedEveryTick);
	bool m_bAnimatedEveryTick;
	//CNetworkVar(bool, m_bAlternateSorting);
	bool m_bAlternateSorting;

	// Adrian
	unsigned char m_iTextureFrameIndex;

	// Bbox visualization
	unsigned char m_fBBoxVisFlags;

	// The list that holds OnDataChanged events uses this to make sure we don't get multiple
	// OnDataChanged calls in the same frame if the client receives multiple packets.
	int m_DataChangeEventRef;

#if !defined(NO_ENTITY_PREDICTION)
	// Player who is driving my simulation
	CHandle m_hPlayerSimulationOwner;
#endif

	// The owner!
	EHANDLE m_hOwnerEntity;
	EHANDLE m_hEffectEntity;

	int m_fDataObjectTypes;

	AimEntsListHandle_t m_AimEntsListHandle;
	int m_nCreationTick;

	float m_fRenderingClipPlane[4];    // world space clip plane when drawing
	bool m_bEnableRenderingClipPlane;  // true to use the custom clip plane when drawing

	unsigned short m_InterpolationListEntry;  // Entry into g_InterpolationList (or g_InterpolationList.InvalidIndex if not in the list).

	unsigned short m_TeleportListEntry;

	CThreadFastMutex m_CalcAbsolutePositionMutex;
	CThreadFastMutex m_CalcAbsoluteVelocityMutex;

#ifdef TF_CLIENT_DLL
	bool m_bValidatedOwner;
	bool m_bDeemedInvalid;
	bool m_bWasDeemedInvalid;
	RenderMode_t m_PreviousRenderMode;
	color32 m_PreviousRenderColor;
#endif

	bool m_bOldShouldDraw;
};

#endif