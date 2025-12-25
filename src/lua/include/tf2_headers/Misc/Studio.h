#if !defined(MiscStudio_Included)
#define MiscStudio_Included

#include "Misc/BaseTypes.h"
#include "Misc/Datamap.h"
#include "Misc/LocalFlexController.h"
#include "Main/UtlVector.h"
#include "Types.h"

#define STUDIO_ENABLE_PERF_COUNTERS
#define STUDIO_SEQUENCE_ACTIVITY_LOOKUPS_ARE_SLOW 0
#define STUDIO_SEQUENCE_ACTIVITY_LAZY_INITIALIZE 1

typedef struct IMaterial IMaterial;
typedef struct IMesh IMesh;
typedef struct IMorph IMorph;
typedef struct vertexFileHeader_t vertexFileHeader_t;
typedef struct StripHeader_t StripHeader_t;

#define STUDIO_VERSION 48

#define MAXSTUDIOTRIANGLES 65536
#define MAXSTUDIOVERTS 65536
#define MAXSTUDIOFLEXVERTS 10000
#define MAXSTUDIOSKINS 32
#define MAXSTUDIOBONES 128
#define MAXSTUDIOFLEXDESC 1024
#define MAXSTUDIOFLEXCTRL 96
#define MAXSTUDIOPOSEPARAM 24
#define MAXSTUDIOBONECTRLS 4
#define MAXSTUDIOANIMBLOCKS 256
#define MAXSTUDIOBONEBITS 7
#define MAX_NUM_BONES_PER_VERT 3

#define NEW_EVENT_STYLE (1 << 10)

typedef struct mstudiodata_t
{
	int count;
	int offset;
} mstudiodata_t;

#define STUDIO_PROC_AXISINTERP 1
#define STUDIO_PROC_QUATINTERP 2
#define STUDIO_PROC_AIMATBONE 3
#define STUDIO_PROC_AIMATATTACH 4
#define STUDIO_PROC_JIGGLE 5

typedef struct mstudioaxisinterpbone_t
{
	int control;
	int axis;
	Vector pos[6];
	Quaternion quat[6];
} mstudioaxisinterpbone_t;

typedef struct mstudioquatinterpinfo_t
{
	float inv_tolerance;
	Quaternion trigger;
	Vector pos;
	Quaternion quat;
} mstudioquatinterpinfo_t;

typedef struct mstudioquatinterpbone_t
{
	int control;
	int numtriggers;
	int triggerindex;
} mstudioquatinterpbone_t;

#define JIGGLE_IS_FLEXIBLE 0x01
#define JIGGLE_IS_RIGID 0x02
#define JIGGLE_HAS_YAW_CONSTRAINT 0x04
#define JIGGLE_HAS_PITCH_CONSTRAINT 0x08
#define JIGGLE_HAS_ANGLE_CONSTRAINT 0x10
#define JIGGLE_HAS_LENGTH_CONSTRAINT 0x20
#define JIGGLE_HAS_BASE_SPRING 0x40
#define JIGGLE_IS_BOING 0x80

typedef struct mstudiojigglebone_t
{
	int flags;
	float length;
	float tipMass;
	float yawStiffness;
	float yawDamping;
	float pitchStiffness;
	float pitchDamping;
	float alongStiffness;
	float alongDamping;
	float angleLimit;
	float minYaw;
	float maxYaw;
	float yawFriction;
	float yawBounce;
	float minPitch;
	float maxPitch;
	float pitchFriction;
	float pitchBounce;
	float baseMass;
	float baseStiffness;
	float baseDamping;
	float baseMinLeft;
	float baseMaxLeft;
	float baseLeftFriction;
	float baseMinUp;
	float baseMaxUp;
	float baseUpFriction;
	float baseMinForward;
	float baseMaxForward;
	float baseForwardFriction;
	float boingImpactSpeed;
	float boingImpactAngle;
	float boingDampingRate;
	float boingFrequency;
	float boingAmplitude;
} mstudiojigglebone_t;

typedef struct mstudioaimatbone_t
{
	int parent;
	int aim;
	Vector aimvector;
	Vector upvector;
	Vector basepos;
} mstudioaimatbone_t;

