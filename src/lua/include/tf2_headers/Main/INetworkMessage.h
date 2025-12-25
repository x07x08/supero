#if !defined(MainINetworkMessage_Included)
#define MainINetworkMessage_Included

#include "Main/INetMessage.h"

typedef struct INetChannel INetChannel;

enum
{
	NETWORKSYSTEM_FIRST_GROUP = 1
};

typedef struct INetworkMessage INetworkMessage;

typedef struct INetworkMessage_VMT
{
	void (*SetNetChannel)(INetworkMessage*, INetChannel* netchan);
	void (*SetReliable)(INetworkMessage*, bool state);
	bool (*ReadFromBuffer)(INetworkMessage*, bf_read* buffer);
	bool (*WriteToBuffer)(INetworkMessage*, bf_write* buffer);
	bool (*IsReliable)(INetworkMessage*); // const
	int (*GetGroup)(INetworkMessage*); // const
	int (*GetType)(INetworkMessage*); // const
	const char* (*GetGroupName)(INetworkMessage*); // const
	const char* (*GetName)(INetworkMessage*); // const
	INetChannel* (*GetNetChannel)(INetworkMessage*); // const
	const char* (*ToString)(INetworkMessage*); // const
	void (*Release)(INetworkMessage*);
	//virtual ~INetworkMessage() {};
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)
} INetworkMessage_VMT;

struct INetworkMessage
{
	INetworkMessage_VMT* vmt;
};

typedef INetworkMessage_VMT CNetworkMessage_VMT;

typedef struct CNetworkMessage// : public INetworkMessage
{
	CNetworkMessage_VMT* vmt;

	bool m_bReliable;
	INetChannel* m_pNetChannel;
	//virtual ~CNetworkMessage() {};
} CNetworkMessage;

#endif
