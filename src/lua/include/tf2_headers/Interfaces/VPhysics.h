#if !defined(IntfVPhysics_Included)
#define IntfVPhysics_Included

#include "Misc/IAppSystem.h"
#include "Types.h"
#include "Misc/CommonMacros.h"

#define k_flMaxVelocity 2000.0f
#define k_flMaxAngularVelocity 360.0f * 10.0f
#define DEFAULT_MIN_FRICTION_MASS 10.0f
#define DEFAULT_MAX_FRICTION_MASS 2500.0f

typedef struct vcollide_t vcollide_t;

typedef struct IPhysCollide IPhysCollide;
typedef struct CPhysCollide CPhysCollide;
typedef struct IPhysicsObject IPhysicsObject;
typedef struct IVPhysicsDebugOverlay IVPhysicsDebugOverlay;
typedef struct IPhysics IPhysics;
typedef struct IPhysicsCollision IPhysicsCollision;
typedef struct IPhysicsGameTrace IPhysicsGameTrace;
typedef struct IPhysicsCollisionData IPhysicsCollisionData;
typedef struct IPhysicsCollisionEvent IPhysicsCollisionEvent;
typedef struct IPhysicsObjectEvent IPhysicsObjectEvent;
typedef struct IPhysicsConstraintEvent IPhysicsConstraintEvent;
typedef struct IMotionEvent IMotionEvent;
typedef struct IPhysicsCollisionSolver IPhysicsCollisionSolver;
typedef struct IPhysicsTraceFilter IPhysicsTraceFilter;

enum
{
	COLLIDE_POLY = 0,
	COLLIDE_MOPP = 1,
	COLLIDE_BALL = 2,
	COLLIDE_VIRTUAL = 3,
};

typedef struct IPhysCollide_VMT
{
	//virtual ~IPhysCollide() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	// virtual void AddReference();
	// virtual void ReleaseReference();

	// get a surface manager
	void* (*CreateSurfaceManager)(IPhysCollide*, short*); // const
	void (*GetAllLedges)(IPhysCollide*, void* ledges); // const
	unsigned int (*GetSerializationSize)(IPhysCollide*); // const
	unsigned int (*SerializeToBuffer)(IPhysCollide*, char* pDest, bool bSwap); // const
	int (*GetVCollideIndex)(IPhysCollide*); // const
	Vec3 (*GetMassCenter)(IPhysCollide*); // const
	void (*SetMassCenter)(IPhysCollide*, const Vec3* massCenter);
	Vec3 (*GetOrthographicAreas)(IPhysCollide*); // const
	void (*SetOrthographicAreas)(IPhysCollide*, const Vec3* areas);
	float (*GetSphereRadius)(IPhysCollide*); // const
	void (*OutputDebugInfo)(IPhysCollide*); // const
} IPhysCollide_VMT;

struct IPhysCollide
{
	IPhysCollide_VMT* vmt;
};

#define LEAFMAP_HAS_CUBEMAP 0x0001
#define LEAFMAP_HAS_SINGLE_VERTEX_SPAN 0x0002
#define LEAFMAP_HAS_MULTIPLE_VERTEX_SPANS 0x0004

typedef struct leafmap_t
{
	void* pLeaf;
	unsigned short vertCount;
	byte flags;
	byte spanCount;
	unsigned short startVert[8];
} leafmap_t;

typedef struct collidemap_t
{
	int leafCount;
	leafmap_t leafmap[1];
} collidemap_t;

typedef struct CPhysCollide_VMT
{
	IPhysCollide_VMT vmt_IPhysCollide;
	//static CPhysCollide* UnserializeFromBuffer(const char* pBuffer, unsigned int size, int index, bool swap = false);
	//virtual const void* GetCompactSurface() const { return NULL; }
	//virtual Vec3 GetOrthographicAreas() const { return Vec3(1, 1, 1); }
	//virtual float GetSphereRadius() const { return 0; }
	//virtual void ComputeOrthographicAreas(float epsilon) {}
	//virtual void SetOrthographicAreas(const Vec3& areas) {}
	//virtual const collidemap_t* GetCollideMap() const { return NULL; }
} CPhysCollide_VMT;

struct CPhysCollide
{
	CPhysCollide_VMT* vmt;
};

// ------------------------------------------------------------------------------------
// UNITS:
// ------------------------------------------------------------------------------------
// NOTE:  Coordinates are in HL units.  1 unit == 1 inch.  X is east (forward), Y is north (left), Z is up (up)
// QAngle are pitch (around y), Yaw (around Z), Roll (around X)
// AngularImpulse are exponetial maps (an axis in HL units scaled by a "twist" angle in degrees)
//		They can be transformed like normals/covectors and added linearly
// mass is kg, volume is in^3, acceleration is in/s^2, velocity is in/s

// density is kg/m^3 (water ~= 998 at room temperature)
// preferably, these would be in kg/in^3, but the range of those numbers makes them not very human readable
// having water be about 1000 is really convenient for data entry.
// Since volume is in in^3 and density is in kg/m^3:
//	density = (mass / volume) * CUBIC_METERS_PER_CUBIC_INCH
// Force is applied using impulses (kg*in/s)
// Torque is applied using impulses (kg*degrees/s)
// ------------------------------------------------------------------------------------

#define METERS_PER_INCH (0.0254f)
#define CUBIC_METERS_PER_CUBIC_INCH (METERS_PER_INCH * METERS_PER_INCH * METERS_PER_INCH)
// 2.2 lbs / kg
#define POUNDS_PER_KG (2.2f)
#define KG_PER_POUND (1.0f / POUNDS_PER_KG)

// convert from pounds to kg
#define lbs2kg(x) ((x) * KG_PER_POUND)
#define kg2lbs(x) ((x) * POUNDS_PER_KG)

//const float VPHYSICS_MIN_MASS = 0.1f;
//const float VPHYSICS_MAX_MASS = 5e4f;

typedef struct IPhysicsShadowController IPhysicsShadowController;