typedef struct mstudiobone_t
{
	int sznameindex;
	int parent;
	int bonecontroller[6];
	Vector pos;
	Quaternion quat;
	RadianEuler rot;
	Vector posscale;
	Vector rotscale;
	matrix3x4 poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex;
	int physicsbone;
	int surfacepropidx;
	int contents;
	int unused[8];
} mstudiobone_t;

typedef struct mstudiolinearbone_t
{
	int numbones;
	int flagsindex;
	int parentindex;
	int posindex;
	int quatindex;
	int rotindex;
	int posetoboneindex;
	int posscaleindex;
	int rotscaleindex;
	int qalignmentindex;
	int unused[6];
} mstudiolinearbone_t;

typedef enum StudioBoneFlexComponent_t
{
	STUDIO_BONE_FLEX_INVALID = -1,
	STUDIO_BONE_FLEX_TX = 0,
	STUDIO_BONE_FLEX_TY = 1,
	STUDIO_BONE_FLEX_TZ = 2
} StudioBoneFlexComponent_t;

typedef struct mstudioboneflexdrivercontrol_t
{
	int m_nBoneComponent;
	int m_nFlexControllerIndex;
	float m_flMin;
	float m_flMax;
} mstudioboneflexdrivercontrol_t;

typedef struct mstudioboneflexdriver_t
{
	int m_nBoneIndex;
	int m_nControlCount;
	int m_nControlIndex;
	int unused[3];
} mstudioboneflexdriver_t;

#define BONE_CALCULATE_MASK 0x1F
#define BONE_PHYSICALLY_SIMULATED 0x01
#define BONE_PHYSICS_PROCEDURAL 0x02
#define BONE_ALWAYS_PROCEDURAL 0x04
#define BONE_SCREEN_ALIGN_SPHERE 0x08
#define BONE_SCREEN_ALIGN_CYLINDER 0x10
#define BONE_USED_MASK 0x0007FF00
#define BONE_USED_BY_ANYTHING 0x0007FF00
#define BONE_USED_BY_HITBOX 0x00000100
#define BONE_USED_BY_ATTACHMENT 0x00000200
#define BONE_USED_BY_VERTEX_MASK 0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0 0x00000400
#define BONE_USED_BY_VERTEX_LOD1 0x00000800
#define BONE_USED_BY_VERTEX_LOD2 0x00001000
#define BONE_USED_BY_VERTEX_LOD3 0x00002000
#define BONE_USED_BY_VERTEX_LOD4 0x00004000
#define BONE_USED_BY_VERTEX_LOD5 0x00008000
#define BONE_USED_BY_VERTEX_LOD6 0x00010000
#define BONE_USED_BY_VERTEX_LOD7 0x00020000
#define BONE_USED_BY_BONE_MERGE 0x00040000
#define BONE_USED_BY_VERTEX_AT_LOD(lod) (BONE_USED_BY_VERTEX_LOD0 << (lod))
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ((BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK) | BONE_USED_BY_VERTEX_AT_LOD(lod))

#define MAX_NUM_LODS 8

#define BONE_TYPE_MASK 0x00F00000
#define BONE_FIXED_ALIGNMENT 0x00100000
#define BONE_HAS_SAVEFRAME_POS 0x00200000
#define BONE_HAS_SAVEFRAME_ROT 0x00400000

typedef struct mstudiobonecontroller_t
{
	int bone;
	int type;
	float start;
	float end;
	int rest;
	int inputfield;
	int unused[8];
} mstudiobonecontroller_t;

typedef struct mstudiobbox_t
{
	int bone;
	int group;
	Vector bbmin;
	Vector bbmax;
	int szhitboxnameindex;
	Vec3 angle;
	float m_radius;
	uint8_t _padding[0x10];
} mstudiobbox_t;

typedef struct mstudiomodelgroup_t
{
	int szlabelindex;
	int sznameindex;
} mstudiomodelgroup_t;

typedef struct mstudiomodelgrouplookup_t
{
	int modelgroup;
	int indexwithingroup;
} mstudiomodelgrouplookup_t;

typedef struct mstudioevent_t
{
	float cycle;
	int event;
	int type;
	char options[64];
	int szeventindex;
} mstudioevent_t;

