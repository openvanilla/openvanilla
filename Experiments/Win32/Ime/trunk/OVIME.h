#include <windows.h>
#include <tchar.h>
#include "imm.h"
#include "ExtraStructs.h"
#include "Vk.h"


// Extern
extern HFONT hUIFont;
extern HINSTANCE hInst;
extern HGLOBAL hUIExtra;

// Constant
#define CS_OVIME (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)
#define UICLASSNAME _T("OVIMEUI")
#define UICOMPCLASSNAME _T("OVIMECOMP")
#define UICANDCLASSNAME _T("OVIMECAND")

	
#define UICHILDEXTRASIZE 12
#define FIGWL_MOUSE 4
#define FIGWL_SVRWND 8
#define FIM_CAPUTURED 0x01
#define FIM_MOVED 0x02

#define WM_UI_COMPMOVE (WM_USER+601)

// Macro
#define GETLPCOMPSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)
#define GETLPCANDSTR(lpcs,i) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwOffset[i])

// Functions
extern "C" {
	// OVIME.cpp
	BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID);
	BOOL IMERegisterClass( HINSTANCE );
	BOOL IMEUnRegisterClass( HINSTANCE );
	// UIWnd.cpp
	LRESULT APIENTRY UIWndProc(HWND, UINT, WPARAM, LPARAM);
	void DrawUIBorder(LPRECT);
	void DragUI(HWND ,HWND, UINT, WPARAM, LPARAM, BOOL);
	// UIComp.cpp
	LRESULT APIENTRY CompWndProc(HWND, UINT, WPARAM, LPARAM);
	void CreateCompWindow(HWND, LPUIEXTRA);
	void MoveCompWindow(HWND, LPUIEXTRA, LPINPUTCONTEXT);
	void PaintCompWindow(HWND);
	void HideCompWindow(LPUIEXTRA);
	// UICand.cpp
	LRESULT APIENTRY CandWndProc(HWND, UINT, WPARAM, LPARAM);
	void CreateCandWindow(HWND, LPUIEXTRA);
	void MoveCandWindow(HWND, LPUIEXTRA, LPINPUTCONTEXT);
	void PaintCandWindow(HWND);
	void HideCandWindow(LPUIEXTRA);
	// ControlHandle.cpp
	LONG ControlHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// NotifyHandel.cpp
	LONG NotifyHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// IME.cpp
	BOOL MyIsIMEMessage(UINT);
	void MyGenerateMessage(HIMC, UINT, WPARAM, LPARAM);
	BOOL MyGenerateMessageToTransKey(LPDWORD, UINT*, UINT, WPARAM, LPARAM);
	// Utils.cpp
	void MakeCompStr(LPMYPRIVATE, LPCOMPOSITIONSTRING);
	void InitCompStr(LPCOMPOSITIONSTRING);
	void InitCandInfo(LPCANDIDATEINFO);
	void UpdateCandidate(LPINPUTCONTEXT, LPTSTR);
	void ClearCandidate(LPCANDIDATEINFO);
	LPTSTR UTF16toWCHAR(char *);

	void DebugLog(char *, void*);
	void DebugLogW(LPTSTR format, void *p);

	
	// OVVBPOJ
	int _stdcall keyevent(int c, char *s);
}