typedef struct IPhysicsObject_VMT
{
	//virtual ~IPhysicsObject(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	bool (*IsStatic)(IPhysicsObject*); // const
	bool (*IsAsleep)(IPhysicsObject*); // const
	bool (*IsTrigger)(IPhysicsObject*); // const
	bool (*IsFluid)(IPhysicsObject*); // const
	bool (*IsHinged)(IPhysicsObject*); // const
	bool (*IsCollisionEnabled)(IPhysicsObject*); // const
	bool (*IsGravityEnabled)(IPhysicsObject*); // const
	bool (*IsDragEnabled)(IPhysicsObject*); // const
	bool (*IsMotionEnabled)(IPhysicsObject*); // const
	bool (*IsMoveable)(IPhysicsObject*); // const
	bool (*IsAttachedToConstraint)(IPhysicsObject*, bool bExternalOnly); // const
	void (*EnableCollisions)(IPhysicsObject*, bool enable);
	void (*EnableGravity)(IPhysicsObject*, bool enable);
	void (*EnableDrag)(IPhysicsObject*, bool enable);
	void (*EnableMotion)(IPhysicsObject*, bool enable);
	void (*SetGameData)(IPhysicsObject*, void* pGameData);
	void* (*GetGameData)(IPhysicsObject*); // const
	void (*SetGameFlags)(IPhysicsObject*, unsigned short userFlags);
	unsigned short (*GetGameFlags)(IPhysicsObject*); // const
	void (*SetGameIndex)(IPhysicsObject*, unsigned short gameIndex);
	unsigned short (*GetGameIndex)(IPhysicsObject*); // const
	void (*SetCallbackFlags)(IPhysicsObject*, unsigned short callbackflags);
	unsigned short (*GetCallbackFlags)(IPhysicsObject*); // const
	void (*Wake)(IPhysicsObject*);
	void (*Sleep)(IPhysicsObject*);
	void (*RecheckCollisionFilter)(IPhysicsObject*);
	void (*RecheckContactPoints)(IPhysicsObject*);
	void (*SetMass)(IPhysicsObject*, float mass);
	float (*GetMass)(IPhysicsObject*); // const
	float (*GetInvMass)(IPhysicsObject*); // const
	Vec3 (*GetInertia)(IPhysicsObject*); // const
	Vec3 (*GetInvInertia)(IPhysicsObject*); // const
	void (*SetInertia)(IPhysicsObject*, const Vec3* inertia);
	void (*SetDamping)(IPhysicsObject*, const float* speed, const float* rot);
	void (*GetDamping)(IPhysicsObject*, float* speed, float* rot); // const
	void (*SetDragCoefficient)(IPhysicsObject*, float* pDrag, float* pAngularDrag);
	void (*SetBuoyancyRatio)(IPhysicsObject*, float ratio);
	int (*GetMaterialIndex)(IPhysicsObject*); // const
	void (*SetMaterialIndex)(IPhysicsObject*, int materialIndex);
	unsigned int (*GetContents)(IPhysicsObject*); // const
	void (*SetContents)(IPhysicsObject*, unsigned int contents);
	float (*GetSphereRadius)(IPhysicsObject*); // const
	float (*GetEnergy)(IPhysicsObject*); // const
	Vec3 (*GetMassCenterLocalSpace)(IPhysicsObject*); // const
	void (*SetPosition)(IPhysicsObject*, const Vec3* worldPosition, const QAngle* angles, bool isTeleport);
	void (*SetPositionMatrix)(IPhysicsObject*, const matrix3x4* matrix, bool isTeleport);
	void (*GetPosition)(IPhysicsObject*, Vec3* worldPosition, QAngle* angles); // const
	void (*GetPositionMatrix)(IPhysicsObject*, matrix3x4* positionMatrix); // const
	void (*SetVelocity)(IPhysicsObject*, const Vec3* velocity, const Vec3* angularVelocity);
	void (*SetVelocityInstantaneous)(IPhysicsObject*, const Vec3* velocity, const Vec3* angularVelocity);
	void (*GetVelocity)(IPhysicsObject*, Vec3* velocity, Vec3* angularVelocity); // const
	void (*AddVelocity)(IPhysicsObject*, const Vec3* velocity, const Vec3* angularVelocity);
	void (*GetVelocityAtPoint)(IPhysicsObject*, const Vec3* worldPosition, Vec3* pVelocity); // const
	void (*GetImplicitVelocity)(IPhysicsObject*, Vec3* velocity, Vec3* angularVelocity); // const
	void (*LocalToWorld)(IPhysicsObject*, Vec3* worldPosition, const Vec3* localPosition); // const
	void (*WorldToLocal)(IPhysicsObject*, Vec3* localPosition, const Vec3* worldPosition); // const
	void (*LocalToWorldVector)(IPhysicsObject*, Vec3* worldVector, const Vec3* localVector); // const
	void (*WorldToLocalVector)(IPhysicsObject*, Vec3* localVector, const Vec3* worldVector); // const
	void (*ApplyForceCenter)(IPhysicsObject*, const Vec3* forceVector);
	void (*ApplyForceOffset)(IPhysicsObject*, const Vec3* forceVector, const Vec3* worldPosition);
	void (*ApplyTorqueCenter)(IPhysicsObject*, const Vec3* torque);
	void (*CalculateForceOffset)(IPhysicsObject*, const Vec3* forceVector, const Vec3* worldPosition, Vec3* centerForce, Vec3* centerTorque); // const
	void (*CalculateVelocityOffset)(IPhysicsObject*, const Vec3* forceVector, const Vec3* worldPosition, Vec3* centerVelocity, Vec3* centerAngularVelocity); // const
	float (*CalculateLinearDrag)(IPhysicsObject*, const Vec3* unitDirection); // const
	float (*CalculateAngularDrag)(IPhysicsObject*, const Vec3* objectSpaceRotationAxis); // const
	bool (*GetContactPoint)(IPhysicsObject*, Vec3* contactPoint, IPhysicsObject** contactObject); // const
	void (*SetShadow)(IPhysicsObject*, float maxSpeed, float maxAngularSpeed, bool allowPhysicsMovement, bool allowPhysicsRotation);
	void (*UpdateShadow)(IPhysicsObject*, const Vec3* targetPosition, const QAngle* targetAngles, bool tempDisableGravity, float timeOffset);
	int (*GetShadowPosition)(IPhysicsObject*, Vec3* position, QAngle* angles); // const
	/*IPhysicsShadowController*/ void* (*GetShadowController)(IPhysicsObject*); // const
	void (*RemoveShadowController)(IPhysicsObject*);
	float (*ComputeShadowControl)(IPhysicsObject*, /*const hlshadowcontrol_params_t **/ void* params, float secondsToArrival, float dt);
	const CPhysCollide* (*GetCollide)(IPhysicsObject*); // const
	const char* (*GetName)(IPhysicsObject*); // const
	void (*BecomeTrigger)(IPhysicsObject*);
	void (*RemoveTrigger)(IPhysicsObject*);
	void (*BecomeHinged)(IPhysicsObject*, int localAxis);
	void (*RemoveHinged)(IPhysicsObject*);
	/*IPhysicsFrictionSnapshot*/ void* (*CreateFrictionSnapshot)(IPhysicsObject*);
	void (*DestroyFrictionSnapshot)(IPhysicsObject*, /*IPhysicsFrictionSnapshot*/ void* pSnapshot);
	void (*OutputDebugInfo)(IPhysicsObject*); // const
} IPhysicsObject_VMT;

struct IPhysicsObject
{
	IPhysicsObject_VMT* vmt;

