#if !defined(MiscISurface_Included)
#define MiscISurface_Included

#include "Misc/IAppSystem.h"
#include "Misc/IHTML.h"
#include "Misc/ImageFormat.h"
#include "Misc/IVGuiMatInfo.h"
#include "Types.h"

typedef struct Vertex_t
{
	Vector2D m_Position;
	Vector2D m_TexCoord;
} Vertex_t;

typedef enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
} FontDrawType_t;

typedef struct CharRenderInfo
{
	int x, y;
	Vertex_t* verts;
	int textureId;
	int abcA;
	int abcB;
	int abcC;
	int fontTall;
	HFont currentFont;
	FontDrawType_t drawType;
	wchar_t ch;
	bool valid;
	bool shouldclip;
} CharRenderInfo;

typedef struct IntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
} IntRect;

typedef enum ETextureFormat
{
	eTextureFormat_RGBA,
	eTextureFormat_BGRA,
	eTextureFormat_BGRA_Opaque
} ETextureFormat;

typedef enum SurfaceFeature_e
{
	ANTIALIASED_FONTS = 1,
	DROPSHADOW_FONTS = 2,
	ESCAPE_KEY = 3,
	OPENING_NEW_HTML_WINDOWS = 4,
	FRAME_MINIMIZE_MAXIMIZE = 5,
	OUTLINE_FONTS = 6,
	DIRECT_HWND_RENDER = 7,
} SurfaceFeature_e;

typedef enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800
} EFontFlags;

typedef struct ISurface ISurface;