#define ATTACHMENT_FLAG_WORLD_ALIGN 0x10000

typedef struct mstudioattachment_t
{
	int sznameindex;
	unsigned int flags;
	int localbone;
	matrix3x4 local;
	int unused[8];
} mstudioattachment_t;

#define IK_SELF 1
#define IK_WORLD 2
#define IK_GROUND 3
#define IK_RELEASE 4
#define IK_ATTACHMENT 5
#define IK_UNLATCH 6

typedef struct mstudioikerror_t
{
	Vector pos;
	Quaternion q;
} mstudioikerror_t;

typedef union mstudioanimvalue_t mstudioanimvalue_t;

typedef struct mstudiocompressedikerror_t
{
	float scale[6];
	short offset[6];
} mstudiocompressedikerror_t;

typedef struct mstudioikrule_t
{
	int index;
	int type;
	int chain;
	int bone;
	int slot;
	float height;
	float radius;
	float floor;
	Vector pos;
	Quaternion q;
	int compressedikerrorindex;
	int unused2;
	int iStart;
	int ikerrorindex;
	float start;
	float peak;
	float tail;
	float end;
	float unused3;
	float contact;
	float drop;
	float top;
	int unused6;
	int unused7;
	int unused8;
	int szattachmentindex;
	int unused[7];
} mstudioikrule_t;

typedef struct mstudioiklock_t
{
	int chain;
	float flPosWeight;
	float flLocalQWeight;
	int flags;
	int unused[4];
} mstudioiklock_t;

typedef struct mstudiolocalhierarchy_t
{
	int iBone;
	int iNewParent;
	float start;
	float peak;
	float tail;
	float end;
	int iStart;
	int localanimindex;
	int unused[4];
} mstudiolocalhierarchy_t;

typedef union mstudioanimvalue_t
{
	struct
	{
		byte valid;
		byte total;
	} num;

	short value;
} mstudioanimvalue_t;

typedef struct mstudioanim_valueptr_t
{
	short offset[3];
} mstudioanim_valueptr_t;

#define STUDIO_ANIM_RAWPOS 0x01
#define STUDIO_ANIM_RAWROT 0x02
#define STUDIO_ANIM_ANIMPOS 0x04
#define STUDIO_ANIM_ANIMROT 0x08
#define STUDIO_ANIM_DELTA 0x10
#define STUDIO_ANIM_RAWROT2 0x20

typedef struct mstudioanim_t
{
	byte bone;
	byte flags;
	short nextoffset;
} mstudioanim_t;

typedef struct mstudiomovement_t
{
	int endframe;
	int motionflags;
	float v0;
	float v1;
	float angle;
	Vector vector;
	Vector position;
} mstudiomovement_t;

typedef struct studiohdr_t studiohdr_t;

typedef struct mstudioanimblock_t
{
	int datastart;
	int dataend;
} mstudioanimblock_t;

typedef struct mstudioanimsections_t
{
	int animblock;
	int animindex;
} mstudioanimsections_t;

typedef struct mstudioanimdesc_t
{
	int baseptr;
	int sznameindex;
	float fps;
	int flags;
	int numframes;
	int nummovements;
	int movementindex;
	int unused1[6];
	int animblock;
	int animindex;
	int numikrules;
	int ikruleindex;
	int animblockikruleindex;
	int numlocalhierarchy;
	int localhierarchyindex;
	int sectionindex;
	int sectionframes;
	short zeroframespan;
	short zeroframecount;
	int zeroframeindex;
	float zeroframestalltime;
} mstudioanimdesc_t;

typedef struct mstudioikrule_t mstudioikrule_t;

typedef struct mstudioautolayer_t
{
	short iSequence;
	short iPose;
	int flags;
	float start;
	float peak;
	float tail;
	float end;
} mstudioautolayer_t;

typedef struct mstudioactivitymodifier_t
{
	int sznameindex;
} mstudioactivitymodifier_t;

