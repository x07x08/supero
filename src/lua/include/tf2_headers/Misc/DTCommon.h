#if !defined(MiscDTCommon_Included)
#define MiscDTCommon_Included

typedef struct SendProp SendProp;
typedef struct DVariant DVariant;

#define MAX_DATATABLES 1024
#define MAX_DATATABLE_PROPS 4096
#define MAX_ARRAY_ELEMENTS 2048
#define HIGH_DEFAULT -121121.121121f
#define BITS_FULLRES -1
#define BITS_WORLDCOORD -2
#define DT_MAX_STRING_BITS 9
#define DT_MAX_STRING_BUFFERSIZE (1 << DT_MAX_STRING_BITS)
#define STRINGBUFSIZE(className, varName) sizeof(((className*)0)->varName)
#define PROPSIZEOF(className, varName) sizeof(((className*)0)->varName)
#define SPROP_UNSIGNED (1 << 0)
#define SPROP_COORD (1 << 1)
#define SPROP_NOSCALE (1 << 2)
#define SPROP_ROUNDDOWN (1 << 3)
#define SPROP_ROUNDUP (1 << 4)
#define SPROP_NORMAL (1 << 5)
#define SPROP_EXCLUDE (1 << 6)
#define SPROP_XYZE (1 << 7)
#define SPROP_INSIDEARRAY (1 << 8)
#define SPROP_PROXY_ALWAYS_YES (1 << 9)
#define SPROP_CHANGES_OFTEN (1 << 10)
#define SPROP_IS_A_VECTOR_ELEM (1 << 11)
#define SPROP_COLLAPSIBLE (1 << 12)
#define SPROP_COORD_MP (1 << 13)
#define SPROP_COORD_MP_LOWPRECISION (1 << 14)
#define SPROP_COORD_MP_INTEGRAL (1 << 15)
#define SPROP_NUMFLAGBITS_NETWORKED 16
#define SPROP_ENCODED_AGAINST_TICKCOUNT (1 << 16)
#define SPROP_NUMFLAGBITS 17
#define SIZEOF_IGNORE -1

typedef enum SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
#if 0
	DPT_Quaternion,
#endif
	DPT_NUMSendPropTypes
} SendPropType;

struct DVariant
{
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
#if 0
		float m_Vector[4];
#else
		float m_Vector[3];
#endif
	};
	SendPropType m_Type;
};

#endif