	void* m_pGameData;
	void* m_pObject;
	const CPhysCollide* m_pCollide;
	IPhysicsShadowController* m_pShadow;

	Vec3 m_dragBasis;
	Vec3 m_angDragBasis;

	// these 5 should pack into a short
	// pack new bools here
	bool m_shadowTempGravityDisable : 1;

	char pad[4];

	bool m_hasTouchedDynamic : 1;
	bool m_asleepSinceCreation : 1;
	bool m_forceSilentDelete : 1;
	unsigned char m_sleepState : 2;
	unsigned char m_hingedAxis : 3;
	unsigned char m_collideType : 3;
	unsigned short m_gameIndex;


	unsigned short m_materialIndex;
	unsigned short m_activeIndex;

	unsigned short m_callbacks;
	unsigned short m_gameFlags;
	unsigned int m_contentsMask;

	float m_volume;
	float m_buoyancyRatio;
	float m_dragCoefficient;
	float m_angDragCoefficient;
};

typedef struct IPhysicsEnvironment IPhysicsEnvironment;
typedef struct IPhysicsSurfaceProps IPhysicsSurfaceProps;
typedef struct IPhysicsConstraint IPhysicsConstraint;
typedef struct IPhysicsConstraintGroup IPhysicsConstraintGroup;
typedef struct IPhysicsFluidController IPhysicsFluidController;
typedef struct IPhysicsSpring IPhysicsSpring;
typedef struct IPhysicsVehicleController IPhysicsVehicleController;
typedef struct IConvexInfo IConvexInfo;
typedef struct IPhysicsObjectPairHash IPhysicsObjectPairHash;
typedef struct IPhysicsCollisionSet IPhysicsCollisionSet;
typedef struct IPhysicsPlayerController IPhysicsPlayerController;
typedef struct IPhysicsFrictionSnapshot IPhysicsFrictionSnapshot;

typedef struct Ray_t Ray_t;
typedef struct constraint_ragdollparams_t constraint_ragdollparams_t;
typedef struct constraint_hingeparams_t constraint_hingeparams_t;
typedef struct constraint_fixedparams_t constraint_fixedparams_t;
typedef struct constraint_ballsocketparams_t constraint_ballsocketparams_t;
typedef struct constraint_slidingparams_t constraint_slidingparams_t;
typedef struct constraint_pulleyparams_t constraint_pulleyparams_t;
typedef struct constraint_lengthparams_t constraint_lengthparams_t;
typedef struct constraint_groupparams_t constraint_groupparams_t;

typedef struct vehicleparams_t vehicleparams_t;

typedef struct fluidparams_t fluidparams_t;
typedef struct springparams_t springparams_t;
typedef struct objectparams_t
{
	Vec3* massCenterOverride;
	float mass;
	float inertia;
	float damping;
	float rotdamping;
	float rotInertiaLimit;
	const char* pName;  // used only for debugging
	void* pGameData;
	float volume;
	float dragCoefficient;
	bool enableCollisions;
} objectparams_t;
typedef struct debugcollide_t debugcollide_t;
typedef struct CGameTrace CGameTrace;
typedef CGameTrace trace_t;
typedef struct physics_stats_t physics_stats_t;
typedef struct physics_performanceparams_t
{
	int maxCollisionsPerObjectPerTimestep;  // object will be frozen after this many collisions (visual hitching vs. CPU cost)
	int maxCollisionChecksPerTimestep;      // objects may penetrate after this many collision checks (can be extended in AdditionalCollisionChecksThisTick)
	float maxVelocity;                      // limit world space linear velocity to this (in / s)
	float maxAngularVelocity;               // limit world space angular velocity to this (degrees / s)
	float lookAheadTimeObjectsVsWorld;      // predict collisions this far (seconds) into the future
	float lookAheadTimeObjectsVsObject;     // predict collisions this far (seconds) into the future
	float minFrictionMass;                  // min mass for friction solves (constrains dynamic range of mass to improve stability)
	float maxFrictionMass;                  // mas mass for friction solves
} physics_performanceparams_t;
typedef struct virtualmeshparams_t virtualmeshparams_t;

// enum PhysInterfaceId_t;
typedef struct physsaveparams_t physsaveparams_t;
typedef struct physrestoreparams_t physrestoreparams_t;
typedef struct physprerestoreparams_t physprerestoreparams_t;

typedef enum PhysInterfaceId_t
{
	PIID_UNKNOWN,
	PIID_IPHYSICSOBJECT,
	PIID_IPHYSICSFLUIDCONTROLLER,
	PIID_IPHYSICSSPRING,
	PIID_IPHYSICSCONSTRAINTGROUP,
	PIID_IPHYSICSCONSTRAINT,
	PIID_IPHYSICSSHADOWCONTROLLER,
	PIID_IPHYSICSPLAYERCONTROLLER,
	PIID_IPHYSICSMOTIONCONTROLLER,
	PIID_IPHYSICSVEHICLECONTROLLER,
	PIID_IPHYSICSGAMETRACE,

	PIID_NUM_TYPES
} PhysInterfaceId_t;

typedef struct ISave ISave;
typedef struct IRestore ISave;

//#define IVPhysicsDebugOverlay_REFL "IVPhysicsDebugOverlay"
//#define IVPhysicsDebugOverlay_SIGNATURE "VPhysicsDebugOverlay001"
//#define IVPhysicsDebugOverlay_MODULE "vphysics"

typedef struct IVPhysicsDebugOverlay_VMT
{
	void (*AddEntityTextOverlay)(IVPhysicsDebugOverlay*, int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format, ...);
	void (*AddBoxOverlay)(IVPhysicsDebugOverlay*, const Vec3* origin, const Vec3* mins, const Vec3* max, QAngle const* orientation, int r, int g, int b, int a, float duration);
	void (*AddTriangleOverlay)(IVPhysicsDebugOverlay*, const Vec3* p1, const Vec3* p2, const Vec3* p3, int r, int g, int b, int a, bool noDepthTest, float duration);
	void (*AddLineOverlay)(IVPhysicsDebugOverlay*, const Vec3* origin, const Vec3* dest, int r, int g, int b, bool noDepthTest, float duration);
	void (*AddTextOverlay)(IVPhysicsDebugOverlay*, const Vec3* origin, float duration, const char* format, ...);
	void (*AddTextOverlayOffset)(IVPhysicsDebugOverlay*, const Vec3* origin, int line_offset, float duration, const char* format, ...);
	void (*AddScreenTextOverlay)(IVPhysicsDebugOverlay*, float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text);
	void (*AddSweptBoxOverlay)(IVPhysicsDebugOverlay*, const Vec3* start, const Vec3* end, const Vec3* mins, const Vec3* max, const QAngle* angles, int r, int g, int b, int a, float flDuration);
	void (*AddTextOverlayRGB)(IVPhysicsDebugOverlay*, const Vec3* origin, int line_offset, float duration, float r, float g, float b, float alpha, const char* format, ...);
} IVPhysicsDebugOverlay_VMT;