typedef struct mstudioseqdesc_t
{
	int baseptr;
	int szlabelindex;
	int szactivitynameindex;
	int flags;
	int activity;
	int actweight;
	int numevents;
	int eventindex;
	Vector bbmin;
	Vector bbmax;
	int numblends;
	int animindexindex;

	int movementindex;
	int groupsize[2];
	int paramindex[2];
	float paramstart[2];
	float paramend[2];
	int paramparent;
	float fadeintime;
	float fadeouttime;
	int localentrynode;
	int localexitnode;
	int nodeflags;
	float entryphase;
	float exitphase;
	float lastframe;
	int nextseq;
	int pose;
	int numikrules;
	int numautolayers;
	int autolayerindex;
	int weightlistindex;
	int posekeyindex;
	int numiklocks;
	int iklockindex;
	int keyvalueindex;
	int keyvaluesize;
	int cycleposeindex;
	int activitymodifierindex;
	int numactivitymodifiers;
	int unused[5];
} mstudioseqdesc_t;

typedef struct mstudioposeparamdesc_t
{
	int sznameindex;
	int flags;
	float start;
	float end;
	float loop;
} mstudioposeparamdesc_t;

typedef struct mstudioflexdesc_t
{
	int szFACSindex;
} mstudioflexdesc_t;

typedef struct mstudioflexcontroller_t
{
	int sztypeindex;
	int sznameindex;
	int localToGlobal;
	float min;
	float max;
} mstudioflexcontroller_t;

typedef enum FlexControllerRemapType_t
{
	FLEXCONTROLLER_REMAP_PASSTHRU = 0,
	FLEXCONTROLLER_REMAP_2WAY,
	FLEXCONTROLLER_REMAP_NWAY,
	FLEXCONTROLLER_REMAP_EYELID
} FlexControllerRemapType_t;

typedef struct CStudioHdr CStudioHdr;
typedef struct mstudioflexcontrollerui_t
{
	int sznameindex;

	int szindex0;
	int szindex1;
	int szindex2;

	unsigned char remaptype;
	bool stereo;
	byte unused[2];
} mstudioflexcontrollerui_t;

typedef struct mstudiovertanim_t
{
	unsigned short index;
	byte speed;
	byte side;

	/*union
	{
	    short delta[3];
	    float16 flDelta[3];
	};

	union
	{
	    short ndelta[3];
	    float16 flNDelta[3];
	};*/
} mstudiovertanim_t;

typedef struct mstudiovertanim_wrinkle_t
{
	mstudiovertanim_t m_mstudiovertanim_t;
	short wrinkledelta;
} mstudiovertanim_wrinkle_t;

typedef enum StudioVertAnimType_t
{
	STUDIO_VERT_ANIM_NORMAL = 0,
	STUDIO_VERT_ANIM_WRINKLE
} StudioVertAnimType_t;

typedef struct mstudioflex_t
{
	int flexdesc;
	float target0;
	float target1;
	float target2;
	float target3;
	int numverts;
	int vertindex;
	int flexpair;
	unsigned char vertanimtype;
	unsigned char unusedchar[3];
	int unused[6];
} mstudioflex_t;

typedef struct mstudioflexop_t
{
	int op;
	union
	{
		int index;
		float value;
	} d;
} mstudioflexop_t;

typedef struct mstudioflexrule_t
{
	int flex;
	int numops;
	int opindex;
} mstudioflexrule_t;

typedef struct mstudioboneweight_t
{
	float weight[MAX_NUM_BONES_PER_VERT];
	char bone[MAX_NUM_BONES_PER_VERT];
	byte numbones;
} mstudioboneweight_t;

typedef struct mstudiovertex_t
{
	mstudioboneweight_t m_BoneWeights;
	Vector m_vecPosition;
	Vector m_vecNormal;
	Vector2D m_vecTexCoord;
} mstudiovertex_t;

typedef struct mstudiotexture_t
{
	int sznameindex;
	int flags;
	int used;
	int unused1;
	IMaterial* material;
	void* clientmaterial;
	int unused[10];
} mstudiotexture_t;

