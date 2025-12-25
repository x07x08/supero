#if !defined(MainNetChannel_Included)
#define MainNetChannel_Included

#include "Main/INetworkSystem.h"
#include "Main/UtlVector.h"
#include "Misc/BitBuf.h"
#include "Misc/BaseTypes.h"

enum
{
	FRAG_NORMAL_STREAM = 0,
	FRAG_FILE_STREAM,
	MAX_STREAMS
};

#define NET_MAX_DATAGRAM_PAYLOAD 1400
#define NET_MAX_PAYLOAD_BITS 11
#define DEFAULT_RATE 10000
#define SIGNON_TIME_OUT 120.0f
#define CONNECTION_PROBLEM_TIME 15.0f

#define MAX_RATE 50000
#define MIN_RATE 100

#define FRAGMENT_BITS 8
#define FRAGMENT_SIZE (1 << FRAGMENT_BITS)
#define MAX_FILE_SIZE_BITS 26
#define MAX_FILE_SIZE ((1 << MAX_FILE_SIZE_BITS) - 1)

#define NET_MAX_PAYLOAD 4000
#define NET_MAX_MESSAGE 4096
#define MIN_ROUTEABLE_PACKET 16
#define MAX_ROUTEABLE_PACKET 1400
#define UDP_HEADER_SIZE 28

#define PACKET_FLAG_RELIABLE (1 << 0)
#define PACKET_FLAG_CHOKED (1 << 1)

typedef struct CUDPSocket CUDPSocket;
typedef struct CUtlBuffer CUtlBuffer;
typedef struct CNetPacket CNetPacket;

typedef struct INetChannel INetChannel;

typedef struct INetworkMessageHandler INetworkMessageHandler;

typedef struct INetworkMessageHandler_VMT
{
	void (*OnConnectionClosing)(INetworkMessageHandler*, INetChannel* channel, char const* reason);
	void (*OnConnectionStarted)(INetworkMessageHandler*, INetChannel* channel);
	void (*OnPacketStarted)(INetworkMessageHandler*, int inseq, int outseq);
	void (*OnPacketFinished)(INetworkMessageHandler*);
	//virtual ~INetworkMessageHandler() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)
} INetworkMessageHandler_VMT;

struct INetworkMessageHandler
{
	INetworkMessageHandler_VMT* vmt;
};

typedef struct INetChannelHandler INetChannelHandler;

typedef struct INetChannelHandler_VMT
{
	//virtual ~INetChannelHandler(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	void (*ConnectionStart)(INetChannelHandler*, INetChannel* chan);
	void (*ConnectionClosing)(INetChannelHandler*, const char* reason);
	void (*ConnectionCrashed)(INetChannelHandler*, const char* reason);
	void (*PacketStart)(INetChannelHandler*, int incoming_sequence, int outgoing_acknowledged);
	void (*PacketEnd)(INetChannelHandler*);
	void (*FileRequested)(INetChannelHandler*, const char* fileName, unsigned int transferID);
	void (*FileReceived)(INetChannelHandler*, const char* fileName, unsigned int transferID);
	void (*FileDenied)(INetChannelHandler*, const char* fileName, unsigned int transferID);
} INetChannelHandler_VMT;

struct INetChannelHandler
{
	INetChannelHandler_VMT* vmt;
};

typedef struct CNetPacket CNetPacket;
//{
//	DECLARE_FIXEDSIZE_ALLOCATOR(CNetPacket);
//
// public:
//	netadr_t m_From;
//	CUDPSocket *m_pSource;
//	float m_flReceivedTime;
//	unsigned char *m_pData;
//	bf_read m_Message;
//	int m_nSizeInBytes;
//
// private:
//	int m_nRefCount;
//};

typedef struct IConnectionlessPacketHandler IConnectionlessPacketHandler;

typedef struct IConnectionlessPacketHandler_VMT
{
	bool (*ProcessConnectionlessPacket)(IConnectionlessPacketHandler*, CNetPacket* packet);
	//virtual ~IConnectionlessPacketHandler(void) {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)
} IConnectionlessPacketHandler_VMT;

struct IConnectionlessPacketHandler
{
	IConnectionlessPacketHandler_VMT* vmt;
};

typedef struct ILookupChannel ILookupChannel;

typedef struct ILookupChannel_VMT
{
	INetChannel* (*FindNetChannel)(ILookupChannel*, const netadr_t* from);
} ILookupChannel_VMT;

struct ILookupChannel
{
	ILookupChannel_VMT* vmt;
};

#endif
