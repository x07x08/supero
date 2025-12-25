#if !defined(MainINetChannelInfo_Included)
#define MainINetChannelInfo_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

typedef struct INetChannelInfo INetChannelInfo;

typedef struct INetChannelInfo_VMT
{
	const char* (*GetName)(INetChannelInfo*);
	const char* (*GetAddress)(INetChannelInfo*);
	float (*GetTime)(INetChannelInfo*);
	float (*GetTimeConnected)(INetChannelInfo*);
	int (*GetBufferSize)(INetChannelInfo*);
	int (*GetDataRate)(INetChannelInfo*);
	bool (*IsLoopback)(INetChannelInfo*);
	bool (*IsTimingOut)(INetChannelInfo*);
	bool (*IsPlayback)(INetChannelInfo*);
	float (*GetLatency)(INetChannelInfo*, int flow);
	float (*GetAvgLatency)(INetChannelInfo*, int flow);
	float (*GetAvgLoss)(INetChannelInfo*, int flow);
	float (*GetAvgChoke)(INetChannelInfo*, int flow);
	float (*GetAvgData)(INetChannelInfo*, int flow);
	float (*GetAvgPackets)(INetChannelInfo*, int flow);
	int (*GetTotalData)(INetChannelInfo*, int flow);
	int (*GetSequenceNr)(INetChannelInfo*, int flow);
	bool (*IsValidPacket)(INetChannelInfo*, int flow, int frame_number);
	float (*GetPacketTime)(INetChannelInfo*, int flow, int frame_number);
	int (*GetPacketBytes)(INetChannelInfo*, int flow, int frame_number, int group);
	bool (*GetStreamProgress)(INetChannelInfo*, int flow, int* received, int* total);
	float (*GetTimeSinceLastReceived)(INetChannelInfo*);
	float (*GetCommandInterpolationAmount)(INetChannelInfo*, int flow, int frame_number);
	void (*GetPacketResponseLatency)(INetChannelInfo*, int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke);
	void (*GetRemoteFramerate)(INetChannelInfo*, float* pflFrameTime, float* pflFrameTimeStdDeviation);
	float (*GetTimeoutSeconds)(INetChannelInfo*);
} INetChannelInfo_VMT;

struct INetChannelInfo
{
	INetChannelInfo_VMT* vmt;
};

#endif
