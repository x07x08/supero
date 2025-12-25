#if !defined(IntfIPanel_Included)
#define IntfIPanel_Included

#include "Misc/VGUI.h"
#include "Main/UtlVector.h"
#include "Main/KeyValues.h"

typedef struct IClientPanel IClientPanel;
typedef struct SurfacePlat SurfacePlat;
typedef struct Panel Panel;

typedef struct IPanel IPanel;

typedef struct IPanel_VMT
{
	void (*Init)(IPanel*, VPANEL vguiPanel, IClientPanel* panel);
	void (*SetPos)(IPanel*, VPANEL vguiPanel, int x, int y);
	void (*GetPos)(IPanel*, VPANEL vguiPanel, int* x, int* y);
	void (*SetSize)(IPanel*, VPANEL vguiPanel, int wide, int tall);
	void (*GetSize)(IPanel*, VPANEL vguiPanel, int* wide, int* tall);
	void (*SetMinimumSize)(IPanel*, VPANEL vguiPanel, int wide, int tall);
	void (*GetMinimumSize)(IPanel*, VPANEL vguiPanel, int* wide, int* tall);
	void (*SetZPos)(IPanel*, VPANEL vguiPanel, int z);
	int (*GetZPos)(IPanel*, VPANEL vguiPanel);
	void (*GetAbsPos)(IPanel*, VPANEL vguiPanel, int* x, int* y);
	void (*GetClipRect)(IPanel*, VPANEL vguiPanel, int* x0, int* y0, int* x1, int* y1);
	void (*SetInset)(IPanel*, VPANEL vguiPanel, int left, int top, int right, int bottom);
	void (*GetInset)(IPanel*, VPANEL vguiPanel, int* left, int* top, int* right, int* bottom);
	void (*SetVisible)(IPanel*, VPANEL vguiPanel, bool state);
	bool (*IsVisible)(IPanel*, VPANEL vguiPanel);
	void (*SetParent)(IPanel*, VPANEL vguiPanel, VPANEL newParent);
	int (*GetChildCount)(IPanel*, VPANEL vguiPanel);
	VPANEL (*GetChild)(IPanel*, VPANEL vguiPanel, int index);
	CUtlVector* (*GetChildren)(IPanel*, VPANEL vguiPanel);
	VPANEL (*GetParent)(IPanel*, VPANEL vguiPanel);
	void (*MoveToFront)(IPanel*, VPANEL vguiPanel);
	void (*MoveToBack)(IPanel*, VPANEL vguiPanel);
	bool (*HasParent)(IPanel*, VPANEL vguiPanel, VPANEL potentialParent);
	bool (*IsPopup)(IPanel*, VPANEL vguiPanel);
	void (*SetPopup)(IPanel*, VPANEL vguiPanel, bool state);
	bool (*IsFullyVisible)(IPanel*, VPANEL vguiPanel);
	HScheme (*GetScheme)(IPanel*, VPANEL vguiPanel);
	bool (*IsProportional)(IPanel*, VPANEL vguiPanel);
	bool (*IsAutoDeleteSet)(IPanel*, VPANEL vguiPanel);
	void (*DeletePanel)(IPanel*, VPANEL vguiPanel);
	void (*SetKeyBoardInputEnabled)(IPanel*, VPANEL vguiPanel, bool state);
	void (*SetMouseInputEnabled)(IPanel*, VPANEL vguiPanel, bool state);
	bool (*IsKeyBoardInputEnabled)(IPanel*, VPANEL vguiPanel);
	bool (*IsMouseInputEnabled)(IPanel*, VPANEL vguiPanel);
	void (*Solve)(IPanel*, VPANEL vguiPanel);
	const char* (*GetName)(IPanel*, VPANEL vguiPanel);
	const char* (*GetClassName)(IPanel*, VPANEL vguiPanel);
	void (*SendMessage)(IPanel*, VPANEL vguiPanel, KeyValues* params, VPANEL ifromPanel);
	void (*Think)(IPanel*, VPANEL vguiPanel);
	void (*PerformApplySchemeSettings)(IPanel*, VPANEL vguiPanel);
	void (*PaintTraverse)(IPanel*, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
	void (*Repaint)(IPanel*, VPANEL vguiPanel);
	VPANEL (*IsWithinTraverse)(IPanel*, VPANEL vguiPanel, int x, int y, bool traversePopups);
	void (*OnChildAdded)(IPanel*, VPANEL vguiPanel, VPANEL child);
	void (*OnSizeChanged)(IPanel*, VPANEL vguiPanel, int newWide, int newTall);
	void (*InternalFocusChanged)(IPanel*, VPANEL vguiPanel, bool lost);
	bool (*RequestInfo)(IPanel*, VPANEL vguiPanel, KeyValues* outputData);
	void (*RequestFocus)(IPanel*, VPANEL vguiPanel, int direction);
	bool (*RequestFocusPrev)(IPanel*, VPANEL vguiPanel, VPANEL existingPanel);
	bool (*RequestFocusNext)(IPanel*, VPANEL vguiPanel, VPANEL existingPanel);
	VPANEL (*GetCurrentKeyFocus)(IPanel*, VPANEL vguiPanel);
	int (*GetTabPosition)(IPanel*, VPANEL vguiPanel);
	SurfacePlat* (*Plat)(IPanel*, VPANEL vguiPanel);
	void (*SetPlat)(IPanel*, VPANEL vguiPanel, SurfacePlat* Plat);
	Panel* (*GetPanel)(IPanel*, VPANEL vguiPanel, const char* destinationModule);
	bool (*IsEnabled)(IPanel*, VPANEL vguiPanel);
	void (*SetEnabled)(IPanel*, VPANEL vguiPanel, bool state);
	bool (*IsTopmostPopup)(IPanel*, VPANEL vguiPanel);
	void (*SetTopmostPopup)(IPanel*, VPANEL vguiPanel, bool state);
	void (*SetSiblingPin)(IPanel*, VPANEL vguiPanel, VPANEL newSibling, byte iMyCornerToPin, byte iSiblingCornerToPinTo);
} IPanel_VMT;

struct IPanel// : public IBaseInterface
{
	IPanel_VMT* vmt;
};

#define IPanel_REFL "IPanel"
#define IPanel_SIGNATURE "VGUI_Panel009"
#define IPanel_MODULE "vgui2"

#endif
