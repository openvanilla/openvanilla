#include <windows.h>
#include <tchar.h>

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
void UIClearInputMethodList();
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
// UIComp.cpp
void UICreateCompWindow(HWND);
void UIMoveCompWindow(HWND, int, int, wchar_t*);
void UIShowCompWindow();
void UIHideCompWindow();
// UICand.cpp
void UICreateCandWindow(HWND);
void UIMoveCandWindow(HWND, int, int, wchar_t*);
void UIShowCandWindow();
void UIHideCandWindow();
// UINotify.cpp
void UICreateNotifyWindow(HWND);
void UIShowNotifyWindow(wchar_t*);
void UIHideNotifyWindow();
}