typedef struct mstudioeyeball_t
{
	int sznameindex;
	int bone;
	Vector org;
	float zoffset;
	float radius;
	Vector up;
	Vector forward;
	int texture;
	int unused1;
	float iris_scale;
	int unused2;
	int upperflexdesc[3];
	int lowerflexdesc[3];
	float uppertarget[3];
	float lowertarget[3];
	int upperlidflexdesc;
	int lowerlidflexdesc;
	int unused[4];
	bool m_bNonFACS;
	char unused3[3];
	int unused4[7];
} mstudioeyeball_t;

typedef struct mstudioiklink_t
{
	int bone;
	Vector kneeDir;
	Vector unused0;
} mstudioiklink_t;

typedef struct mstudioikchain_t
{
	int sznameindex;
	int linktype;
	int numlinks;
	int linkindex;
} mstudioikchain_t;

typedef struct mstudioiface_t
{
	unsigned short a, b, c;
} mstudioiface_t;

typedef struct mstudiomodel_t mstudiomodel_t;

typedef struct mstudio_modelvertexdata_t
{
	void* pVertexData;
	void* pTangentData;
} mstudio_modelvertexdata_t;

typedef struct mstudio_meshvertexdata_t
{
	mstudio_modelvertexdata_t* modelvertexdata;
	int numLODVertexes[MAX_NUM_LODS];
} mstudio_meshvertexdata_t;

typedef struct mstudiomesh_t
{
	int material;
	int modelindex;
	int numvertices;
	int vertexoffset;
	int numflexes;
	int flexindex;
	int materialtype;
	int materialparam;
	int meshid;
	Vector center;
	mstudio_meshvertexdata_t vertexdata;
	int unused[8];
} mstudiomesh_t;

typedef struct mstudiomodel_t
{
	char name[64];
	int type;
	float boundingradius;
	int nummeshes;
	int meshindex;
	int numvertices;
	int vertexindex;
	int tangentsindex;
	int numattachments;
	int attachmentindex;
	int numeyeballs;
	int eyeballindex;
	mstudio_modelvertexdata_t vertexdata;
	int unused[8];
} mstudiomodel_t;

typedef enum studiomeshgroupflags_t
{
	MESHGROUP_IS_FLEXED = 0x1,
	MESHGROUP_IS_HWSKINNED = 0x2,
	MESHGROUP_IS_DELTA_FLEXED = 0x4
} studiomeshgroupflags_t;

typedef struct studiomeshgroup_t
{
	IMesh* m_pMesh;
	int m_NumStrips;
	int m_Flags;
	StripHeader_t* m_pStripData;
	unsigned short* m_pGroupIndexToMeshIndex;
	int m_NumVertices;
	int* m_pUniqueTris;
	unsigned short* m_pIndices;
	bool m_MeshNeedsRestore;
	short m_ColorMeshID;
	IMorph* m_pMorph;
} studiomeshgroup_t;

typedef struct studiomeshdata_t
{
	int m_NumGroup;
	studiomeshgroup_t* m_pMeshGroup;
} studiomeshdata_t;

typedef struct studioloddata_t
{
	studiomeshdata_t* m_pMeshData;
	float m_SwitchPoint;
	int numMaterials;
	IMaterial** ppMaterials;
	int* pMaterialFlags;
	int* m_pHWMorphDecalBoneRemap;
	int m_nDecalBoneCount;
} studioloddata_t;

typedef struct studiohwdata_t
{
	int m_RootLOD;
	int m_NumLODs;
	studioloddata_t* m_pLODs;
	int m_NumStudioMeshes;
} studiohwdata_t;

typedef struct mstudiobodyparts_t
{
	int sznameindex;
	int nummodels;
	int base;
	int modelindex;
} mstudiobodyparts_t;

typedef struct mstudiomouth_t
{
	int bone;
	Vector forward;
	int flexdesc;
} mstudiomouth_t;

typedef struct mstudiohitboxset_t
{
	int sznameindex;
	int numhitboxes;
	int hitboxindex;
} mstudiohitboxset_t;

typedef struct mstudiosrcbonetransform_t
{
	int sznameindex;
	matrix3x4 pretransform;
	matrix3x4 posttransform;
} mstudiosrcbonetransform_t;

typedef struct virtualgroup_t
{
	void* cache;
	CUtlVector boneMap;
	CUtlVector masterBone;
	CUtlVector masterSeq;
	CUtlVector masterAnim;
	CUtlVector masterAttachment;
	CUtlVector masterPose;
	CUtlVector masterNode;
} virtualgroup_t;

