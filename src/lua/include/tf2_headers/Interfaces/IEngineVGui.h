#if !defined(IntfIEngineVGui_Included)
#define IntfIEngineVGui_Included

#if defined(__INTELLISENSE__) || defined(__JETBRAINS_IDE__)
#include <stdbool.h>
#endif

#include "Interfaces/Interface.h"
#include "Misc/VGUI.h"

typedef enum VGuiPanel_t
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS
} VGuiPanel_t;

typedef enum PaintMode_t
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
	PAINT_CURSOR = (1 << 2)
} PaintMode_t;

typedef struct IEngineVGui IEngineVGui;

typedef struct IEngineVGui_VMT
{
	VPANEL (*GetPanel)(IEngineVGui*, VGuiPanel_t type);
	bool (*IsGameUIVisible)(IEngineVGui*);
} IEngineVGui_VMT;

struct IEngineVGui
{
	IEngineVGui_VMT* vmt;
};

#define IEngineVGui_REFL "IEngineVGui"
#define IEngineVGui_SIGNATURE "VEngineVGui002"
#define IEngineVGui_MODULE "engine"

#endif
