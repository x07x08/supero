#if !defined(MiscIHTML_Included)
#define MiscIHTML_Included

#include "Misc/Cursor.h"
#include "Misc/IImage.h"
#include "Misc/KeyCode.h"
#include "Misc/MouseCode.h"

typedef enum MOUSE_STATE
{
	UP,
	DOWN,
	MMOVE,
	DBLCLICK
} MOUSE_STATE;

typedef enum EWebPageLoadError
{
	eLoadErrorNone = 0,
	eMimeTypeNotSupported,
	eCacheMiss,
	eBadURL,
	eConnectionProblem,
	eProxyConnectionProblem,
	eLoadErrorUnknown
} EWebPageLoadError;

typedef struct IHTML IHTML;

typedef struct IHTML_VMT
{
	void (*OpenURL)(IHTML*, const char*);
	bool (*StopLoading)(IHTML*);
	bool (*Refresh)(IHTML*);
	bool (*Show)(IHTML*, bool shown);
	const char* (*GetOpenedPage)(IHTML*);
	void (*Obsolete_OnSize)(IHTML*, int x, int y, int w, int h);
	void (*GetHTMLSize)(IHTML*, int* wide, int* tall);
	void (*Clear)(IHTML*);
	void (*AddText)(IHTML*, const char* text);
	void (*Obsolete_OnMouse)(IHTML*, MouseCode code, MOUSE_STATE s, int x, int y);
	void (*Obsolete_OnChar)(IHTML*, wchar_t unichar);
	void (*Obsolete_OnKeyDown)(IHTML*, KeyCode code);
	IImage* (*GetBitmap)(IHTML*);
	void (*SetVisible)(IHTML*, bool state);
	void (*SetSize)(IHTML*, int wide, int tall);
	void (*OnMouse)(IHTML*, MouseCode code, MOUSE_STATE s, int x, int y, bool bPopupMenuMenu);
	void (*OnChar)(IHTML*, wchar_t unichar, bool bPopupMenu);
	void (*OnKeyDown)(IHTML*, KeyCode code, bool bPopupMenu);
	void (*ScrollV)(IHTML*, int nPixels);
	void (*ScrollH)(IHTML*, int nPixels);
	void (*OnMouseWheeled)(IHTML*, int delta, bool bPopupMenu);
	void (*OnKeyUp)(IHTML*, KeyCode code, bool bPopupMenu);
	void (*PostURL)(IHTML*, const char* pchURL, const char* pchPostData);
	void (*RunJavascript)(IHTML*, const char* pchScript);
	void (*SetMousePosition)(IHTML*, int x, int y, bool bPopupMenu);
	void (*SetUserAgentInfo)(IHTML*, const wchar_t* pwchUserAgent);
	void (*AddHeader)(IHTML*, const char* pchHeader, const char* pchValue);
	void (*SetFileDialogChoice)(IHTML*, const char* pchFileName);
	void (*HidePopup)(IHTML*);
	void (*SetHTMLFocus)(IHTML*);
	void (*KillHTMLFocus)(IHTML*);
	void (*HorizontalScrollBarSize)(IHTML*, int* x, int* y, int* wide, int* tall);
	void (*VerticalScrollBarSize)(IHTML*, int* x, int* y, int* wide, int* tall);
	int (*HorizontalScroll)(IHTML*);
	int (*VerticalScroll)(IHTML*);
	int (*HorizontalScrollMax)(IHTML*);
	int (*VerticalScrollMax)(IHTML*);
	bool (*IsHorizontalScrollBarVisible)(IHTML*);
	bool (*IsVeritcalScrollBarVisible)(IHTML*);
	void (*SetHorizontalScroll)(IHTML*, int scroll);
	void (*SetVerticalScroll)(IHTML*, int scroll);
	void (*ViewSource)(IHTML*);
	void (*Copy)(IHTML*);
	void (*Paste)(IHTML*);
	bool (*IsIERender)(IHTML*);
	void (*GetIDispatchPtr)(IHTML*, void** pIDispatch);
	void (*GetHTMLScroll)(IHTML*, int* top, int* left);
} IHTML_VMT;

struct IHTML
{
	IHTML_VMT* vmt;
};

typedef struct IHTMLEvents IHTMLEvents;

typedef struct IHTMLEvents_VMT
{
	bool (*Obsolete_OnStartURL)(IHTMLEvents*, const char* url, const char* target, bool first);
	void (*Obsolete_OnFinishURL)(IHTMLEvents*, const char* url);
	void (*Obsolete_OnProgressURL)(IHTMLEvents*, long current, long maximum);
	void (*Obsolete_OnSetStatusText)(IHTMLEvents*, const char* text);
	void (*Obsolete_OnUpdate)(IHTMLEvents*);
	void (*Obsolete_OnLink)(IHTMLEvents*);
	void (*Obsolete_OffLink)(IHTMLEvents*);
	void (*OnURLChanged)(IHTMLEvents*, const char* url, const char* pchPostData, bool bIsRedirect);
	void (*OnFinishRequest)(IHTMLEvents*, const char* url, const char* pageTitle);
	bool (*OnStartRequestInternal)(IHTMLEvents*, const char* url, const char* target, const char* pchPostData, bool bIsRedirect);
	void (*ShowPopup)(IHTMLEvents*, int x, int y, int wide, int tall);
	void (*HidePopup)(IHTMLEvents*);
	bool (*OnPopupHTMLWindow)(IHTMLEvents*, const char* pchURL, int x, int y, int wide, int tall);
	void (*SetHTMLTitle)(IHTMLEvents*, const char* pchTitle);
	void (*OnLoadingResource)(IHTMLEvents*, const char* pchURL);
	void (*OnSetStatusText)(IHTMLEvents*, const char* text);
	void (*OnSetCursor)(IHTMLEvents*, CursorCode cursor);
	void (*OnFileLoadDialog)(IHTMLEvents*, const char* pchTitle, const char* pchInitialFile);
	void (*OnShowToolTip)(IHTMLEvents*, const char* pchText);
	void (*OnUpdateToolTip)(IHTMLEvents*, const char* pchText);
	void (*OnHideToolTip)(IHTMLEvents*);
	bool (*BOnCreateNewWindow)(IHTMLEvents*, void** ppDispatch);
	void (*OnLink)(IHTMLEvents*);
	void (*OffLink)(IHTMLEvents*);
	void (*OnCloseWindow)(IHTMLEvents*);
	void (*OnUpdate)(IHTMLEvents*);
	void (*OnProgressRequest)(IHTMLEvents*, long current, long maximum);
	bool (*OnOpenNewTab)(IHTMLEvents*, const char* pchURL, bool bForeground);
} IHTMLEvents_VMT;

struct IHTMLEvents
{
	IHTMLEvents_VMT* vmt;
};

#endif
