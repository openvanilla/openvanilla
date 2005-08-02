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
// UIComp.cpp
void CreateCompWindow(HWND);
void MoveCompWindow(HWND, int, int, LPTSTR);
void HideCompWindow();
// UICand.cpp
void CreateCandWindow(HWND);
void MoveCandWindow(HWND, LPTSTR);
void HideCandWindow();
}