struct IVPhysicsDebugOverlay
{
	IVPhysicsDebugOverlay_VMT* vmt;
};

typedef struct IPhysics_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	IPhysicsEnvironment* (*CreateEnvironment)(IPhysics*);
	void (*DestroyEnvironment)(IPhysics*, IPhysicsEnvironment*);
	IPhysicsEnvironment* (*GetActiveEnvironmentByIndex)(IPhysics*, int index);

	// Creates a fast hash of pairs of objects
	// Useful for maintaining a table of object relationships like pairs that do not collide.
	IPhysicsObjectPairHash* (*CreateObjectPairHash)(IPhysics*);
	void (*DestroyObjectPairHash)(IPhysics*, IPhysicsObjectPairHash* pHash);

	// holds a cache of these by id.  So you can get by id to search for the previously created set
	// UNDONE: Sets are currently limited to 32 elements.  More elements will return NULL in create.
	// NOTE: id is not allowed to be zero.
	IPhysicsCollisionSet* (*FindOrCreateCollisionSet)(IPhysics*, unsigned int id, int maxElementCount);
	IPhysicsCollisionSet* (*FindCollisionSet)(IPhysics*, unsigned int id);
	void (*DestroyAllCollisionSets)(IPhysics*);
} IPhysics_VMT;

struct IPhysics
{
	IPhysics_VMT* vmt;
};

// CPhysConvex is a single convex solid
typedef struct CPhysConvex CPhysConvex;
// CPhysPolysoup is an abstract triangle soup mesh
typedef struct CPhysPolysoup CPhysPolysoup;
typedef struct ICollisionQuery ICollisionQuery;
typedef struct IVPhysicsKeyParser IVPhysicsKeyParser;
typedef struct convertconvexparams_t convertconvexparams_t;
typedef struct CPackedPhysicsDescription CPackedPhysicsDescription;

typedef struct CPolyhedron CPolyhedron;

// UNDONE: Find a better place for this?  Should be in collisionutils, but it's needs VPHYSICS' solver.
typedef struct truncatedcone_t
{
	Vec3 origin;
	Vec3 normal;
	float h;      // height of the cone (hl units)
	float theta;  // cone angle (degrees)
} truncatedcone_t;

#define IPhysicsCollision_REFL "IPhysicsCollision"
#define IPhysicsCollision_SIGNATURE "VPhysicsCollision007"
#define IPhysicsCollision_MODULE "vphysics"

