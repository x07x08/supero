#if !defined(IntfIVDebugOverlay_Included)
#define IntfIVDebugOverlay_Included

#include "Types.h"

typedef struct OverlayText_t OverlayText_t;

#if !defined(PRINTF_FORMAT_STRING)
#define PRINTF_FORMAT_STRING
#endif

typedef struct IVDebugOverlay IVDebugOverlay;

typedef struct IVDebugOverlay_VMT
{
	void (*AddEntityTextOverlay)(IVDebugOverlay*, int ent_index, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...);
	void (*AddBoxOverlay)(IVDebugOverlay*, const Vector* origin, const Vector* mins, const Vector* max, QAngle const* orientation, int r, int g, int b, int a, float duration);
	void (*AddTriangleOverlay)(IVDebugOverlay*, const Vector* p1, const Vector* p2, const Vector* p3, int r, int g, int b, int a, bool noDepthTest, float duration);
	void (*AddLineOverlay)(IVDebugOverlay*, const Vector* origin, const Vector* dest, int r, int g, int b, bool noDepthTest, float duration);
	void (*AddTextOverlay)(IVDebugOverlay*, const Vector* origin, float duration, PRINTF_FORMAT_STRING const char* format, ...);
	void (*AddTextOverlayOffset)(IVDebugOverlay*, const Vector* origin, int line_offset, float duration, PRINTF_FORMAT_STRING const char* format, ...);
	void (*AddScreenTextOverlay)(IVDebugOverlay*, float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char* text);
	void (*AddSweptBoxOverlay)(IVDebugOverlay*, const Vector* start, const Vector* end, const Vector* mins, const Vector* max, const QAngle* angles, int r, int g, int b, int a, float flDuration);
	void (*AddGridOverlay)(IVDebugOverlay*, const Vector* origin);
	int (*ScreenPosition)(IVDebugOverlay*, const Vector* point, Vector* screen);
	int (*ScreenPositionXY)(IVDebugOverlay*, float flXPos, float flYPos, Vector* screen);
	OverlayText_t* (*GetFirst)(IVDebugOverlay*);
	OverlayText_t* (*GetNext)(IVDebugOverlay*, OverlayText_t * current);
	void (*ClearDeadOverlays)(IVDebugOverlay*);
	void (*ClearAllOverlays)(IVDebugOverlay*);
	void (*AddTextOverlayRGB)(IVDebugOverlay*, const Vector* origin, int line_offset, float duration, float r, float g, float b, float alpha, PRINTF_FORMAT_STRING const char* format, ...);
	void (*AddTextOverlayRGBInt)(IVDebugOverlay*, const Vector* origin, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...);
	void (*AddLineOverlayAlpha)(IVDebugOverlay*, const Vector* origin, const Vector* dest, int r, int g, int b, int a, bool noDepthTest, float duration);
	//void (*AddBoxOverlay2)(IVDebugOverlay*, const Vector* origin, const Vector* mins, const Vector* max, QAngle const* orientation, const Color* faceColor, const Color* edgeColor, float duration);
	void (*AddBoxOverlay2)(IVDebugOverlay*, const Vector* origin, const Vector* mins, const Vector* max, QAngle const* orientation, const Color_t* faceColor, const Color_t* edgeColor, float duration);
	void (*AddScreenTextOverlay2)(IVDebugOverlay*, float flXPos, float flYPos, int iLine, float flDuration, int r, int g, int b, int a, const char* text);
} IVDebugOverlay_VMT;

struct IVDebugOverlay
{
	IVDebugOverlay_VMT* vmt;
};

#define IVDebugOverlay_REFL "IVDebugOverlay"
#define IVDebugOverlay_SIGNATURE "VDebugOverlay003"
#define IVDebugOverlay_MODULE "engine"

#endif
