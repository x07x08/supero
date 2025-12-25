#if !defined(MainINetworkSystem_Included)
#define MainINetworkSystem_Included

#include "Main/INetworkMessage.h"
#include "Misc/IAppSystem.h"
#include "Misc/NetAdr.h"

enum
{
	NETWORKSYSTEM_DEFAULT_SERVER_PORT = 27001,
	NETWORKSYSTEM_DEFAULT_CLIENT_PORT = 27002
};

typedef int ConnectionHandle_t;

typedef enum ConnectionStatus_t
{
	CONNECTION_STATE_DISCONNECTED = 0,
	CONNECTION_STATE_CONNECTING,
	CONNECTION_STATE_CONNECTION_FAILED,
	CONNECTION_STATE_CONNECTED
} ConnectionStatus_t;

typedef enum NetworkEventType_t
{
	NETWORK_EVENT_CONNECTED = 0,
	NETWORK_EVENT_DISCONNECTED,
	NETWORK_EVENT_MESSAGE_RECEIVED,
} NetworkEventType_t;

typedef struct NetworkEvent_t
{
	NetworkEventType_t m_nType;
} NetworkEvent_t;

struct NetworkConnectionEvent_t
{
	NetworkEvent_t m_NetworkEvent_t;
	void* m_pChannel;
};

struct NetworkDisconnectionEvent_t
{
	NetworkEvent_t m_NetworkEvent_t;
	void* m_pChannel;
};

struct NetworkMessageReceivedEvent_t
{
	NetworkEvent_t m_NetworkEvent_t;
	void* m_pChannel;
	INetworkMessage* m_pNetworkMessage;
};

typedef struct INetworkSystem INetworkSystem;

typedef struct INetworkSystem_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	bool (*RegisterMessage)(INetworkSystem*, INetworkMessage* msg);
	bool (*StartServer)(INetworkSystem*, unsigned short nServerListenPort);
	void (*ShutdownServer)(INetworkSystem*);
	void (*ServerReceiveMessages)(INetworkSystem*);
	void (*ServerSendMessages)(INetworkSystem*);
	bool (*StartClient)(INetworkSystem*, unsigned short nClientListenPort);
	void (*ShutdownClient)(INetworkSystem*);
	void (*ClientSendMessages)(INetworkSystem*);
	void (*ClientReceiveMessages)(INetworkSystem*);
	void* (*ConnectClientToServer)(INetworkSystem*, const char* pServer, int nServerListenPort);
	void (*DisconnectClientFromServer)(INetworkSystem*, void* pChan);
	NetworkEvent_t* (*FirstNetworkEvent)(INetworkSystem*);
	NetworkEvent_t* (*NextNetworkEvent)(INetworkSystem*);
	const char* (*GetLocalHostName)(INetworkSystem*); // const
	const char* (*GetLocalAddress)(INetworkSystem*); // const
} INetworkSystem_VMT;

struct INetworkSystem
{
	INetworkSystem_VMT* vmt;
};

#endif
