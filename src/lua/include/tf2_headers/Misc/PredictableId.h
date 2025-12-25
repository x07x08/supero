#if !defined(MiscPredictableId_Included)
#define MiscPredictableId_Included

typedef struct CPredictableId__m_PredictableID
{
	unsigned int ack : 1;       // 1
	unsigned int player : 5;    // 6
	unsigned int command : 10;  // 16
	unsigned int hash : 12;     // 28
	unsigned int instance : 4;  // 32
} CPredictableId__m_PredictableID;

typedef struct CPredictableId
{
	CPredictableId__m_PredictableID m_PredictableID;
} CPredictableId;

#endif
