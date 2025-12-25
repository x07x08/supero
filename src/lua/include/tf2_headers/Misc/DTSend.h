#if !defined(MiscDTSend_Included)
#define MiscDTSend_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Misc/DTCommon.h"
// #include "bitvec.h"

#define ABSOLUTE_PLAYER_LIMIT 255
#define SP_MODEL_INDEX_BITS 13

typedef struct SendProp SendProp;
typedef struct CSendProxyRecipients CSendProxyRecipients;
typedef struct RecvProp RecvProp;
typedef struct SendTable SendTable;
typedef struct CSendTablePrecalc CSendTablePrecalc;
typedef struct CNonModifiedPointerProxy CNonModifiedPointerProxy;
typedef struct CStandardSendProxiesV1 CStandardSendProxiesV1;
typedef struct CStandardSendProxies CStandardSendProxies;

typedef void (*SendVarProxyFn)(const SendProp* pProp, const void* pStructBase, const void* pData, DVariant* pOut, int iElement, int objectID);

typedef void* (*SendTableProxyFn)(
	const SendProp* pProp,
	const void* pStructBase,
	const void* pData,
	CSendProxyRecipients* pRecipients,
	int objectID);

struct CNonModifiedPointerProxy
{
	SendTableProxyFn m_Fn;
	CNonModifiedPointerProxy* m_pNext;
};

struct CStandardSendProxiesV1
{
	SendVarProxyFn m_Int8ToInt32;
	SendVarProxyFn m_Int16ToInt32;
	SendVarProxyFn m_Int32ToInt32;

	SendVarProxyFn m_UInt8ToInt32;
	SendVarProxyFn m_UInt16ToInt32;
	SendVarProxyFn m_UInt32ToInt32;

	SendVarProxyFn m_FloatToFloat;
	SendVarProxyFn m_VectorToVector;

#ifdef SUPPORTS_INT64
	SendVarProxyFn m_Int64ToInt64;
	SendVarProxyFn m_UInt64ToInt64;
#endif
};

struct CStandardSendProxies
{
	CStandardSendProxiesV1 proxiesV1;

	SendTableProxyFn m_DataTableToDataTable;
	SendTableProxyFn m_SendLocalDataTable;
	CNonModifiedPointerProxy** m_ppNonModifiedPointerProxies;
};

// extern CStandardSendProxies g_StandardSendProxies;

#define MAX_DATATABLE_PROXIES 32

struct CSendProxyRecipients
{
	char m_Bits[32];
};

typedef int (*ArrayLengthSendProxyFn)(const void* pStruct, int objectID);

#define DATATABLE_PROXY_INDEX_NOPROXY 255
#define DATATABLE_PROXY_INDEX_INVALID 254

struct SendProp
{
	void* vmt; //DTOR

	RecvProp* m_pMatchingRecvProp;
	SendPropType m_Type;
	int m_nBits;
	float m_fLowValue;
	float m_fHighValue;
	SendProp* m_pArrayProp;
	ArrayLengthSendProxyFn m_ArrayLengthProxy;
	int m_nElements;
	int m_ElementStride;
	const char* m_pExcludeDTName;
	const char* m_pParentArrayPropName;
	const char* m_pVarName;
	float m_fHighLowMul;

	int m_Flags;
	SendVarProxyFn m_ProxyFn;
	SendTableProxyFn m_DataTableProxyFn;
	SendTable* m_pDataTable;
	int m_Offset;
	const void* m_pExtraData;
};

struct SendTable
{
	SendProp* m_pProps;
	int m_nProps;
	const char* m_pNetTableName;
	CSendTablePrecalc* m_pPrecalc;

	bool m_bInitialized : 1;
	bool m_bHasBeenWritten : 1;
	bool m_bHasPropsEncodedAgainstCurrentTickCount : 1;
};

#endif
