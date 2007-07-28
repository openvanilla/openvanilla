#pragma unmanaged
#include <windows.h>
#include <tchar.h>

#include "OVUtility.h"

typedef struct _tagUICHILD{
	HWND    hWnd;
	POINT   pt;
	SIZE    sz;
} UICHILD, *LPUICHILD;

extern "C" {
// Interfaces
// OVIMEUI.cpp
BOOL IMEUIRegisterClass( HINSTANCE );
BOOL IMEUIUnRegisterClass( HINSTANCE );
BOOL MyIsIMEMessage(UINT);
//<comment author='b6s'>
// Uses the managed UI function instead
//void UIPushInputMethod(wchar_t*);
//</comment>
int UICurrentInputMethod();

void UIConstruct();
void UIDispose();

// UIStatus.cpp
void UICreateStatusWindow(HWND);
void UIMoveStatusWindow(HWND, int, int);
void UIShowStatusWindow();
void UIHideStatusWindow();
int UIModuleChange();
int UIModuleRotate();
void UIChangeHalfFull(HWND);
void UIChangeChiEng(HWND);
void UIChangeSimpifiedOrTraditional(HWND);
void UIChangeBoPoMoFoLayout(int index);
void UIClearStatusMenuModString();
void UISetStatusModStrMenuEach(const char* newName);
void UISetStatusModStrMenuAll(int modAmount, const char* modNames[]);
void UISetStatusModStrCurrent(int index);

// UIComp.cpp
void UICreateCompWindow(HWND);
void UIMoveCompWindow(int, int, int, int);
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

// UINotify.cpp
void UICreateNotifyWindow(HWND);
void UIShowNotifyWindow();
void UIHideNotifyWindow();
void UISetNotifyStr(wchar_t*);
}
