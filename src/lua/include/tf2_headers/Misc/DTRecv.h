#if !defined(MiscDTRecv_Included)
#define MiscDTRecv_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Misc/DTCommon.h"

#define ADDRESSPROXY_NONE -1

typedef struct CRecvProxyData CRecvProxyData;
typedef struct CStandardRecvProxies CStandardRecvProxies;
typedef struct CRecvDecoder CRecvDecoder;
typedef struct RecvProp RecvProp;
typedef struct RecvTable RecvTable;

struct CRecvProxyData
{
	const RecvProp* m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

typedef void (*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);
typedef void (*ArrayLengthRecvProxyFn)(void* pStruct, int objectID, int currentArrayLength);
typedef void (*DataTableRecvVarProxyFn)(const RecvProp* pProp, void** pOut, void* pData, int objectID);

struct CStandardRecvProxies
{
	RecvVarProxyFn m_Int32ToInt8;
	RecvVarProxyFn m_Int32ToInt16;
	RecvVarProxyFn m_Int32ToInt32;
	RecvVarProxyFn m_FloatToFloat;
	RecvVarProxyFn m_VectorToVector;
};
//extern CStandardRecvProxies g_StandardRecvProxies;

struct RecvProp
{
	const char* m_pVarName;
	SendPropType m_RecvType;
	int m_Flags;
	int m_StringBufferSize;

	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	ArrayLengthRecvProxyFn m_ArrayLengthProxy;
	RecvVarProxyFn m_ProxyFn;
	DataTableRecvVarProxyFn m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

struct RecvTable
{
	RecvProp* m_pProps;
	int m_nProps;
	CRecvDecoder* m_pDecoder;
	const char* m_pNetTableName;

	bool m_bInitialized;
	bool m_bInMainList;
};

#endif
