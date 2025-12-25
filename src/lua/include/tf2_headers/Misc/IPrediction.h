#if !defined(MiscIPrediction_Included)
#define MiscIPrediction_Included

#include "Types.h"
#include "Misc/CommonMacros.h"

typedef struct IPrediction IPrediction;

typedef struct IPrediction_VMT
{
	//virtual ~IPrediction(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*Init)(IPrediction*);
	void (*Shutdown)(IPrediction*);
	void (*Update)(IPrediction*, int startframe, bool validframe, int incoming_acknowledged, int outgoing_command);
	void (*PreEntityPacketReceived)(IPrediction*, int commands_acknowledged, int current_world_update_packet);
	void (*PostEntityPacketReceived)(IPrediction*);
	void (*PostNetworkDataReceived)(IPrediction*, int commands_acknowledged);
	void (*OnReceivedUncompressedPacket)(IPrediction*);
	void (*GetViewOrigin)(IPrediction*, Vector* org);
	void (*SetViewOrigin)(IPrediction*, Vector* org);
	void (*GetViewAngles)(IPrediction*, QAngle* ang);
	void (*SetViewAngles)(IPrediction*, QAngle* ang);
	void (*GetLocalViewAngles)(IPrediction*, QAngle* ang);
	void (*SetLocalViewAngles)(IPrediction*, QAngle* ang);
} IPrediction_VMT;

struct IPrediction
{
	IPrediction_VMT* vmt;
};

#endif