typedef struct IPhysicsCollision_VMT
{
	//virtual ~IPhysicsCollision(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	// produce a convex element from verts (convex hull around verts)
	CPhysConvex* (*ConvexFromVerts)(IPhysicsCollision*, Vec3** pVerts, int vertCount);
	// produce a convex element from planes (csg of planes)
	CPhysConvex* (*ConvexFromPlanes)(IPhysicsCollision*, float* pPlanes, int planeCount, float mergeDistance);
	// calculate volume of a convex element
	float (*ConvexVolume)(IPhysicsCollision*, CPhysConvex* pConvex);

	float (*ConvexSurfaceArea)(IPhysicsCollision*, CPhysConvex* pConvex);
	// store game-specific data in a convex solid
	void (*SetConvexGameData)(IPhysicsCollision*, CPhysConvex* pConvex, unsigned int gameData);
	// If not converted, free the convex elements with this call
	void (*ConvexFree)(IPhysicsCollision*, CPhysConvex* pConvex);
	CPhysConvex* (*BBoxToConvex)(IPhysicsCollision*, const Vec3* mins, const Vec3* maxs);
	// produce a convex element from a convex polyhedron
	CPhysConvex* (*ConvexFromConvexPolyhedron)(IPhysicsCollision*, const CPolyhedron* ConvexPolyhedron);
	// produce a set of convex triangles from a convex polygon, normal is assumed to be on the side with forward point ordering, which should be clockwise, output will need to be able to hold exactly (iPointCount-2) convexes
	void (*ConvexesFromConvexPolygon)(IPhysicsCollision*, const Vec3* vPolyNormal, const Vec3* pPoints, int iPointCount, CPhysConvex** pOutput);

	// concave objects
	// create a triangle soup
	CPhysPolysoup* (*PolysoupCreate)(IPhysicsCollision*);
	// destroy the container and memory
	void (*PolysoupDestroy)(IPhysicsCollision*, CPhysPolysoup* pSoup);
	// add a triangle to the soup
	void (*PolysoupAddTriangle)(IPhysicsCollision*, CPhysPolysoup* pSoup, const Vec3* a, const Vec3* b, const Vec3* c, int materialIndex7bits);
	// convert the convex into a compiled collision model
	CPhysCollide* (*ConvertPolysoupToCollide)(IPhysicsCollision*, CPhysPolysoup* pSoup, bool useMOPP);

	// Convert an array of convex elements to a compiled collision model (this deletes the convex elements)
	CPhysCollide* (*ConvertConvexToCollide)(IPhysicsCollision*, CPhysConvex** pConvex, int convexCount);
	CPhysCollide* (*ConvertConvexToCollideParams)(IPhysicsCollision*, CPhysConvex** pConvex, int convexCount, const convertconvexparams_t* convertParams);
	// Free a collide that was created with ConvertConvexToCollide()
	void (*DestroyCollide)(IPhysicsCollision*, CPhysCollide* pCollide);

	// Get the memory size in bytes of the collision model for serialization
	int (*CollideSize)(IPhysicsCollision*, CPhysCollide* pCollide);
	// serialize the collide to a block of memory
	int (*CollideWrite)(IPhysicsCollision*, char* pDest, CPhysCollide* pCollide, bool bSwap);
	// unserialize the collide from a block of memory
	CPhysCollide* (*UnserializeCollide)(IPhysicsCollision*, char* pBuffer, int size, int index);

	// compute the volume of a collide
	float (*CollideVolume)(IPhysicsCollision*, CPhysCollide* pCollide);
	// compute surface area for tools
	float (*CollideSurfaceArea)(IPhysicsCollision*, CPhysCollide* pCollide);

	// Get the support map for a collide in the given direction
	Vec3 (*CollideGetExtent)(IPhysicsCollision*, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles, const Vec3* direction);

	// Get an AABB for an oriented collision model
	void (*CollideGetAABB)(IPhysicsCollision*, Vec3* pMins, Vec3* pMaxs, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles);

	void (*CollideGetMassCenter)(IPhysicsCollision*, CPhysCollide* pCollide, Vec3* pOutMassCenter);
	void (*CollideSetMassCenter)(IPhysicsCollision*, CPhysCollide* pCollide, const Vec3* massCenter);
	// get the approximate cross-sectional area projected orthographically on the bbox of the collide
	// NOTE: These are fractional areas - unitless.  Basically this is the fraction of the OBB on each axis that
	// would be visible if the object were rendered orthographically.
	// NOTE: This has been precomputed when the collide was built or this function will return 1,1,1
	Vec3 (*CollideGetOrthographicAreas)(IPhysicsCollision*, const CPhysCollide* pCollide);
	void (*CollideSetOrthographicAreas)(IPhysicsCollision*, CPhysCollide* pCollide, const Vec3* areas);

	// query the vcollide index in the physics model for the instance
	int (*CollideIndex)(IPhysicsCollision*, const CPhysCollide* pCollide);

	// Convert a bbox to a collide
	CPhysCollide* (*BBoxToCollide)(IPhysicsCollision*, const Vec3* mins, const Vec3* maxs);
	int (*GetConvexesUsedInCollideable)(IPhysicsCollision*, const CPhysCollide* pCollideable, CPhysConvex** pOutputArray, int iOutputArrayLimit);

	// Trace an AABB against a collide
	void (*TraceBox)(IPhysicsCollision*, const Vec3* start, const Vec3* end, const Vec3* mins, const Vec3* maxs, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles, trace_t* ptr);
	void (*TraceBoxCollide)(IPhysicsCollision*, const Ray_t* ray, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles, trace_t* ptr);
	void (*TraceBoxMask)(IPhysicsCollision*, const Ray_t* ray, unsigned int contentsMask, IConvexInfo* pConvexInfo, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles, trace_t* ptr);

	// Trace one collide against another
	void (*TraceCollide)(IPhysicsCollision*, const Vec3* start, const Vec3* end, const CPhysCollide* pSweepCollide, const QAngle* sweepAngles, const CPhysCollide* pCollide, const Vec3* collideOrigin, const QAngle* collideAngles, trace_t* ptr);

	// relatively slow test for box vs. truncated cone
	bool (*IsBoxIntersectingCone)(IPhysicsCollision*, const Vec3* boxAbsMins, const Vec3* boxAbsMaxs, const truncatedcone_t* cone);

	// loads a set of solids into a vcollide_t
	void (*VCollideLoad)(IPhysicsCollision*, vcollide_t* pOutput, int solidCount, const char* pBuffer, int size, bool swap);
	// destroyts the set of solids created by VCollideLoad
	void (*VCollideUnload)(IPhysicsCollision*, vcollide_t* pVCollide);

	// begins parsing a vcollide.  NOTE: This keeps pointers to the text
	// If you free the text and call members of IVPhysicsKeyParser, it will crash
	IVPhysicsKeyParser* (*VPhysicsKeyParserCreate)(IPhysicsCollision*, const char* pKeyData);
	// Free the parser created by VPhysicsKeyParserCreate
	void (*VPhysicsKeyParserDestroy)(IPhysicsCollision*, IVPhysicsKeyParser* pParser);

	// creates a list of verts from a collision mesh
	int (*CreateDebugMesh)(IPhysicsCollision*, CPhysCollide const* pCollisionModel, Vec3** outVerts);
	// destroy the list of verts created by CreateDebugMesh
	void (*DestroyDebugMesh)(IPhysicsCollision*, int vertCount, Vec3* outVerts);

	// create a queryable version of the collision model
	ICollisionQuery* (*CreateQueryModel)(IPhysicsCollision*, CPhysCollide* pCollide);
	// destroy the queryable version
	void (*DestroyQueryModel)(IPhysicsCollision*, ICollisionQuery* pQuery);

	IPhysicsCollision* (*ThreadContextCreate)(IPhysicsCollision*);
	void (*ThreadContextDestroy)(IPhysicsCollision*, IPhysicsCollision* pThreadContex);

	CPhysCollide* (*CreateVirtualMesh)(IPhysicsCollision*, const virtualmeshparams_t* params);
	bool (*SupportsVirtualMesh)(IPhysicsCollision*);

	bool (*GetBBoxCacheSize)(IPhysicsCollision*, int* pCachedSize, int* pCachedCount);

	// extracts a polyhedron that defines a CPhysConvex's shape
	CPolyhedron* (*PolyhedronFromConvex)(IPhysicsCollision*, CPhysConvex* const pConvex, bool bUseTempPolyhedron);

	// dumps info about the collide to Msg()
	void (*OutputDebugInfo)(IPhysicsCollision*, const CPhysCollide* pCollide);
	unsigned int (*ReadStat)(IPhysicsCollision*, int statID);
} IPhysicsCollision_VMT;

struct IPhysicsCollision
{
	IPhysicsCollision_VMT* vmt;
};

// this can be used to post-process a collision model
typedef struct ICollisionQuery_VMT
{
	//virtual ~ICollisionQuery() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	// number of convex pieces in the whole solid
	int (*ConvexCount)(ICollisionQuery*);
	// triangle count for this convex piece
	int (*TriangleCount)(ICollisionQuery*, int convexIndex);
	// get the stored game data
	unsigned int (*GetGameData)(ICollisionQuery*, int convexIndex);
	// Gets the triangle's verts to an array
	void (*GetTriangleVerts)(ICollisionQuery*, int convexIndex, int triangleIndex, Vec3* verts);

	// UNDONE: This doesn't work!!!
	void (*SetTriangleVerts)(ICollisionQuery*, int convexIndex, int triangleIndex, const Vec3* verts);

	// returns the 7-bit material index
	int (*GetTriangleMaterialIndex)(ICollisionQuery*, int convexIndex, int triangleIndex);
	// sets a 7-bit material index for this triangle
	void (*SetTriangleMaterialIndex)(ICollisionQuery*, int convexIndex, int triangleIndex, int index7bits);
} ICollisionQuery_VMT;

struct ICollisionQuery
{
	ICollisionQuery_VMT* vmt;
};

//-----------------------------------------------------------------------------
// Purpose: Ray traces from game engine.
//-----------------------------------------------------------------------------
typedef struct IPhysicsGameTrace_VMT
{
	void (*VehicleTraceRay)(IPhysicsGameTrace*, const Ray_t* ray, void* pVehicle, trace_t* pTrace);
	void (*VehicleTraceRayWithWater)(IPhysicsGameTrace*, const Ray_t* ray, void* pVehicle, trace_t* pTrace);
	bool (*VehiclePointInWater)(IPhysicsGameTrace*, const Vec3* vecPoint);
} IPhysicsGameTrace_VMT;

struct IPhysicsGameTrace
{
	IPhysicsGameTrace_VMT* vmt;
};

// The caller should implement this to return contents masks per convex on a collide
typedef struct IConvexInfo_VMT
{
	unsigned int (*GetContents)(IConvexInfo*, int convexGameData);
} IConvexInfo_VMT;

struct IConvexInfo
{
	IConvexInfo_VMT* vmt;
};

