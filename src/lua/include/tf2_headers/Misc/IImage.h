#if !defined(MiscIImage_Included)
#define MiscIImage_Included

#include "Misc/VGUI.h"
#include "Types.h"
#include "Misc/CommonMacros.h"

typedef unsigned long HTexture;

typedef enum iimage_rotation_t
{
	ROTATED_UNROTATED = 0,
	ROTATED_CLOCKWISE_90,
	ROTATED_ANTICLOCKWISE_90,
	ROTATED_FLIPPED
} iimage_rotation_t;

typedef struct IImage IImage;

typedef struct IImage_VMT
{
	void (*Paint)(IImage*);
	void (*SetPos)(IImage*, int x, int y);
	void (*GetContentSize)(IImage*, int* wide, int* tall);
	void (*GetSize)(IImage*, int* wide, int* tall);
	void (*SetSize)(IImage*, int wide, int tall);
	void (*SetColor)(IImage*, Color_t col);
	//virtual ~IImage() {}
	void (*DTOR)(void*);

	VMT_DTOR_PADDING(0)

	bool (*Evict)(IImage*);
	int (*GetNumFrames)(IImage*);
	void (*SetFrame)(IImage*, int nFrame);
	HTexture (*GetID)(IImage*);
	void (*SetRotation)(IImage*, int iRotation);
} IImage_VMT;

struct IImage
{
	IImage_VMT* vmt;
};

#endif
