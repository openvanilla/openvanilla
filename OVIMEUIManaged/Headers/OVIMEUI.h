//#pragma unmanaged
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "OVUtility.h"

typedef struct _tagUICHILD{
	HWND    hWnd;
	POINT   pt;
	SIZE    sz;
} UICHILD, *LPUICHILD;

extern "C" {
// Interfaces
// OVIMEUI.cpp
//BOOL IMEUIRegisterClass( HINSTANCE );
//BOOL IMEUIUnRegisterClass( HINSTANCE );
//BOOL MyIsIMEMessage(UINT);
//<comment author='b6s'>
// Uses the managed UI function instead
//void UIPushInputMethod(wchar_t*);
//</comment>
//int UICurrentInputMethod();  move to uistatus.cpp

//void UIConstruct();
//void UIDispose();

bool UIShowDeskBand(bool t);

// UIStatus.cpp
void UICreateStatusWindow(HWND, int, int, bool);
void UIMoveStatusWindow(HWND, int, int);
void UIShowStatusWindow();
void UIHideStatusWindow();
int UIGetStatusPos(LPPOINT lp_pt);
int UIModuleChange();
int UIModuleRotate();
void UIChangeHalfFull();
void UIChangeChiEng();
void UISetModuleChecked(int, bool);
bool UIGetModuleChecked(int);
void UIChangeSimpifiedOrTraditional(HWND);
void UIChangeBoPoMoFoLayout(int index);
void UIClearStatusMenuModString();
void UISetStatusModStrMenuEach(const char* newName);
void UISetStatusModStrMenuAll(int modAmount, const char* modNames[]);
void UISetStatusModStrCurrent(int index);
void UISetkpCount(int count);
void UISetofCount(int count);
void UISetimCount(int count);
int UICurrentInputMethod();
void UIDockStatusWindow(int, int);
bool UIGetIsStatusDocked();
void UISetIsStatusDocked(bool t);

// UIComp.cpp
void UICreateCompWindow(HWND);
void UIMoveCompWindow(int, int, int, const wchar_t*);
void UIShowCompWindow();
void UIHideCompWindow();
int UIGetHeight();
void UISetCursorPos(int);
int UIGetCaretPosX();
void UISetCompStr(wchar_t*);
void UISetCompCaretPosX(int);
void UISetMarkFrom(int);
void UISetMarkTo(int);
void UIClearCompStr();

// UICand.cpp
void UICreateCandWindow(HWND);
void UIMoveCandWindow(int, int, int);
void UIExpandCandi();
void UIShowCandWindow();
void UIHideCandWindow();
void UISetCandStr(wchar_t*);
void UISelectNextCandItem(int i);
int UIGetCandSelectedItemKey();
int UIGetCandHeight();
int UIGetCandWidth();
void UISetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor);
void UISetCandiFont(const wchar_t* ff, int fontsize);
void UISetCandiOpacity(int o);


// UINotify.cpp
void UICreateNotifyWindow(HWND);
void UIShowNotifyWindow();
void UIHideNotifyWindow();
void UIMoveNotifyWindow(int, int);
void UISetNotifyStr(wchar_t*);
int UIGetNotifyHeight();
int UIGetNotifyWidth();
void UISetNotifyColor(int forecolor, int backcolor);
void UISetNotifyFont(const wchar_t* ff, int fontsize);
void UISetNotifyEnabled(bool b);
void UISetNotifyOpacity(int o);

}

