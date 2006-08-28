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


// Extern
extern HINSTANCE hInst;
extern int CompX;
extern int CompY;
extern bool isChinese;
extern bool isFullShape;
extern DisplayServer *dsvr;
extern AVLoader *loader;
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
	// ControlHandle.cpp
	LONG ControlHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
	// NotifyHandel.cpp
	LONG NotifyHandle(HIMC, HWND, UINT, WPARAM, LPARAM);
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
#ifdef DEBUG 
    #include <stdarg.h>
    #include <stdio.h>
    inline static void DebugLog(const char* format,...) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("orz.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("orz.txt", "a")) {
		va_list args;
		va_start (args, format);
		vfprintf (fp, format, args);
		va_end (args);
		fprintf (fp, "\n");
		fclose(fp);
	}
    }

#else
    inline static void DebugLog(const char* format,...) {
    }
#endif

#endif // OVIME_h
