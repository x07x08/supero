#if !defined(IntfIGameMovement_Included)
#define IntfIGameMovement_Included

#include "Main/CBaseHandle.h"
#include "Definitions.h"

typedef struct CTFPlayer CTFPlayer;
typedef CBaseHandle EntityHandle_t;
typedef struct trace_t trace_t;

typedef struct CMoveData
{
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	EntityHandle_t m_nPlayerHandle;
	int m_nImpulseCommand;
	QAngle m_vecViewAngles;
	QAngle m_vecAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flOldForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector m_vecVelocity;
	QAngle m_vecAngles;
	QAngle m_vecOldAngles;
	float m_outStepHeight;
	Vector m_outWishVel;
	Vector m_outJumpVel;
	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	Vector m_vecAbsOrigin;
} CMoveData;

typedef struct CBasePlayer CBasePlayer;

typedef struct IGameMovement IGameMovement;

typedef struct IGameMovement_VMT
{
	//virtual ~IGameMovement(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*ProcessMovement)(IGameMovement*, CBasePlayer* pPlayer, CMoveData* pMove);
	void (*StartTrackPredictionErrors)(IGameMovement*, CBasePlayer* pPlayer);
	void (*FinishTrackPredictionErrors)(IGameMovement*, CBasePlayer* pPlayer);
	void (*DiffPrint)(IGameMovement*, char const* fmt, ...);
	Vector (*GetPlayerMins)(IGameMovement*, bool ducked);
	Vector (*GetPlayerMaxs)(IGameMovement*, bool ducked);
	Vector (*GetPlayerViewOffset)(IGameMovement*, bool ducked);
} IGameMovement_VMT;

struct IGameMovement
{
	IGameMovement_VMT* vmt;
};

typedef enum IntervalType_t
{
	CGameMovement_GROUND = 0,
	CGameMovement_STUCK,
	CGameMovement_LADDER
} IntervalType_t;

enum
{
	MAX_PC_CACHE_SLOTS = 3,
};

typedef struct CGameMovement CGameMovement;

typedef struct CGameMovement_VMT
{
	IGameMovement_VMT vmt_IGameMovement;

	//virtual ~CGameMovement(void) {}
	void (*TracePlayerBBox)(CGameMovement*, const Vector* start, const Vector* end, unsigned int fMask, int collisionGroup, trace_t* pm);
	void (*TryTouchGround)(CGameMovement*, const Vector* start, const Vector* end, const Vector* mins, const Vector* maxs, unsigned int fMask, int collisionGroup, trace_t* pm);
	unsigned int (*PlayerSolidMask)(CGameMovement*, bool brushOnly);
	void (*PlayerMove)(CGameMovement*);
	float (*CalcRoll)(CGameMovement*, const QAngle* angles, const Vector* velocity, float rollangle, float rollspeed);
	void (*DecayPunchAngle)(CGameMovement*);
	void (*CheckWaterJump)(CGameMovement*);
	void (*WaterMove)(CGameMovement*);
	void (*AirAccelerate)(CGameMovement*, Vector* wishdir, float wishspeed, float accel);
	void (*AirMove)(CGameMovement*);
	float (*GetAirSpeedCap)(CGameMovement*);
	bool (*CanAccelerate)(CGameMovement*);
	void (*Accelerate)(CGameMovement*, Vector* wishdir, float wishspeed, float accel);
	void (*WalkMove)(CGameMovement*);
	void (*FullWalkMove)(CGameMovement*);
	void (*OnJump)(CGameMovement*, float fImpulse);
	void (*OnLand)(CGameMovement*, float fVelocity);
	void (*OnTryPlayerMoveCollision)(CGameMovement*, trace_t* tr);
	Vector (*GetPlayerMins)(CGameMovement*); // const
	Vector (*GetPlayerMaxs)(CGameMovement*); // const
	int (*GetCheckInterval)(CGameMovement*, IntervalType_t type);
	bool (*CheckJumpButton)(CGameMovement*);
	void (*FullTossMove)(CGameMovement*);
	void (*FullLadderMove)(CGameMovement*);
	int (*TryPlayerMove)(CGameMovement*, Vector* pFirstDest, trace_t* pFirstTrace, float flSlideMultiplier);
	bool (*LadderMove)(CGameMovement*);
	bool (*OnLadder)(CGameMovement*, trace_t* trace);
	float (*LadderDistance)(CGameMovement*); // const
	unsigned int (*LadderMask)(CGameMovement*); // const
	float (*ClimbSpeed)(CGameMovement*); // const
	float (*LadderLateralMultiplier)(CGameMovement*); // const
	int (*CheckStuck)(CGameMovement*);
	bool (*CheckWater)(CGameMovement*);
	void (*CategorizePosition)(CGameMovement*);
	void (*CheckParameters)(CGameMovement*);
	void (*ReduceTimers)(CGameMovement*);
	void (*CheckFalling)(CGameMovement*);
	void (*PlayerRoughLandingEffects)(CGameMovement*, float fvol);
	void (*Duck)(CGameMovement*);
	void (*HandleDuckingSpeedCrop)(CGameMovement*);
	void (*FinishUnDuck)(CGameMovement*);
	void (*FinishDuck)(CGameMovement*);
	bool (*CanUnduck)(CGameMovement*);
	CBaseHandle (*TestPlayerPosition)(CGameMovement*, const Vector* pos, int collisionGroup, trace_t* pm);
	void (*SetGroundEntity)(CGameMovement*, trace_t* pm);
	void (*StepMove)(CGameMovement*, Vector* vecDestination, trace_t* trace);
	bool (*GameHasLadders)(CGameMovement*); // const
} CGameMovement_VMT;

