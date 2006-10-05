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
void UIPushInputMethod(wchar_t*);
int UICurrentInputMethod();

// UIStatus.cpp
void UICreateStatusWindow(HWND);
void UIMoveStatusWindow(HWND, int, int);
void UIShowStatusWindow();
void UIHideStatusWindow();
void UISetCursorPos(int);
void UISetMarkFrom(int);
void UISetMarkTo(int);
void UIChangeModule(HWND);
void UIChangeHalfFull(HWND);
void UIChangeChiEng(HWND);
// UIComp.cpp
void UICreateCompWindow(HWND);
void UIMoveCompWindow(HWND, int, int, wchar_t*);
void UIShowCompWindow();
void UIHideCompWindow();
void UISetCompStr(wchar_t*);
// UICand.cpp
void UICreateCandWindow(HWND);
void UIMoveCandWindow(HWND, int, int, wchar_t*);
void UIShowCandWindow();
void UIHideCandWindow();
void UISetCandStr(wchar_t*);
// UINotify.cpp
void UICreateNotifyWindow(HWND);
void UIShowNotifyWindow(wchar_t*);
void UIHideNotifyWindow();
}