typedef struct CPhysicsEventHandler CPhysicsEventHandler;
typedef struct IPhysicsCollisionData_VMT
{
	void (*GetSurfaceNormal)(IPhysicsCollisionData*, Vec3* out);  // normal points toward second object (object index 1)
	void (*GetContactPoint)(IPhysicsCollisionData*, Vec3* out);   // contact point of collision (in world space)
	void (*GetContactSpeed)(IPhysicsCollisionData*, Vec3* out);   // speed of surface 1 relative to surface 0 (in world space)
} IPhysicsCollisionData_VMT;

struct IPhysicsCollisionData
{
	IPhysicsCollisionData_VMT* vmt;
};

typedef struct vcollisionevent_t
{
	IPhysicsObject* pObjects[2];
	int surfaceProps[2];
	bool isCollision;
	bool isShadowCollision;
	float deltaCollisionTime;

	float collisionSpeed;                  // only valid at postCollision
	IPhysicsCollisionData* pInternalData;  // may change pre/post collision
} vcollisionevent_t;

typedef struct IPhysicsCollisionEvent_VMT
{
	// returns the two objects that collided, time between last collision of these objects
	// and an opaque data block of collision information
	// NOTE: PreCollision/PostCollision ALWAYS come in matched pairs!!!
	void (*PreCollision)(IPhysicsCollisionEvent*, vcollisionevent_t* pEvent);
	void (*PostCollision)(IPhysicsCollisionEvent*, vcollisionevent_t* pEvent);

	// This is a scrape event.  The object has scraped across another object consuming the indicated energy
	void (*Friction)(IPhysicsCollisionEvent*, IPhysicsObject* pObject, float energy, int surfaceProps, int surfacePropsHit, IPhysicsCollisionData* pData);

	void (*StartTouch)(IPhysicsCollisionEvent*, IPhysicsObject* pObject1, IPhysicsObject* pObject2, IPhysicsCollisionData* pTouchData);
	void (*EndTouch)(IPhysicsCollisionEvent*, IPhysicsObject* pObject1, IPhysicsObject* pObject2, IPhysicsCollisionData* pTouchData);

	void (*FluidStartTouch)(IPhysicsCollisionEvent*, IPhysicsObject* pObject, IPhysicsFluidController* pFluid);
	void (*FluidEndTouch)(IPhysicsCollisionEvent*, IPhysicsObject* pObject, IPhysicsFluidController* pFluid);

	void (*PostSimulationFrame)(IPhysicsCollisionEvent*);

	//virtual void ObjectEnterTrigger(IPhysicsObject* pTrigger, IPhysicsObject* pObject) {}
	//virtual void ObjectLeaveTrigger(IPhysicsObject* pTrigger, IPhysicsObject* pObject) {}
} IPhysicsCollisionEvent_VMT;

struct IPhysicsCollisionEvent
{
	IPhysicsCollisionEvent_VMT* vmt;
};

typedef struct IPhysicsObjectEvent_VMT
{
	// these can be used to optimize out queries on sleeping objects
	// Called when an object is woken after sleeping
	void (*ObjectWake)(IPhysicsObjectEvent*, IPhysicsObject* pObject);
	// called when an object goes to sleep (no longer simulating)
	void (*ObjectSleep)(IPhysicsObjectEvent*, IPhysicsObject* pObject);
} IPhysicsObjectEvent_VMT;

struct IPhysicsObjectEvent
{
	IPhysicsObjectEvent_VMT* vmt;
};

typedef struct IPhysicsConstraintEvent_VMT
{
	// the constraint is now inactive, the game code is required to delete it or re-activate it.
	void (*ConstraintBroken)(IPhysicsConstraintEvent*, IPhysicsConstraint*);
} IPhysicsConstraintEvent_VMT;

struct IPhysicsConstraintEvent
{
	IPhysicsConstraintEvent_VMT* vmt;
};

typedef struct hlshadowcontrol_params_t
{
	Vec3 targetPosition;
	QAngle targetRotation;
	float maxAngular;
	float maxDampAngular;
	float maxSpeed;
	float maxDampSpeed;
	float dampFactor;
	float teleportDistance;
} hlshadowcontrol_params_t;

// UNDONE: At some point allow this to be parameterized using hlshadowcontrol_params_t.
// All of the infrastructure is in place to do that.

typedef struct IPhysicsShadowController_VMT
{
	//virtual ~IPhysicsShadowController(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*Update)(IPhysicsShadowController*, const Vec3* position, const QAngle* angles, float timeOffset);
	void (*MaxSpeed)(IPhysicsShadowController*, float maxSpeed, float maxAngularSpeed);
	void (*StepUp)(IPhysicsShadowController*, float height);

	// If the teleport distance is non-zero, the object will be teleported to
	// the target location when the error exceeds this quantity.
	void (*SetTeleportDistance)(IPhysicsShadowController*, float teleportDistance);
	bool (*AllowsTranslation)(IPhysicsShadowController*);
	bool (*AllowsRotation)(IPhysicsShadowController*);

	// There are two classes of shadow objects:
	// 1) Game physics controlled, shadow follows game physics (this is the default)
	// 2) Physically controlled - shadow position is a target, but the game hasn't guaranteed that the space can be occupied by this object
	void (*SetPhysicallyControlled)(IPhysicsShadowController*, bool isPhysicallyControlled);
	bool (*IsPhysicallyControlled)(IPhysicsShadowController*);
	void (*GetLastImpulse)(IPhysicsShadowController*, Vec3* pOut);
	void (*UseShadowMaterial)(IPhysicsShadowController*, bool bUseShadowMaterial);
	void (*ObjectMaterialChanged)(IPhysicsShadowController*, int materialIndex);

	// Basically get the last inputs to IPhysicsShadowController::Update(), returns last input to timeOffset in Update()
	float (*GetTargetPosition)(IPhysicsShadowController*, Vec3* pPositionOut, QAngle* pAnglesOut);

	float (*GetTeleportDistance)(IPhysicsShadowController*);
	void (*GetMaxSpeed)(IPhysicsShadowController*, float* pMaxSpeedOut, float* pMaxAngularSpeedOut);
} IPhysicsShadowController_VMT;

struct IPhysicsShadowController
{
	IPhysicsShadowController_VMT* vmt;
};

typedef struct CPhysicsSimObject CPhysicsSimObject;
typedef struct IPhysicsMotionController IPhysicsMotionController;

typedef enum simresult_e
{
	SIM_NOTHING = 0,
	SIM_LOCAL_ACCELERATION,
	SIM_LOCAL_FORCE,
	SIM_GLOBAL_ACCELERATION,
	SIM_GLOBAL_FORCE
} simresult_e;

