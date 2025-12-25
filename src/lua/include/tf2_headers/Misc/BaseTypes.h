#if !defined(MiscBaseTypes_Included)
#define MiscBaseTypes_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>
#endif

typedef int qboolean;
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef void* HANDLE;
typedef unsigned char byte;
typedef unsigned short word;
typedef wchar_t ucs2;

typedef enum ThreeState_t
{
	TRS_FALSE,
	TRS_TRUE,
	TRS_NONE
} ThreeState_t;

typedef float vec_t;

typedef struct color24
{
	byte r, g, b;
} color24;

typedef struct color32_s
{
	byte r, g, b, a;
} color32;

typedef struct colorVec
{
	unsigned r, g, b, a;
} colorVec;

typedef struct vrect_t
{
	int x, y, width, height;
	struct vrect_t* pnext;
} vrect_t;

typedef struct Rect_t
{
	int x, y;
	int width, height;
} Rect_t;

typedef struct wrect_t
{
	int left;
	int right;
	int top;
	int bottom;
} wrect_t;

typedef struct interval_t
{
	float start;
	float range;
} interval_t;

typedef uint8_t uint8;
typedef int8_t int8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef uint32 RTime32;
typedef float float32;
typedef double float64;
typedef unsigned int uint;

#endif
