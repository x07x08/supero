#if !defined(MiscModes_Included)
#define MiscModes_Included

typedef struct vmode_s
{
	int width;
	int height;
	int bpp;
	int refreshRate;
} vmode_s;

typedef vmode_s vmode_t;

#endif
