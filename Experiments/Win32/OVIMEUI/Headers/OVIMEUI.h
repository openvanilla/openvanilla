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
BOOL IMERegisterClass( HINSTANCE );
BOOL IMEUnRegisterClass( HINSTANCE );
BOOL MyIsIMEMessage(UINT);
void UIPushInputMethod(LPTSTR);
int UICurrentInputMethod();
// UIStatus.cpp
void CreateStatusWindow(HWND);
void MoveStatusWindow(HWND, int, int);
void ShowStatusWindow();
void HideStatusWindow();
void UISetCursorPos(int);
void UISetMarkFrom(int);
void UISetMarkTo(int);
// UIComp.cpp
void CreateCompWindow(HWND);
void MoveCompWindow(HWND, int, int, LPTSTR);
void ShowCompWindow();
void HideCompWindow();
// UICand.cpp
void CreateCandWindow(HWND);
void MoveCandWindow(HWND, LPTSTR);
void ShowCandWindow();
void HideCandWindow();
}