typedef struct ISurface_VMT
{
	IAppSystem_VMT vmt_IAppSystem;

	// void (*Shutdown)(ISurface*);
	void (*RunFrame)(ISurface*);
	VPANEL (*GetEmbeddedPanel)(ISurface*);
	void (*SetEmbeddedPanel)(ISurface*, VPANEL pPanel);
	void (*PushMakeCurrent)(ISurface*, VPANEL panel, bool useInsets);
	void (*PopMakeCurrent)(ISurface*, VPANEL panel);
	void (*DrawSetColor)(ISurface*, int r, int g, int b, int a);
	void (*DrawSetColor)(ISurface*, Color_t col);
	void (*DrawFilledRect)(ISurface*, int x0, int y0, int x1, int y1);
	void (*DrawFilledRectArray)(ISurface*, IntRect* pRects, int numRects);
	void (*DrawOutlinedRect)(ISurface*, int x0, int y0, int x1, int y1);
	void (*DrawLine)(ISurface*, int x0, int y0, int x1, int y1);
	void (*DrawPolyLine)(ISurface*, int* px, int* py, int numPoints);
	void (*DrawSetTextFont)(ISurface*, HFont font);
	void (*DrawSetTextColor)(ISurface*, int r, int g, int b, int a);
	void (*DrawSetTextColor)(ISurface*, Color_t col);
	void (*DrawSetTextPos)(ISurface*, int x, int y);
	void (*DrawGetTextPos)(ISurface*, int* x, int* y);
	void (*DrawPrintText)(ISurface*, const wchar_t* text, int textLen, FontDrawType_t drawType);
	void (*DrawUnicodeChar)(ISurface*, wchar_t wch, FontDrawType_t drawType);
	void (*DrawFlushText)(ISurface*);
	IHTML* (*CreateHTMLWindow)(ISurface*, IHTMLEvents* events, VPANEL context);
	void (*PaintHTMLWindow)(ISurface*, IHTML* htmlwin);
	void (*DeleteHTMLWindow)(ISurface*, IHTML* htmlwin);
	int (*DrawGetTextureId)(ISurface*, char const* filename);
	bool (*DrawGetTextureFile)(ISurface*, int id, char* filename, int maxlen);
	void (*DrawSetTextureFile)(ISurface*, int id, const char* filename, int hardwareFilter, bool forceReload);
	void (*DrawSetTextureRGBA)(ISurface*, int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, bool forceReload);
	void (*DrawSetTexture)(ISurface*, int id);
	void (*DrawGetTextureSize)(ISurface*, int id, int* wide, int* tall);
	void (*DrawTexturedRect)(ISurface*, int x0, int y0, int x1, int y1);
	bool (*IsTextureIDValid)(ISurface*, int id);
	bool (*DeleteTextureByID)(ISurface*, int id);
	int (*CreateNewTextureID)(ISurface*, bool procedural);
	void (*GetScreenSize)(ISurface*, int* wide, int* tall);
	void (*SetAsTopMost)(ISurface*, VPANEL panel, bool state);
	void (*BringToFront)(ISurface*, VPANEL panel);
	void (*SetForegroundWindow)(ISurface*, VPANEL panel);
	void (*SetPanelVisible)(ISurface*, VPANEL panel, bool state);
	void (*SetMinimized)(ISurface*, VPANEL panel, bool state);
	bool (*IsMinimized)(ISurface*, VPANEL panel);
	void (*FlashWindow)(ISurface*, VPANEL panel, bool state);
	void (*SetTitle)(ISurface*, VPANEL panel, const wchar_t* title);
	void (*SetAsToolBar)(ISurface*, VPANEL panel, bool state);
	void (*CreatePopup)(ISurface*, VPANEL panel, bool minimised, bool showTaskbarIcon, bool disabled, bool mouseInput, bool kbInput);
	void (*SwapBuffers)(ISurface*, VPANEL panel);
	void (*Invalidate)(ISurface*, VPANEL panel);
	void (*SetCursor)(ISurface*, HCursor cursor);
	void (*SetCursorAlwaysVisible)(ISurface*, bool visible);
	bool (*IsCursorVisible)(ISurface*);
	void (*ApplyChanges)(ISurface*);
	bool (*IsWithin)(ISurface*, int x, int y);
	bool (*HasFocus)(ISurface*);
	bool (*SupportsFeature)(ISurface*, SurfaceFeature_e feature);
	void (*RestrictPaintToSinglePanel)(ISurface*, VPANEL panel);
	void (*SetModalPanel)(ISurface*, VPANEL);
	VPANEL (*GetModalPanel)(ISurface*);
	void (*UnlockCursor)(ISurface*);
	void (*LockCursor)(ISurface*);
	void (*SetTranslateExtendedKeys)(ISurface*, bool state);
	VPANEL (*GetTopmostPopup)(ISurface*);
	void (*SetTopLevelFocus)(ISurface*, VPANEL panel);
	HFont (*CreateFont)(ISurface*);
	bool (*SetFontGlyphSet)(ISurface*, HFont font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin, int nRangeMax);
	bool (*AddCustomFontFile)(ISurface*, const char* fontName, const char* fontFileName);
	int (*GetFontTall)(ISurface*, HFont font);
	int (*GetFontTallRequested)(ISurface*, HFont font);
	int (*GetFontAscent)(ISurface*, HFont font, wchar_t wch);
	bool (*IsFontAdditive)(ISurface*, HFont font);
	void (*GetCharABCwide)(ISurface*, HFont font, int ch, int* a, int* b, int* c);
	int (*GetCharacterWidth)(ISurface*, HFont font, int ch);
	void (*GetTextSize)(ISurface*, HFont font, const wchar_t* text, int* wide, int* tall);
	VPANEL (*GetNotifyPanel)(ISurface*);
	void (*SetNotifyIcon)(ISurface*, VPANEL context, HTexture icon, VPANEL panelToReceiveMessages, const char* text);
	void (*PlaySound)(ISurface*, const char* fileName);
	int (*GetPopupCount)(ISurface*);
	VPANEL (*GetPopup)(ISurface*, int index);
	bool (*ShouldPaintChildPanel)(ISurface*, VPANEL childPanel);
	bool (*RecreateContext)(ISurface*, VPANEL panel);
	void (*AddPanel)(ISurface*, VPANEL panel);
	void (*ReleasePanel)(ISurface*, VPANEL panel);
	void (*MovePopupToFront)(ISurface*, VPANEL panel);
	void (*MovePopupToBack)(ISurface*, VPANEL panel);
	void (*SolveTraverse)(ISurface*, VPANEL panel, bool forceApplySchemeSettings);
	void (*PaintTraverse)(ISurface*, VPANEL panel);
	void (*EnableMouseCapture)(ISurface*, VPANEL panel, bool state);
	void (*GetWorkspaceBounds)(ISurface*, int* x, int* y, int* wide, int* tall);
	void (*GetAbsoluteWindowBounds)(ISurface*, int* x, int* y, int* wide, int* tall);
	void (*GetProportionalBase)(ISurface*, int* width, int* height);
	void (*CalculateMouseVisible)(ISurface*);
	bool (*NeedKBInput)(ISurface*);
	bool (*HasCursorPosFunctions)(ISurface*);
	void (*SurfaceGetCursorPos)(ISurface*, int* x, int* y);
	void (*SurfaceSetCursorPos)(ISurface*, int x, int y);
	void (*DrawTexturedLine)(ISurface*, const Vertex_t* a, const Vertex_t* b);
	void (*DrawOutlinedCircle)(ISurface*, int x, int y, int radius, int segments);
	void (*DrawTexturedPolyLine)(ISurface*, const Vertex_t* p, int n);
	void (*DrawTexturedSubRect)(ISurface*, int x0, int y0, int x1, int y1, float texs0, float text0, float texs1, float text1);
	void (*DrawTexturedPolygon)(ISurface*, int n, Vertex_t* pVertice, bool bClipVertices);
	const wchar_t* (*GetTitle)(ISurface*, VPANEL panel);
	bool (*IsCursorLocked)(ISurface*); // const
	void (*SetWorkspaceInsets)(ISurface*, int left, int top, int right, int bottom);
	bool (*DrawGetUnicodeCharRenderInfo)(ISurface*, wchar_t ch, CharRenderInfo* info);
	void (*DrawRenderCharFromInfo)(ISurface*, const CharRenderInfo* info);
	void (*DrawSetAlphaMultiplier)(ISurface*, float alpha);
	float (*DrawGetAlphaMultiplier)(ISurface*);
	void (*SetAllowHTMLJavaScript)(ISurface*, bool state);
	void (*OnScreenSizeChanged)(ISurface*, int nOldWidth, int nOldHeight);
	HCursor (*CreateCursorFromFile)(ISurface*, char const* curOrAniFile, char const* pPathID);
	IVguiMatInfo* (*DrawGetTextureMatInfoFactory)(ISurface*, int id);
	void (*PaintTraverseEx)(ISurface*, VPANEL panel, bool paintPopups);
	float (*GetZPos)(ISurface*); // const
	void (*SetPanelForInput)(ISurface*, VPANEL vpanel);
	void (*DrawFilledRectFastFade)(ISurface*, int x0, int y0, int x1, int y1, int fadeStartPt, int fadeEndPt, unsigned int alpha0, unsigned int alpha1, bool bHorizontal);
	void (*DrawFilledRectFade)(ISurface*, int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool bHorizontal);
	void (*DrawSetTextureRGBAEx)(ISurface*, int id, const unsigned char* rgba, int wide, int tall, ImageFormat imageFormat);
	void (*DrawSetTextScale)(ISurface*, float sx, float sy);
	bool (*SetBitmapFontGlyphSet)(ISurface*, HFont font, const char* windowsFontName, float scalex, float scaley, int flags);
	bool (*AddBitmapFontFile)(ISurface*, const char* fontFileName);
	void (*SetBitmapFontName)(ISurface*, const char* pName, const char* pFontFilename);
	const char* (*GetBitmapFontName)(ISurface*, const char* pName);
	void (*ClearTemporaryFontCache)(ISurface*);
	IImage* (*GetIconImageForFullPath)(ISurface*, char const* pFullPath);
	void (*DrawUnicodeString)(ISurface*, const wchar_t* pwString, FontDrawType_t drawType);
	void (*PrecacheFontCharacters)(ISurface*, HFont font, const wchar_t* pCharacters);
	const char* (*GetResolutionKey)(ISurface*); // const
	const char* (*GetFontName)(ISurface*, HFont font);
	const char* (*GetFontFamilyName)(ISurface*, HFont font);
	void (*GetKernedCharWidth)(ISurface*, HFont font, wchar_t ch, wchar_t chBefore, wchar_t chAfter, float* wide, float* abcA);
	bool (*ForceScreenSizeOverride)(ISurface*, bool bState, int wide, int tall);
	bool (*ForceScreenPosOffset)(ISurface*, bool bState, int x, int y);
	void (*OffsetAbsPos)(ISurface*, int* x, int* y);
	void (*ResetFontCaches)(ISurface*);
	int (*GetTextureNumFrames)(ISurface*, int id);
	void (*DrawSetTextureFrame)(ISurface*, int id, int nFrame, unsigned int* pFrameCache);
	bool (*IsScreenSizeOverrideActive)(ISurface*);
	bool (*IsScreenPosOverrideActive)(ISurface*);
	void (*DestroyTextureID)(ISurface*, int id);
	void (*DrawUpdateRegionTextureRGBA)(ISurface*, int nTextureID, int x, int y, const unsigned char* pchData, int wide, int tall, ImageFormat imageFormat);
	bool (*BHTMLWindowNeedsPaint)(ISurface*, IHTML* htmlwin);
	const char* (*GetWebkitHTMLUserAgentString)(ISurface*);
	void* (*Deprecated_AccessChromeHTMLController)(ISurface*);
	void (*SetFullscreenViewport)(ISurface*, int x, int y, int w, int h);
	void (*GetFullscreenViewport)(ISurface*, int* x, int* y, int* w, int* h);
	void (*PushFullscreenViewport)(ISurface*);
	void (*PopFullscreenViewport)(ISurface*);
	void (*SetSoftwareCursor)(ISurface*, bool bUseSoftwareCursor);
	void (*PaintSoftwareCursor)(ISurface*);
} ISurface_VMT;

struct ISurface
{
	ISurface_VMT* vmt;
};

#endif