// Callback for simulation
typedef struct IMotionEvent_VMT
{
	// These constants instruct the simulator as to how to apply the values copied to linear & angular
	// GLOBAL/LOCAL refer to the coordinate system of the values, whereas acceleration/force determine whether or not
	// mass is divided out (forces must be divided by mass to compute acceleration)

	simresult_e (*Simulate)(IMotionEvent*, IPhysicsMotionController* pController, IPhysicsObject* pObject, float deltaTime, Vec3* linear, Vec3* angular);
} IMotionEvent_VMT;

struct IMotionEvent
{
	IMotionEvent_VMT* vmt;
};

typedef enum priority_t
{
	LOW_PRIORITY = 0,
	MEDIUM_PRIORITY = 1,
	HIGH_PRIORITY = 2,
} priority_t;

typedef struct IPhysicsMotionController_VMT
{
	//virtual ~IPhysicsMotionController(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*SetEventHandler)(IPhysicsMotionController*, IMotionEvent* handler);
	void (*AttachObject)(IPhysicsMotionController*, IPhysicsObject* pObject, bool checkIfAlreadyAttached);
	void (*DetachObject)(IPhysicsMotionController*, IPhysicsObject* pObject);

	// returns the number of objects currently attached to the controller
	int (*CountObjects)(IPhysicsMotionController*);
	// NOTE: pObjectList is an array with at least CountObjects() allocated
	void (*GetObjects)(IPhysicsMotionController*, IPhysicsObject** pObjectList);
	// detaches all attached objects
	void (*ClearObjects)(IPhysicsMotionController*);
	// wakes up all attached objects
	void (*WakeObjects)(IPhysicsMotionController*);

	void (*SetPriority)(IPhysicsMotionController*, priority_t priority);
} IPhysicsMotionController_VMT;

struct IPhysicsMotionController
{
	IPhysicsMotionController_VMT* vmt;
};

// -------------------
// Collision filter function.  Return 0 if objects should not be tested for collisions, nonzero otherwise
// Install with IPhysicsEnvironment::SetCollisionFilter()
// -------------------
typedef struct IPhysicsCollisionSolver_VMT
{
	int (*ShouldCollide)(IPhysicsCollisionSolver*, IPhysicsObject* pObj0, IPhysicsObject* pObj1, void* pGameData0, void* pGameData1);
	int (*ShouldSolvePenetration)(IPhysicsCollisionSolver*, IPhysicsObject* pObj0, IPhysicsObject* pObj1, void* pGameData0, void* pGameData1, float dt);

	// pObject has already done the max number of collisions this tick, should we freeze it to save CPU?
	bool (*ShouldFreezeObject)(IPhysicsCollisionSolver*, IPhysicsObject* pObject);

	// The system has already done too many collision checks, performance will suffer.
	// How many more should it do?
	int (*AdditionalCollisionChecksThisTick)(IPhysicsCollisionSolver*, int currentChecksDone);

	// This list of objects is in a connected contact graph that is too large to solve quickly
	// return true to freeze the system, false to solve it
	bool (*ShouldFreezeContacts)(IPhysicsCollisionSolver*, IPhysicsObject** pObjectList, int objectCount);
} IPhysicsCollisionSolver_VMT;

struct IPhysicsCollisionSolver
{
	IPhysicsCollisionSolver_VMT* vmt;
};

typedef enum PhysicsTraceType_t
{
	VPHYSICS_TRACE_EVERYTHING = 0,
	VPHYSICS_TRACE_STATIC_ONLY,
	VPHYSICS_TRACE_MOVING_ONLY,
	VPHYSICS_TRACE_TRIGGERS_ONLY,
	VPHYSICS_TRACE_STATIC_AND_MOVING,
} PhysicsTraceType_t;

typedef struct IPhysicsTraceFilter_VMT
{
	bool (*ShouldHitObject)(IPhysicsTraceFilter*, IPhysicsObject* pObject, int contentsMask);
	PhysicsTraceType_t (*GetTraceType)(IPhysicsTraceFilter*); // const
} IPhysicsTraceFilter_VMT;

struct IPhysicsTraceFilter
{
	IPhysicsTraceFilter_VMT* vmt;
};

