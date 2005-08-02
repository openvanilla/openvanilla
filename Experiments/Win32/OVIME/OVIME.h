#include <windows.h>
#include <tchar.h>
#include "imm.h"
#include "ExtraStructs.h"
#include "Vk.h"
#include "OVIMEUI.h"


// Extern
extern HINSTANCE hInst;
extern int CompX;
extern int CompY;

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
	void UpdateCandidate(LPINPUTCONTEXT, LPTSTR);
	void ClearCandidate(LPCANDIDATEINFO);
	LPTSTR UTF16toWCHAR(char *str);
	// OVVBPOJ
	// Param: 1 -> Context number, 2 -> KeyCode, 3 -> Output
	int keyevent(int n, int c, char *s);
}
#ifdef DEBUG 
    #include <stdarg.h>
    #include <stdio.h>
    inline static void DebugLog(const char* format,...) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("f:\\orz.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("f:\\orz.txt", "a")) {
		va_list args;
		va_start (args, format);
		vfprintf (fp, format, args);
		va_end (args);
		fprintf (fp, "\n");
		fclose(fp);
	}
    }
    inline static void DebugLogW(const TCHAR* format,...) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("f:\\orz.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("f:\\orz.txt", "a")) {
		va_list args;
		va_start (args, format);
		_vftprintf(fp, format, args);
		va_end (args);
		_vftprintf(fp, _T("\n"), NULL);
		fclose(fp);
	}
    }
#else
    inline static void DebugLog(const char* format,...) {
    }
    inline static void DebugLogW(const TCHAR* format,...) {
    }
#endif
