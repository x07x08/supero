#if !defined(MiscNetAdr_Included)
#define MiscNetAdr_Included

typedef enum netadrtype_t
{
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
} netadrtype_t;

typedef struct netadr_s
{
	netadrtype_t type;
	unsigned char ip[4];
	unsigned short port;
} netadr_t;

#endif
