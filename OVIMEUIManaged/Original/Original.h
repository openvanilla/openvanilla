#include <vector>
#include <windows.h>
#include <tchar.h>
#include "OVIMEUI.h"

#define CS_OVIME (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)
#define UICLASSNAME _T("OVIMEUI")
#define UISTATUSCLASSNAME _T("OVIMESTATUS")
#define UICOMPCLASSNAME _T("OVIMECOMP")
#define UICANDCLASSNAME _T("OVIMECAND")
#define UICHILDEXTRASIZE 12
#define FIGWL_MOUSE 4
#define FIGWL_SVRWND 8
#define FIM_CAPUTURED 0x01
#define FIM_MOVED 0x02

// Interfaces
// OVIMEUI.cpp
void DrawUIBorder(LPRECT);
void DragUI(HWND ,HWND, UINT, WPARAM, LPARAM, BOOL);
// UIStatus.cpp
LRESULT APIENTRY StatusWndProc(HWND, UINT, WPARAM, LPARAM);
void PaintStatusWindow(HWND);
// UIComp.cpp
LRESULT APIENTRY CompWndProc(HWND, UINT, WPARAM, LPARAM);
void PaintCompWindow(HWND);
// UICand.cpp
LRESULT APIENTRY CandWndProc(HWND, UINT, WPARAM, LPARAM);
void PaintCandWindow(HWND);

// Private variable
extern HFONT hUIFont;
extern HINSTANCE hInst;
extern LPTSTR lpCandStr;
extern LPTSTR lpCompStr;
extern UICHILD uiCand;
extern UICHILD uiComp;
extern UICHILD uiStatus;
extern std::vector<LPTSTR> IC;
extern int CurrentIC;