typedef struct IPhysicsEnvironment_VMT
{
	//virtual ~IPhysicsEnvironment(void) {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*SetDebugOverlay)(IPhysicsEnvironment*, CreateInterfaceFn debugOverlayFactory);
	IVPhysicsDebugOverlay* (*GetDebugOverlay)(IPhysicsEnvironment*);

	// gravity is a 3-vector in in/s^2
	void (*SetGravity)(IPhysicsEnvironment*, const Vec3* gravityVector);
	void (*GetGravity)(IPhysicsEnvironment*, Vec3* pGravityVector); // const

	// air density is in kg / m^3 (water is 1000)
	// This controls drag, air that is more dense has more drag.
	void (*SetAirDensity)(IPhysicsEnvironment*, float density);
	float (*GetAirDensity)(IPhysicsEnvironment*); // const

	// object creation
	// create a polygonal object.  pCollisionModel was created by the physics builder DLL in a pre-process.
	IPhysicsObject* (*CreatePolyObject)(IPhysicsEnvironment*, const CPhysCollide* pCollisionModel, int materialIndex, const Vec3* position, const QAngle* angles, objectparams_t* pParams);
	// same as above, but this one cannot move or rotate (infinite mass/inertia)
	IPhysicsObject* (*CreatePolyObjectStatic)(IPhysicsEnvironment*, const CPhysCollide* pCollisionModel, int materialIndex, const Vec3* position, const QAngle* angles, objectparams_t* pParams);
	// Create a perfectly spherical object
	IPhysicsObject* (*CreateSphereObject)(IPhysicsEnvironment*, float radius, int materialIndex, const Vec3* position, const QAngle* angles, objectparams_t* pParams, bool isStatic);
	// destroy an object created with CreatePolyObject() or CreatePolyObjectStatic()
	void (*DestroyObject)(IPhysicsEnvironment*, IPhysicsObject*);

	// Create a polygonal fluid body out of the specified collision model
	// This object will affect any other objects that collide with the collision model
	IPhysicsFluidController* (*CreateFluidController)(IPhysicsEnvironment*, IPhysicsObject* pFluidObject, fluidparams_t* pParams);
	// Destroy an object created with CreateFluidController()
	void (*DestroyFluidController)(IPhysicsEnvironment*, IPhysicsFluidController*);

	// Create a simulated spring that connects 2 objects
	IPhysicsSpring* (*CreateSpring)(IPhysicsEnvironment*, IPhysicsObject* pObjectStart, IPhysicsObject* pObjectEnd, springparams_t* pParams);
	void (*DestroySpring)(IPhysicsEnvironment*, IPhysicsSpring*);

	// Create a constraint in the space of pReferenceObject which is attached by the constraint to pAttachedObject
	IPhysicsConstraint* (*CreateRagdollConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_ragdollparams_t* ragdoll);
	IPhysicsConstraint* (*CreateHingeConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_hingeparams_t* hinge);
	IPhysicsConstraint* (*CreateFixedConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_fixedparams_t* fixed);
	IPhysicsConstraint* (*CreateSlidingConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_slidingparams_t* sliding);
	IPhysicsConstraint* (*CreateBallsocketConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_ballsocketparams_t* ballsocket);
	IPhysicsConstraint* (*CreatePulleyConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_pulleyparams_t* pulley);
	IPhysicsConstraint* (*CreateLengthConstraint)(IPhysicsEnvironment*, IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_lengthparams_t* length);

	void (*DestroyConstraint)(IPhysicsEnvironment*, IPhysicsConstraint*);

	IPhysicsConstraintGroup* (*CreateConstraintGroup)(IPhysicsEnvironment*, const constraint_groupparams_t* groupParams);
	void (*DestroyConstraintGroup)(IPhysicsEnvironment*, IPhysicsConstraintGroup* pGroup);

	IPhysicsShadowController* (*CreateShadowController)(IPhysicsEnvironment*, IPhysicsObject* pObject, bool allowTranslation, bool allowRotation);
	void (*DestroyShadowController)(IPhysicsEnvironment*, IPhysicsShadowController*);

	IPhysicsPlayerController* (*CreatePlayerController)(IPhysicsEnvironment*, IPhysicsObject* pObject);
	void (*DestroyPlayerController)(IPhysicsEnvironment*, IPhysicsPlayerController*);

	IPhysicsMotionController* (*CreateMotionController)(IPhysicsEnvironment*, IMotionEvent* pHandler);
	void (*DestroyMotionController)(IPhysicsEnvironment*, IPhysicsMotionController* pController);

	IPhysicsVehicleController* (*CreateVehicleController)(IPhysicsEnvironment*, IPhysicsObject* pVehicleBodyObject, const vehicleparams_t* params, unsigned int nVehicleType, IPhysicsGameTrace* pGameTrace);
	void (*DestroyVehicleController)(IPhysicsEnvironment*, IPhysicsVehicleController*);

	// install a function to filter collisions/penentration
	void (*SetCollisionSolver)(IPhysicsEnvironment*, IPhysicsCollisionSolver* pSolver);

	// run the simulator for deltaTime seconds
	void (*Simulate)(IPhysicsEnvironment*, float deltaTime);
	// true if currently running the simulator (i.e. in a callback during physenv->Simulate())
	bool (*IsInSimulation)(IPhysicsEnvironment*); // const

	// Manage the timestep (period) of the simulator.  The main functions are all integrated with
	// this period as dt.
	float (*GetSimulationTimestep)(IPhysicsEnvironment*); // const
	void (*SetSimulationTimestep)(IPhysicsEnvironment*, float timestep);

	// returns the current simulation clock's value.  This is an absolute time.
	float (*GetSimulationTime)(IPhysicsEnvironment*); // const
	void (*ResetSimulationClock)(IPhysicsEnvironment*);
	// returns the current simulation clock's value at the next frame.  This is an absolute time.
	float (*GetNextFrameTime)(IPhysicsEnvironment*); // const

	// Collision callbacks (game code collision response)
	void (*SetCollisionEventHandler)(IPhysicsEnvironment*, IPhysicsCollisionEvent* pCollisionEvents);
	void (*SetObjectEventHandler)(IPhysicsEnvironment*, IPhysicsObjectEvent* pObjectEvents);
	void (*SetConstraintEventHandler)(IPhysicsEnvironment*, IPhysicsConstraintEvent* pConstraintEvents);

	void (*SetQuickDelete)(IPhysicsEnvironment*, bool bQuick);

	int (*GetActiveObjectCount)(IPhysicsEnvironment*); // const
	void (*GetActiveObjects)(IPhysicsEnvironment*, IPhysicsObject** pOutputObjectList); // const
	const IPhysicsObject** (*GetObjectList)(IPhysicsEnvironment*, int* pOutputObjectCount); // const
	bool (*TransferObject)(IPhysicsEnvironment*, IPhysicsObject* pObject, IPhysicsEnvironment* pDestinationEnvironment);

	void (*CleanupDeleteList)(IPhysicsEnvironment*);
	void (*EnableDeleteQueue)(IPhysicsEnvironment*, bool enable);

	// Save/Restore methods
	bool (*Save)(IPhysicsEnvironment*, const physsaveparams_t* params);
	void (*PreRestore)(IPhysicsEnvironment*, const physprerestoreparams_t* params);
	bool (*Restore)(IPhysicsEnvironment*, const physrestoreparams_t* params);
	void (*PostRestore)(IPhysicsEnvironment*);

	// Debugging:
	bool (*IsCollisionModelUsed)(IPhysicsEnvironment*, CPhysCollide* pCollide); // const

	// Physics world version of the enginetrace API:
	void (*TraceRay)(IPhysicsEnvironment*, const Ray_t* ray, unsigned int fMask, IPhysicsTraceFilter* pTraceFilter, trace_t* pTrace);
	void (*SweepCollideable)(IPhysicsEnvironment*, const CPhysCollide* pCollide, const Vec3* vecAbsStart, const Vec3* vecAbsEnd, const QAngle* vecAngles, unsigned int fMask, IPhysicsTraceFilter* pTraceFilter, trace_t* pTrace);

	// performance tuning
	void (*GetPerformanceSettings)(IPhysicsEnvironment*, physics_performanceparams_t* pOutput); // const
	void (*SetPerformanceSettings)(IPhysicsEnvironment*, const physics_performanceparams_t* pSettings);

	// perf/cost statistics
	void (*ReadStats)(IPhysicsEnvironment*, physics_stats_t* pOutput);
	void (*ClearStats)(IPhysicsEnvironment*);

	unsigned int (*GetObjectSerializeSize)(IPhysicsEnvironment*, IPhysicsObject* pObject); // const
	void (*SerializeObjectToBuffer)(IPhysicsEnvironment*, IPhysicsObject* pObject, unsigned char* pBuffer, unsigned int bufferSize);
	IPhysicsObject* (*UnserializeObjectFromBuffer)(IPhysicsEnvironment*, void* pGameData, unsigned char* pBuffer, unsigned int bufferSize, bool enableCollisions);

	void (*EnableConstraintNotify)(IPhysicsEnvironment*, bool bEnable);
	void (*DebugCheckContacts)(IPhysicsEnvironment*);
} IPhysicsEnvironment_VMT;

struct IPhysicsEnvironment
{
	IPhysicsEnvironment_VMT* vmt;
};

#define IPhysics_REFL "IPhysics"
#define IPhysics_SIGNATURE "VPhysics031"
#define IPhysics_MODULE "vphysics"

#endif
