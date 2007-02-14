#pragma unmanaged
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include "OVIMEUI.h"

#define CS_OVIME (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)
#define UICLASSNAME _T("OVIMEUI")
#define UISTATUSCLASSNAME _T("OVIMESTATUS")
#define UICOMPCLASSNAME _T("OVIMECOMP")
#define UICANDCLASSNAME _T("OVIMECAND")
#define UINOTIFYCLASSNAME _T("OVIMENOTIFY")
#define UICHILDEXTRASIZE 12
#define FIGWL_MOUSE 4
#define FIGWL_SVRWND 8
#define FIM_CAPUTURED 0x01
#define FIM_MOVED 0x02

// Interfaces
// OVIMEUI.cpp
//<comment author='b6s'>
// Uses the managed UI function instead
/*
void DrawUIBorder(LPRECT);
void DragUI(HWND ,HWND, UINT, WPARAM, LPARAM, BOOL);

void FillSolidRect( HDC dc, LPRECT rc, COLORREF color );
void Draw3DBorder(HDC hdc, LPRECT rc, COLORREF light, COLORREF dark, int width=1);
*/
//</comment>

// UIStatus.cpp
LRESULT APIENTRY StatusWndProc(HWND, UINT, WPARAM, LPARAM);
//<comment author='b6s'>
// Uses the managed UI function instead
//void PaintStatusWindow(HWND);
//</comment>

// UIComp.cpp
LRESULT APIENTRY CompWndProc(HWND, UINT, WPARAM, LPARAM);
//<comment author='b6s'>
// Uses the managed UI function instead
//void PaintCompWindow(HWND);
//</comment>

// UICand.cpp
LRESULT APIENTRY CandWndProc(HWND, UINT, WPARAM, LPARAM);
//<comment author='b6s'>
// Uses the managed UI function instead
//void PaintCandWindow(HWND);
//</comment>

// UINotify.cpp
LRESULT APIENTRY NotifyWndProc(HWND, UINT, WPARAM, LPARAM);
//<comment author='b6s'>
// Uses the managed UI function instead
//void PaintNotifyWindow(HWND);
//</comment>

// Private variable
extern HFONT hUIFont;
extern HICON hMainIcon;
extern HWND hIMEWnd;
extern HWND hToolbar;
extern HMENU hIMESelMenu;
extern HINSTANCE hInst;
extern HINSTANCE hInstDLL;
extern wchar_t* lpCandStr;
extern int numCand;
extern wchar_t* lpCompStr;
extern wchar_t* lpNotifyStr;
extern int CompCursorPos;
extern int CompSelStart;
extern int CompSelEnd;
extern UICHILD uiCand;
extern UICHILD uiComp;
extern UICHILD uiStatus;
extern UICHILD uiNotify;
//extern std::vector<wchar_t*> IC;
extern int CurrentIC;
extern bool isChinese;
extern bool isTraditional;
extern bool isFull;