struct CGameMovement
{
	CGameMovement_VMT* vmt;

	CBasePlayer* player;
	CMoveData* mv;
	int m_nOldWaterLevel;
	float m_flWaterEntryTime;
	int m_nOnLadder;
	Vector m_vecForward;
	Vector m_vecRight;
	Vector m_vecUp;
	int m_CachedGetPointContents[MAX_PLAYERS_ARRAY_SAFE][MAX_PC_CACHE_SLOTS];
	Vector m_CachedGetPointContentsPoint[MAX_PLAYERS_ARRAY_SAFE][MAX_PC_CACHE_SLOTS];
	Vector m_vecProximityMins;
	Vector m_vecProximityMaxs;
	float m_fFrameTime;
	int m_iSpeedCropped;
	float m_flStuckCheckTime[MAX_PLAYERS_ARRAY_SAFE][2];
};

typedef struct CGameMovementInherited
{
	CBasePlayer* player;
	CMoveData* mv;
	int m_nOldWaterLevel;
	float m_flWaterEntryTime;
	int m_nOnLadder;
	Vector m_vecForward;
	Vector m_vecRight;
	Vector m_vecUp;
	int m_CachedGetPointContents[MAX_PLAYERS_ARRAY_SAFE][MAX_PC_CACHE_SLOTS];
	Vector m_CachedGetPointContentsPoint[MAX_PLAYERS_ARRAY_SAFE][MAX_PC_CACHE_SLOTS];
	Vector m_vecProximityMins;
	Vector m_vecProximityMaxs;
	float m_fFrameTime;
	int m_iSpeedCropped;
	float m_flStuckCheckTime[MAX_PLAYERS_ARRAY_SAFE][2];
} CGameMovementInherited;

typedef CGameMovement_VMT CTFGameMovement_VMT;

typedef struct CTFGameMovement CTFGameMovement;

struct CTFGameMovement
{
	CTFGameMovement_VMT* vmt;

	CGameMovementInherited m_CGameMovement;

	Vector m_vecWaterPoint;
	CTFPlayer* m_pTFPlayer;
	bool m_isPassingThroughEnemies;
};

#define CTFGameMovement_REFL "CTFGameMovement"
#define CTFGameMovement_SIGNATURE "GameMovement001"
#define CTFGameMovement_MODULE "client"

#endif
