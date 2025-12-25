#if !defined(MainINetChannel_Included)
#define MainINetChannel_Included

#include "Main/NetChannel.h"
#include "Main/INetChannelInfo.h"
#include "Main/INetMessage.h"
#include "Misc/CommonMacros.h"

typedef struct IDemoRecorder IDemoRecorder;
typedef struct INetChannelHandler INetChannelHandler;

typedef struct INetChannel INetChannel;

typedef struct INetChannel_VMT
{
	INetChannelInfo_VMT vmt_INetChannelInfo;

	//virtual ~INetChannel(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*SetDataRate)(INetChannel*, float rate);
	bool (*RegisterMessage)(INetChannel*, INetMessage* msg);
	// bool (*StartStreaming)(INetChannel*, unsigned int challengeNr);
	// void (*ResetStreaming)(INetChannel*);
	void (*SetTimeout)(INetChannel*, float seconds);
	void (*SetDemoRecorder)(INetChannel*, IDemoRecorder* recorder);
	void (*SetChallengeNr)(INetChannel*, unsigned int chnr);
	void (*Reset)(INetChannel*);
	void (*Clear)(INetChannel*);
	void (*Shutdown)(INetChannel*, const char* reason);
	void (*ProcessPlayback)(INetChannel*);
	// bool (*ProcessStream)(INetChannel*);
	void (*ProcessPacket)(INetChannel*, struct netpacket_s* packet, bool bHasHeader);
	bool (*SendNetMsg)(INetChannel*, INetMessage* msg, bool bForceReliable, bool bVoice);
	bool (*SendData)(INetChannel*, bf_write* msg, bool bReliable);
	bool (*SendFile)(INetChannel*, const char* filename, unsigned int transferID);
	void (*DenyFile)(INetChannel*, const char* filename, unsigned int transferID);
	void (*RequestFile_OLD)(INetChannel*, const char* filename, unsigned int transferID);
	void (*SetChoked)(INetChannel*);
	int (*SendDatagram)(INetChannel*, bf_write* data);
	bool (*Transmit)(INetChannel*, bool onlyReliable);
	const netadr_t* (*GetRemoteAddress)(INetChannel*); // const
	INetChannelHandler* (*GetMsgHandler)(INetChannel*); // const
	int (*GetDropNumber)(INetChannel*); // const
	int (*GetSocket)(INetChannel*); // const
	unsigned int (*GetChallengeNr)(INetChannel*); // const
	void (*GetSequenceData)(INetChannel*, int* nOutSequenceNr, int* nInSequenceNr, int* nOutSequenceNrAck);
	void (*SetSequenceData)(INetChannel*, int nOutSequenceNr, int nInSequenceNr, int nOutSequenceNrAck);
	void (*UpdateMessageStats)(INetChannel*, int msggroup, int bits);
	bool (*CanPacket)(INetChannel*); // const
	bool (*IsOverflowed)(INetChannel*); // const
	bool (*IsTimedOut)(INetChannel*); // const
	bool (*HasPendingReliableData)(INetChannel*);
	void (*SetFileTransmissionMode)(INetChannel*, bool bBackgroundMode);
	void (*SetCompressionMode)(INetChannel*, bool bUseCompression);
	unsigned int (*RequestFile)(INetChannel*, const char* filename);

	// float (*GetTimeSinceLastReceived)(INetChannel*); // const

	void (*SetMaxBufferSize)(INetChannel*, bool bReliable, int nBytes, bool bVoice);
	bool (*IsNull)(INetChannel*); // const
	int (*GetNumBitsWritten)(INetChannel*, bool bReliable);
	void (*SetInterpolationAmount)(INetChannel*, float flInterpolationAmount);
	void (*SetRemoteFramerate)(INetChannel*, float flFrameTime, float flFrameTimeStdDeviation);
	void (*SetMaxRoutablePayloadSize)(INetChannel*, int nSplitSize);
	int (*GetMaxRoutablePayloadSize)(INetChannel*);
	int (*GetProtocolVersion)(INetChannel*);
} INetChannel_VMT;

struct INetChannel
{
	INetChannel_VMT* vmt;
};

typedef struct CNetChannel CNetChannel;

typedef struct CNetChannel_VMT
{
	INetChannel_VMT vmt_INetChannel;

	ConnectionStatus_t (*GetConnectionState)(CNetChannel*);

} CNetChannel_VMT;

struct CNetChannel
{
	CNetChannel_VMT* vmt;

	ConnectionStatus_t m_ConnectionState;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
	int m_PacketDrop;
	bf_write m_StreamReliable;
	byte m_ReliableDataBuffer[8 * 1024];
	CUtlVector m_ReliableDataBufferMP; // byte
	bf_write m_StreamUnreliable;
	byte m_UnreliableDataBuffer[NET_MAX_DATAGRAM_PAYLOAD];
	CUDPSocket* m_pSocket;
	int m_StreamSocket;
	unsigned int m_MaxReliablePayloadSize;
	netadr_t remote_address;
	float last_received;
	float connect_time;
	int m_Rate;
	float m_fClearTime;
	float m_Timeout;
	char m_Name[32];
	INetworkMessageHandler* m_MessageHandler;
};

#endif
