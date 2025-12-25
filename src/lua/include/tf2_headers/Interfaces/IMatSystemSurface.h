#if !defined(IntfIMatSystemSurface_Included)
#define IntfIMatSystemSurface_Included

#include "Interfaces/Interface.h"
#include "Misc/ISurface.h"

// MAKE_SIGNATURE(CMatSystemSurface_StartDrawing, "vguimatsurface.dll", "40 53 56 57 48 83 EC ? 48 8B F9 80 3D", 0x0);
// MAKE_SIGNATURE(CMatSystemSurface_FinishDrawing, "vguimatsurface.dll", "40 53 48 83 EC ? 33 C9", 0x0);

// class VMatrix;
typedef struct IMaterial IMaterial;
typedef struct ITexture ITexture;
// typedef struct InputEvent_t InputEvent_t;

typedef void (*GetMouseCallback_t)(int* x, int* y);
typedef void (*SetMouseCallback_t)(int x, int y);
typedef void (*PlaySoundFunc_t)(const char* pFileName);

typedef struct IMatSystemSurface IMatSystemSurface;

typedef struct IMatSystemSurface_VMT
{
	ISurface_VMT vmt_ISurface;

	void (*AttachToWindow)(IMatSystemSurface*, void* hwnd, bool bLetAppDriveInput);
	void (*EnableWindowsMessages)(IMatSystemSurface*, bool bEnable);
	void (*Begin3DPaint)(IMatSystemSurface*, int iLeft, int iTop, int iRight, int iBottom, bool bRenderToTexture);
	void (*End3DPaint)(IMatSystemSurface*);
	void (*DisableClipping)(IMatSystemSurface*, bool bDisable);
	void (*GetClippingRect)(IMatSystemSurface*, int* left, int* top, int* right, int* bottom, bool* bClippingDisabled);
	void (*SetClippingRect)(IMatSystemSurface*, int left, int top, int right, int bottom);
	// bool (*IsCursorLocked)(IMatSystemSurface*); // const
	void (*SetMouseCallbacks)(IMatSystemSurface*, GetMouseCallback_t getFunc, SetMouseCallback_t setFunc);
	void (*InstallPlaySoundFunc)(IMatSystemSurface*, PlaySoundFunc_t soundFunc);
	void (*DrawColoredCircle)(IMatSystemSurface*, int centerx, int centery, float radius, int r, int g, int b, int a);
	int (*DrawColoredText)(IMatSystemSurface*, HFont font, int x, int y, int r, int g, int b, int a, const char* fmt, ...);
	void (*DrawColoredTextRect)(IMatSystemSurface*, HFont font, int x, int y, int w, int h, int r, int g, int b, int a, const char* fmt, ...);
	void (*DrawTextHeight)(IMatSystemSurface*, HFont font, int w, int* h, const char* fmt, ...);
	int (*DrawTextLen)(IMatSystemSurface*, HFont font, const char* fmt, ...);
	void (*DrawPanelIn3DSpace)(IMatSystemSurface*, VPANEL pRootPanel, const VMatrix* panelCenterToWorld, int nPixelWidth, int nPixelHeight, float flWorldWidth, float flWorldHeight);
	void (*DrawSetTextureMaterial)(IMatSystemSurface*, int id, IMaterial* pMaterial);
	bool (*HandleInputEvent)(IMatSystemSurface*, const InputEvent_t* event);
	void (*Set3DPaintTempRenderTarget)(IMatSystemSurface*, const char* pRenderTargetName);
	void (*Reset3DPaintTempRenderTarget)(IMatSystemSurface*);
	IMaterial* (*DrawGetTextureMaterial)(IMatSystemSurface*, int id);
	void (*GetFullscreenViewportAndRenderTarget)(IMatSystemSurface*, int* x, int* y, int* w, int* h, ITexture** ppRenderTarget);
	void (*SetFullscreenViewportAndRenderTarget)(IMatSystemSurface*, int x, int y, int w, int h, ITexture* pRenderTarget);
	int (*DrawGetTextureId)(IMatSystemSurface*, ITexture* pTexture);
	void (*BeginSkinCompositionPainting)(IMatSystemSurface*);
	void (*EndSkinCompositionPainting)(IMatSystemSurface*);

	// SIGNATURE(StartDrawing, void, CMatSystemSurface, this);
	// SIGNATURE(FinishDrawing, void, CMatSystemSurface, this);
} IMatSystemSurface_VMT;

struct IMatSystemSurface
{
	IMatSystemSurface_VMT* vmt;
};

#define IMatSystemSurface_REFL "IMatSystemSurface"
#define IMatSystemSurface_SIGNATURE "VGUI_Surface030"
#define IMatSystemSurface_MODULE "vguimatsurface"

#endif
