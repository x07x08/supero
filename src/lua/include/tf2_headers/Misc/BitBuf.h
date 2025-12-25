#if !defined(MiscBitBuf_Included)
#define MiscBitBuf_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#define BITS_PER_INT 32

typedef struct bf_write bf_write;
typedef struct bf_write bf_read;

struct bf_write
{
	unsigned long* m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
	bool m_bOverflow;
	bool m_bAssertOnOverflow;
	const char* m_pDebugName;
};

#endif
