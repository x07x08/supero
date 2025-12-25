#if !defined(IntfCPrediction_Included)
#define IntfCPrediction_Included

#include "Misc/IPrediction.h"
#include "Main/CBaseHandle.h"
#include "Main/UtlVector.h"
#include "Definitions.h"

// MAKE_SIGNATURE(CPrediction_RestoreEntityToPredictedFrame, "client.dll", "40 55 48 83 EC ? 8B EA", 0x0);

typedef struct CMoveData CMoveData;
typedef struct CUserCmd CUserCmd;
typedef struct CBasePlayer CBasePlayer;
typedef struct IMoveHelper IMoveHelper;

typedef struct CPrediction CPrediction;

typedef struct CPrediction_VMT
{
	IPrediction_VMT vmt_IPrediction;

	//virtual ~CPrediction() {};
	bool (*InPrediction)(CPrediction*);
	bool (*IsFirstTimePredicted)(CPrediction*);
	int (*GetIncomingPacketNumber)(CPrediction*);
	void (*RunCommand)(CPrediction*, CBasePlayer* player, CUserCmd* ucmd, IMoveHelper* moveHelper);
	void (*SetupMove)(CPrediction*, CBasePlayer* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move);
	void (*FinishMove)(CPrediction*, CBasePlayer* player, CUserCmd* ucmd, CMoveData* move);
	void (*SetIdealPitch)(CPrediction*, CBasePlayer* player, const Vec3* origin, const Vec3* angles, const Vec3* viewheight);
	void (*_Update)(CPrediction*, bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command);

	// SIGNATURE_ARGS(RestoreEntityToPredictedFrame, void, CPrediction, (int predicted_frame), this, predicted_frame);
} CPrediction_VMT;

struct CPrediction
{
	CPrediction_VMT* vmt;

	CHandle m_hLastGround;
	bool m_bInPrediction;
	bool m_bFirstTimePredicted;
	bool m_bOldCLPredictValue;
	bool m_bEnginePaused;
	int m_nPreviousStartFrame;
	int m_nCommandsPredicted;
	int m_nServerCommandsAcknowledged;
	int m_bPreviousAckHadErrors;
	int m_bPreviousAckErrorTriggersFullLatchReset;
	CUtlVector m_EntsWithPredictionErrorsInLastAck;
	int m_nIncomingPacketNumber;
	float m_flIdealPitch;
};

#define CPrediction_REFL "CPrediction"
#define CPrediction_SIGNATURE "VClientPrediction001"
#define CPrediction_MODULE "client"

#endif
