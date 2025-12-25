#if !defined(MiscImageFormat_Included)
#define MiscImageFormat_Included

typedef enum NormalDecodeMode_t
{
	NORMAL_DECODE_NONE = 0
} NormalDecodeMode_t;

// typedef enum _D3DFORMAT D3DFORMAT;

typedef enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,
	IMAGE_FORMAT_RGB323232F,
	IMAGE_FORMAT_RGBA32323232F,
	IMAGE_FORMAT_NV_DST16,
	IMAGE_FORMAT_NV_DST24,
	IMAGE_FORMAT_NV_INTZ,
	IMAGE_FORMAT_NV_RAWZ,
	IMAGE_FORMAT_ATI_DST16,
	IMAGE_FORMAT_ATI_DST24,
	IMAGE_FORMAT_NV_NULL,
	IMAGE_FORMAT_ATI2N,
	IMAGE_FORMAT_ATI1N,
	IMAGE_FORMAT_DXT1_RUNTIME,
	IMAGE_FORMAT_DXT5_RUNTIME,
	NUM_IMAGE_FORMATS
} ImageFormat;

typedef struct BGRA8888_t
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
} BGRA8888_t;

typedef struct BGRX8888_t
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char x;
} BGRX8888_t;

typedef struct RGBA8888_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} RGBA8888_t;

typedef struct RGB888_t
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RGB888_t;

typedef struct BGR888_t
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
} BGR888_t;

typedef struct BGR565_t
{
	unsigned short b : 5;
	unsigned short g : 6;
	unsigned short r : 5;
} BGR565_t;

typedef struct BGRA5551_t
{
	unsigned short b : 5;
	unsigned short g : 5;
	unsigned short r : 5;
	unsigned short a : 1;
} BGRA5551_t;

typedef struct BGRA4444_t
{
	unsigned short b : 4;
	unsigned short g : 4;
	unsigned short r : 4;
	unsigned short a : 4;
} BGRA4444_t;

typedef struct RGBX5551_t
{
	unsigned short r : 5;
	unsigned short g : 5;
	unsigned short b : 5;
	unsigned short x : 1;
} RGBX5551_t;

#endif
