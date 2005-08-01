#include <windows.h>
#include <tchar.h>

typedef struct _tagUICHILD{
	HWND    hWnd;
	POINT   pt;
	SIZE    sz;
} UICHILD, NEAR *PUICHILD, FAR *LPUICHILD;

typedef struct _tagUIEXTRA{
	UICHILD  uiStatus;
	UICHILD  uiCand;
	UICHILD  uiComp;
} UIEXTRA, NEAR *PUIEXTRA, FAR *LPUIEXTRA;

extern "C" {
// Interfaces
// OVIMEUI.cpp
BOOL IMERegisterClass( HINSTANCE );
BOOL IMEUnRegisterClass( HINSTANCE );
void SetLPUIExtra(LPUIEXTRA);
LPTSTR UTF16toWCHAR(char *);
BOOL MyIsIMEMessage(UINT);
// UIComp.cpp
void CreateCompWindow(HWND);
void MoveCompWindow(HWND, LPTSTR);
void HideCompWindow();
// UICand.cpp
void CreateCandWindow(HWND);
void MoveCandWindow(HWND, LPTSTR);
void HideCandWindow();
}

extern LPUIEXTRA lpUIExtra;
