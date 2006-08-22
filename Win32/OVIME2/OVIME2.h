#ifndef	_PCMANIME_H_
#define	_PCMANIME_H_

#include <windows.h>
#include <tchar.h>
#include <immdev.h>
#include "OVIME2Client.h"

extern HINSTANCE g_dllInst;
extern bool g_isWindowNT;
extern bool g_useUnicode;

extern const TCHAR g_OVIMEClass[];
extern const TCHAR g_compWndClass[];
extern const TCHAR g_candWndClass[];
extern const TCHAR g_statusWndClass[];

#define	WM_IME_RELOADCONFIG		(WM_APP+1)

#define DEF_FONT_SIZE           16

extern OVIME2Client* g_client;

inline BOOL IsImeMessage(UINT msg)
{
	return ( (msg >= WM_IME_STARTCOMPOSITION && msg <= WM_IME_KEYLAST)
			|| (msg >= WM_IME_SETCONTEXT && msg <= WM_IME_KEYUP) );
}

void LoadConfig();
void ToggleChiEngMode(HIMC hIMC);
void ToggleFullShapeMode(HIMC hIMC);

BOOL GenerateIMEMessage(HIMC hIMC, UINT msg, WPARAM wp=0, LPARAM lp=0);

void ToggleChiEngMode(HIMC hIMC);

void ConfigureChewingIME(HWND parent);

typedef struct _tagTRANSMSG {
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
} TRANSMSG, *LPTRANSMSG;

struct KeyInfo
{
	UINT repeatCount:16;
	UINT scanCode:8;
	UINT isExtended:1;
	UINT reserved:4;
	UINT contextCode:1;
	UINT prevKeyState:1;
	UINT isKeyUp:1;	// transition state
};

inline KeyInfo GetKeyInfo(LPARAM lparam)
{	return *(KeyInfo*)&lparam;	}

inline bool IsKeyDown(BYTE keystate){ return !!(keystate & 0xF0); }
inline bool IsKeyToggled(BYTE keystate){ return !!(keystate & 0x0F); }

BOOL ProcessKeyEvent( UINT key, KeyInfo ki, const BYTE* keystate );

#endif
