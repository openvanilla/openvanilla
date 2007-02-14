#ifndef OVIME_h
#define OVIME_h

#include <windows.h>
#include <tchar.h>
#include <immdev.h>

#include "ExtraStructs.h"
#include "Vk.h"
#include "OVIMEUI.h"
#include "AVDisplayServer.h"
#include "DisplayServer.h"
#include "AVLoader.h"
#include "ImmModel.h"
#include "ImmController.h"

// Extern
extern HINSTANCE hInst;
extern int CompX;
extern int CompY;
//James add
extern int CandX;
extern int CandY;
extern bool hasCompStarted;
extern bool isCompEnabled;

extern bool isChinese;
extern bool isFullShape;
extern bool isTraditional;
extern DisplayServer *dsvr;
//extern AVLoader *loader;
//extern MyOVDisplayServer dsvr;

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

#define WM_UI_COMPMOVE (WM_APP+600)
#define WM_IME_RELOADCONFIG (WM_APP+601)

// Macro
#define GETLPCOMPSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTSTR(lpcs) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)
#define GETLPCANDSTR(lpcs,i) (wchar_t*)((LPBYTE)(lpcs) + (lpcs)->dwOffset[i])

// Functions
extern "C" {
	// OVIME.cpp
	BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID);
	// UIWnd.cpp
	LRESULT APIENTRY UIWndProc(HWND, UINT, WPARAM, LPARAM);
	void RefreshUI(HWND hWnd);
	// ControlHandle.cpp
	LRESULT ControlHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// NotifyHandel.cpp
	LRESULT NotifyHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// IME.cpp
	void MyGenerateMessage(HIMC, UINT, WPARAM, LPARAM);
	BOOL MyGenerateMessageToTransKey(LPDWORD, UINT*, UINT, WPARAM, LPARAM);
	// Utils.cpp
	void MakeCompStr(LPMYPRIVATE, LPCOMPOSITIONSTRING);
	void InitCompStr(LPCOMPOSITIONSTRING);
	void InitCandInfo(LPCANDIDATEINFO);
	void UpdateCandidate(LPINPUTCONTEXT, const wchar_t*);
	void ClearCandidate(LPCANDIDATEINFO);
	
	// OVVBPOJ
	// Param: 1 -> Context number, 2 -> KeyCode, 3 -> Output
	// Return: Length of returned string.
	/*
	int KeyEvent(int n, int c);
	// Param: 1 -> Output
	// Return: Number of names.
	int ModuleName(int , char *);
	void SetDisplayServer(AVDisplayServer*);
	void InitLoader();
	void ShutdownLoader();
	void ReloadConfig();
	*/
}
#endif // OVIME_h