typedef struct virtualsequence_t
{
	int flags;
	int activity;
	int group;
	int index;
} virtualsequence_t;

typedef struct virtualgeneric_t
{
	int group;
	int index;
} virtualgeneric_t;

typedef struct virtualmodel_t virtualmodel_t;
typedef struct thinModelVertices_t thinModelVertices_t;

#define MODEL_VERTEX_FILE_ID (('V' << 24) + ('S' << 16) + ('D' << 8) + 'I')
#define MODEL_VERTEX_FILE_VERSION 4
#define MODEL_VERTEX_FILE_THIN_ID (('V' << 24) + ('C' << 16) + ('D' << 8) + 'I')

typedef struct vertexFileHeader_t
{
	int id;
	int version;
	int checksum;
	int numLODs;
	int numLODVertexes[MAX_NUM_LODS];
	int numFixups;
	int fixupTableStart;
	int vertexDataStart;
	int tangentDataStart;
} vertexFileHeader_t;

typedef struct vertexFileFixup_t
{
	int lod;
	int sourceVertexID;
	int numVertexes;
} vertexFileFixup_t;

#define STUDIOHDR_FLAGS_AUTOGENERATED_HITBOX 0x00000001
#define STUDIOHDR_FLAGS_USES_ENV_CUBEMAP 0x00000002
#define STUDIOHDR_FLAGS_FORCE_OPAQUE 0x00000004
#define STUDIOHDR_FLAGS_TRANSLUCENT_TWOPASS 0x00000008
#define STUDIOHDR_FLAGS_STATIC_PROP 0x00000010
#define STUDIOHDR_FLAGS_USES_FB_TEXTURE 0x00000020
#define STUDIOHDR_FLAGS_HASSHADOWLOD 0x00000040
#define STUDIOHDR_FLAGS_USES_BUMPMAPPING 0x00000080
#define STUDIOHDR_FLAGS_USE_SHADOWLOD_MATERIALS 0x00000100
#define STUDIOHDR_FLAGS_OBSOLETE 0x00000200
#define STUDIOHDR_FLAGS_UNUSED 0x00000400
#define STUDIOHDR_FLAGS_NO_FORCED_FADE 0x00000800
#define STUDIOHDR_FLAGS_FORCE_PHONEME_CROSSFADE 0x00001000
#define STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT 0x00002000
#define STUDIOHDR_FLAGS_FLEXES_CONVERTED 0x00004000
#define STUDIOHDR_FLAGS_BUILT_IN_PREVIEW_MODE 0x00008000
#define STUDIOHDR_FLAGS_AMBIENT_BOOST 0x00010000
#define STUDIOHDR_FLAGS_DO_NOT_CAST_SHADOWS 0x00020000
#define STUDIOHDR_FLAGS_CAST_TEXTURE_SHADOWS 0x00040000
#define STUDIOHDR_FLAGS_VERT_ANIM_FIXED_POINT_SCALE 0x00200000

typedef struct studiohdr2_t
{
	int numsrcbonetransform;
	int srcbonetransformindex;
	int illumpositionattachmentindex;
	float flMaxEyeDeflection;
	int linearboneindex;
	int sznameindex;
	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;
	int reserved[56];
} studiohdr2_t;

typedef struct studiohdr_t
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	Vector eyeposition;
	Vector illumposition;
	Vector hull_min;
	Vector hull_max;
	Vector view_bbmin;
	Vector view_bbmax;
	int flags;
	int numbones;
	int boneindex;
	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;

	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;
	int activitylistversion;
	int eventsindexed;
	int numtextures;
	int textureindex;
	int numcdtextures;
	int cdtextureindex;
	int numskinref;
	int numskinfamilies;
	int skinindex;
	int numbodyparts;
	int bodypartindex;
	int numlocalattachments;
	int localattachmentindex;
	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;
	int numflexdesc;
	int flexdescindex;
	int numflexcontrollers;
	int flexcontrollerindex;
	int numflexrules;
	int flexruleindex;
	int numikchains;
	int ikchainindex;
	int nummouths;
	int mouthindex;
	int numlocalposeparameters;
	int localposeparamindex;
	int surfacepropindex;
	int keyvalueindex;
	int keyvaluesize;
	int numlocalikautoplaylocks;
	int localikautoplaylockindex;
	float mass;
	int contents;
	int numincludemodels;
	int includemodelindex;
	void* virtualModel;
	int szanimblocknameindex;
	int numanimblocks;
	int animblockindex;
	void* animblockModel;
	int bonetablebynameindex;
	void* pVertexBase;
	void* pIndexBase;
	byte constdirectionallightdot;
	byte rootLOD;
	byte numAllowedRootLODs;
	byte unused[1];
	int unused4;
	int numflexcontrollerui;
	int flexcontrolleruiindex;
	float flVertAnimFixedPointScale;
	int unused3[1];
	int studiohdr2index;
	int unused2[1];
} studiohdr_t;

typedef struct IDataCache IDataCache;
typedef struct IMDLCache IMDLCache;

typedef struct flexweight_t
{
	int key;
	float weight;
	float influence;
} flexweight_t;

typedef struct flexsetting_t
{
	int nameindex;

	int obsolete1;
	int numsettings;
	int index;
	int obsolete2;
	int settingindex;
} flexsetting_t;

typedef struct flexsettinghdr_t
{
	int id;
	int version;
	char name[64];
	int length;
	int numflexsettings;
	int flexsettingindex;
	int nameindex;
	int numindexes;
	int indexindex;

	int numkeys;
	int keynameindex;
	int keymappingindex;
} flexsettinghdr_t;

#define STUDIO_CONST 1
#define STUDIO_FETCH1 2
#define STUDIO_FETCH2 3
#define STUDIO_ADD 4
#define STUDIO_SUB 5
#define STUDIO_MUL 6
#define STUDIO_DIV 7
#define STUDIO_NEG 8
#define STUDIO_EXP 9
#define STUDIO_OPEN 10
#define STUDIO_CLOSE 11
#define STUDIO_COMMA 12
#define STUDIO_MAX 13
#define STUDIO_MIN 14
#define STUDIO_2WAY_0 15
#define STUDIO_2WAY_1 16
#define STUDIO_NWAY 17
#define STUDIO_COMBO 18
#define STUDIO_DOMINATE 19
#define STUDIO_DME_LOWER_EYELID 20
#define STUDIO_DME_UPPER_EYELID 21
#define STUDIO_X 0x00000001
#define STUDIO_Y 0x00000002
#define STUDIO_Z 0x00000004
#define STUDIO_XR 0x00000008
#define STUDIO_YR 0x00000010
#define STUDIO_ZR 0x00000020
#define STUDIO_LX 0x00000040
#define STUDIO_LY 0x00000080
#define STUDIO_LZ 0x00000100
#define STUDIO_LXR 0x00000200
#define STUDIO_LYR 0x00000400
#define STUDIO_LZR 0x00000800
#define STUDIO_LINEAR 0x00001000
#define STUDIO_TYPES 0x0003FFFF
#define STUDIO_RLOOP 0x00040000
#define STUDIO_LOOPING 0x0001
#define STUDIO_SNAP 0x0002
#define STUDIO_DELTA 0x0004
#define STUDIO_AUTOPLAY 0x0008
#define STUDIO_POST 0x0010
#define STUDIO_ALLZEROS 0x0020
#define STUDIO_CYCLEPOSE 0x0080
#define STUDIO_REALTIME 0x0100
#define STUDIO_LOCAL 0x0200
#define STUDIO_HIDDEN 0x0400
#define STUDIO_OVERRIDE 0x0800
#define STUDIO_ACTIVITY 0x1000
#define STUDIO_EVENT 0x2000
#define STUDIO_WORLD 0x4000
#define STUDIO_AL_POST 0x0010
#define STUDIO_AL_SPLINE 0x0040
#define STUDIO_AL_XFADE 0x0080
#define STUDIO_AL_NOBLEND 0x0200
#define STUDIO_AL_LOCAL 0x1000
#define STUDIO_AL_POSE 0x4000

#endif
