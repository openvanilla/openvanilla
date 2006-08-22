// ChewingIME.cpp : 定義 DLL 應用程式的進入點。
//

#include "OVIME2.h"

#include "CompStr.h"
#include "CandList.h"
#include "IMCLock.h"

#include "resource.h"

#include <winreg.h>
#include <shlobj.h>
#include <windowsx.h>

#include "CompWnd.h"
#include "CandWnd.h"
#include "StatusWnd.h"

#include "IMEUI.h"

HINSTANCE g_dllInst = NULL;
bool g_isWindowNT = false;
bool g_useUnicode = true;	// Under Windows 95, IME has no unicode support

long g_shiftPressedTime = -1;

OVIME2Client* g_client = NULL;

BOOL FilterKeyByChewing( IMCLock& imc, UINT key, KeyInfo ki, const BYTE* keystate );

int ControlListCursor( UINT &key, CandList* candList );

BOOL GenerateIMEMessage( HIMC hIMC, UINT msg, WPARAM wp, LPARAM lp )
{
	if(!hIMC)
		return FALSE;

	BOOL success = FALSE;
	INPUTCONTEXT* ic = ImmLockIMC(hIMC);
	if(!ic)
		return FALSE;

	HIMCC hbuf = ImmReSizeIMCC( ic->hMsgBuf, sizeof(TRANSMSG) * (ic->dwNumMsgBuf + 1) );
	if( hbuf )
	{
		ic->hMsgBuf = hbuf;
		TRANSMSG* pbuf = (TRANSMSG*)ImmLockIMCC( hbuf );
		if( pbuf )
		{
			pbuf[ic->dwNumMsgBuf].message = msg;
			pbuf[ic->dwNumMsgBuf].wParam = wp;
			pbuf[ic->dwNumMsgBuf].lParam = lp;
			ic->dwNumMsgBuf++;
			success = TRUE;
			ImmUnlockIMCC(hbuf);
		}
	}
	ImmUnlockIMC(hIMC);

	if( success )
		success = ImmGenerateMessage(hIMC);

	return success;
}


BOOL    APIENTRY ImeInquire(LPIMEINFO lpIMEInfo, LPTSTR lpszUIClass, LPCTSTR lpszOptions)
{
	_tcscpy( lpszUIClass, g_OVIMEClass );

	lpIMEInfo->fdwConversionCaps = IME_CMODE_NOCONVERSION | IME_CMODE_FULLSHAPE | IME_CMODE_CHINESE;
	lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
	lpIMEInfo->fdwUICaps = UI_CAP_2700;
	lpIMEInfo->fdwSCSCaps = 0;
	lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;
	lpIMEInfo->fdwProperty = /*IME_PROP_IGNORE_UPKEYS|*/IME_PROP_AT_CARET|IME_PROP_KBD_CHAR_FIRST|
							 IME_PROP_CANDLIST_START_FROM_1|IME_PROP_COMPLETE_ON_UNSELECT
							 |IME_PROP_END_UNLOAD;

	if( g_useUnicode )	{
		 lpIMEInfo->fdwProperty |= IME_PROP_UNICODE;
#ifndef UNICODE
		int len = strlen(g_OVIMEClass) + 1;
		MultiByteToWideChar( CP_ACP, 0, g_OVIMEClass, len, (LPWSTR)lpszUIClass, len );
#endif
	}

/*
	if(g_isWindowsNT && (DWORD(lpszOptions) & IME_SYSTEMINFO_WINLOGON ))
	{
		// Configuration should be disabled.
	}
*/
	return TRUE;
}

BOOL    APIENTRY ImeConfigure(HKL hkl, HWND hWnd, DWORD dwMode, LPVOID pRegisterWord)
{
	if( IME_CONFIG_GENERAL == dwMode )
	{
		TCHAR buf[MAX_PATH];
		GetWindowsDirectory( buf, MAX_PATH );
		_tcscat( buf, _T("\\OpenVanilla\\OVPreferences.exe"));
		ShellExecute( hWnd, _T("open"), buf, NULL, NULL, SW_SHOWNORMAL );
	}
	InvalidateRect(hWnd,NULL,FALSE);
	return TRUE;
}

DWORD   APIENTRY ImeConversionList(HIMC, LPCTSTR, LPCANDIDATELIST, DWORD dwBufLen, UINT uFlag)
{
	return 0;
}

BOOL    APIENTRY ImeDestroy(UINT)
{
	return TRUE;
}

LRESULT APIENTRY ImeEscape(HIMC, UINT, LPVOID)
{
	return FALSE;
}

void ToggleChiEngMode(HIMC hIMC)
{
	bool isChinese;
//	if( g_enableShift )
//	{
		DWORD conv, sentence;
		ImmGetConversionStatus( hIMC, &conv, &sentence);
		isChinese = !!(conv & IME_CMODE_CHINESE);
		if( isChinese )
			conv &= ~IME_CMODE_CHINESE;
		else
			conv |= IME_CMODE_CHINESE;
		ImmSetConversionStatus( hIMC, conv, sentence);
//	}
//	else
//	{
//		isChinese = !LOBYTE(GetKeyState(VK_CAPITAL));
//		BYTE scan = MapVirtualKey(VK_CAPITAL, 0);
//		keybd_event( VK_CAPITAL, MapVirtualKey(VK_CAPITAL, 0), 0, 0  );	// Capslock on/off
//		keybd_event( VK_CAPITAL, MapVirtualKey(VK_CAPITAL, 0), KEYEVENTF_KEYUP, 0 );	// Capslock on/off
//	}

	isChinese = !isChinese;
/*
	if( g_client )
	{
		if( isChinese )	// We need Chinese mode
		{
			if( !g_client->ChineseMode() )	{// Chewing is in English mode
				g_client->Capslock();
				if( ! LOBYTE(GetKeyState(VK_CAPITAL)) && g_enableCapsLock )	// no CapsLock
						g_client->Capslock();	// Switch Chewing to Chinese mode
			}
		}
		else	// We need English mode
		{
			if(  g_client->ChineseMode() )	// Chewing is in Chinese mode
				g_client->Capslock();	// Switch Chewing to English mode
		}
	}
*/
}

void ToggleFullShapeMode(HIMC hIMC)
{
	DWORD conv, sentence;
	ImmGetConversionStatus( hIMC, &conv, &sentence);
	bool isFullShape = !!(conv & IME_CMODE_FULLSHAPE);
	if( isFullShape )
		conv &= ~IME_CMODE_FULLSHAPE;
	else
		conv |= IME_CMODE_FULLSHAPE;
	ImmSetConversionStatus( hIMC, conv, sentence);
}

BOOL ProcessCandidateList( HIMC hIMC, HIMCC hCandInfo )
{
/*
	// Candidate list opened
	int pageCount = 0;
	CandList* candList = (CandList*)ImmLockIMCC(hCandInfo);
	if( !candList )
		return FALSE;
	if( pageCount = g_client->Candidate() )
	{
		candList->setPageStart( g_client->CurrentPage() * g_client->ChoicePerPage() );
		candList->setPageSize( g_client->ChoicePerPage() );

		int old_total_count = candList->getTotalCount();
		candList->setTotalCount( g_client->TotalChoice() );

		if( candList->getSelection() < candList->getPageStart() ||
			candList->getSelection() > candList->getTotalCount() ||
			candList->getSelection() > candList->getPageStart() + candList->getPageSize() - 1 ) {
				candList->setSelection( candList->getPageStart() );
		}

		if(candList->getPageStart() == 0 && candList->getSelection() == 0 )
		{
			int TotalChoice = g_client->TotalChoice();
			for( int i = 0; i < TotalChoice; ++i )
			{
				char* cand = g_client->Selection( i );
				if( cand )	{
					wchar_t wcand[10];
					MultiByteToWideChar( CP_UTF8, 0, cand, -1, wcand, sizeof(wcand)/sizeof(wchar_t) );
					candList->setCand( i , wcand );
					free(cand);
				}
				else
					candList->setCand( i , L"" );
			}
		}
		ImmUnlockIMCC( hCandInfo );

		if( 0 == old_total_count )
			GenerateIMEMessage( hIMC, WM_IME_NOTIFY, IMN_OPENCANDIDATE, 1 );
		GenerateIMEMessage( hIMC, WM_IME_NOTIFY, IMN_CHANGECANDIDATE, 1 );
		return TRUE;
	}
	else if( candList->getTotalCount() > 0 )
	{
		ImmUnlockIMCC( hCandInfo );
		GenerateIMEMessage( hIMC, WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 1 );
		candList->setTotalCount(0);
	}
*/
	return FALSE;
}


BOOL    APIENTRY ImeProcessKey(HIMC hIMC, UINT uVirKey, LPARAM lParam, CONST BYTE *lpbKeyState )
{
	if( !hIMC )
		return FALSE;

	if( GetKeyInfo(lParam).isKeyUp )	{	// Key up
/*
		if( g_enableShift )		{
			if( g_shiftPressedTime > 0 )	{
				if( uVirKey == VK_SHIFT && (GetTickCount() - g_shiftPressedTime) <= 300 )
				{
					// Toggle Chinese/English mode.
					ToggleChiEngMode(hIMC);
				}
				g_shiftPressedTime = -1;
			}
		}
*/
		return FALSE;
	}

/*
	if( g_enableShift )	{
		if( uVirKey == VK_SHIFT  )	{
			if( ! IsKeyDown( lpbKeyState[VK_CONTROL] ) && g_shiftPressedTime < 0 )
				g_shiftPressedTime = GetTickCount();
		}
		else if( g_shiftPressedTime > 0 )
			g_shiftPressedTime = -1;
	}
*/

    //  Is server alive? Or the server could be different one.
    if ( g_client != NULL )
        g_client->CheckServer();

	// IME Toggle key : Ctrl + Space & Shift + Space
	if( LOWORD(uVirKey) == VK_SPACE && 
		(IsKeyDown( lpbKeyState[VK_CONTROL]) || IsKeyDown( lpbKeyState[VK_SHIFT])) ) {
		return TRUE;	// Eat the message
	}

	if( ! g_client->keyEvent( uVirKey, lParam, lpbKeyState ) ) {
		return TRUE;
	}

	wstring comp_str;
	wstring result_str;
	wstring notify_str;
	int cursor_pos;
	int mark_from;
	int mark_to;
	bool has_cand;
	g_client->getData( comp_str, result_str, notify_str, 
		has_cand, cursor_pos, mark_from, mark_to );

	INPUTCONTEXT* ic = (INPUTCONTEXT*)ImmLockIMC(hIMC);
	if( has_cand )	{
		ProcessCandidateList( hIMC, ic->hCandInfo );
		ImmUnlockIMC( hIMC );
		return TRUE;
	}

	CompStr* cs = (CompStr*)ImmLockIMCC(ic->hCompStr);
	bool composition_started = !!*cs->getCompStr();

	cs->setCompStr( comp_str.c_str() );
	cs->setResultStr( result_str.c_str() );
	cs->setCursorPos( cursor_pos );
	cs->beforeGenerateMsg();

	WORD word = *(WORD*)cs->getCompStr();
	bool is_empty = !*cs->getCompStr();
	bool has_result = !!*cs->getResultStr();

	if( ! composition_started && *cs->getCompStr() )
	{
		GenerateIMEMessage( hIMC, WM_IME_STARTCOMPOSITION );
		composition_started = true;
	}

	ImmUnlockIMCC(ic->hCompStr);

	GenerateIMEMessage( hIMC, WM_IME_COMPOSITION, 
				(composition_started ? word : 0),
				(GCS_COMPSTR|GCS_COMPATTR|GCS_COMPREADSTR|GCS_COMPREADATTR|
				GCS_COMPCLAUSE|GCS_COMPREADCLAUSE|
				GCS_COMPREADATTR|GCS_CURSORPOS|
				(composition_started ? GCS_DELTASTART : 0 )|
				(has_result ? (GCS_RESULTCLAUSE|GCS_RESULTSTR|GCS_RESULTREADSTR|GCS_RESULTREADCLAUSE):0 ) ) );

	if( is_empty && composition_started )
		GenerateIMEMessage( hIMC, WM_IME_ENDCOMPOSITION );

	ImmUnlockIMC(hIMC);
	return TRUE;
}

BOOL    APIENTRY ImeSelect(HIMC hIMC, BOOL fSelect)
{
	IMCLock imc( hIMC );
	INPUTCONTEXT* ic = imc.getIC();
	if( !ic )
		return FALSE;

	if(fSelect)
	{
		ic->fOpen = TRUE;

		ImmReSizeIMCC( imc.getIC()->hCompStr, sizeof(CompStr) );
		CompStr* cs = imc.getCompStr();
		if(!cs)
			return FALSE;
		cs = new (cs) CompStr;	// placement new

		ImmReSizeIMCC( imc.getIC()->hCandInfo, sizeof(CandList) );
		CandList* cl = imc.getCandList();
		if(!cl)
			return FALSE;
		cl = new (cl) CandList;	// placement new

		if( ! g_client ) {	// Not connected
			g_client = new OVIME2Client();
		}

		if( !(ic->fdwInit & INIT_CONVERSION) )		// Initialize
		{
//			ic->fdwConversion = g_defaultEnglish ? IME_CMODE_CHARCODE : IME_CMODE_CHINESE;
			ic->fdwConversion = IME_CMODE_CHINESE;

//			if( g_defaultFullSpace )
//				ic->fdwConversion |=  IME_CMODE_FULLSHAPE;
//			else
				ic->fdwConversion &=  ~IME_CMODE_FULLSHAPE;
			ic->fdwInit |= INIT_CONVERSION;
		}
		if( !(ic->fdwInit & INIT_STATUSWNDPOS) )
		{
			RECT rc;
			IMEUI::getWorkingArea( &rc, ic->hWnd );
			ic->ptStatusWndPos.x = rc.right - (9+20*3+4) - 150;
			ic->ptStatusWndPos.y = rc.bottom - 26;
			ic->fdwInit |= INIT_STATUSWNDPOS;
		}
		if( !(ic->fdwInit & INIT_LOGFONT) )
		{
			// TODO: initialize font here
			ic->lfFont;
		}

		// Set Chinese or English mode
/*
		if( imc.isChinese() )	//	Chinese mode
		{
			if( g_client )
			{
				if( g_client->ChineseMode() )
				{
					if(  LOBYTE(GetKeyState(VK_CAPITAL)) && g_enableCapsLock ) 
							g_client->Capslock();
				}
				else if( ! LOBYTE(GetKeyState(VK_CAPITAL)) && g_enableCapsLock )
						g_client->Capslock();
			}
		}
		else
		{
			ic->fdwConversion &= ~IME_CMODE_CHINESE;
		}
*/
	}
	else
	{
		CompStr* cs = imc.getCompStr();
		cs->~CompStr();	// delete cs;
		CandList* cl = imc.getCandList();
		cl->~CandList();	// delete cl;
	}
	return TRUE;
}

//  Activates or deactivates an input context and notifies the IME of the newly active input context. 
//  The IME can use the notification for initialization.
BOOL    APIENTRY ImeSetActiveContext(HIMC hIMC, BOOL fFlag)
{
	if( fFlag )
	{
	}
	return TRUE;
}

UINT    APIENTRY ImeToAsciiEx(UINT uVirtKey, UINT uScaCode, CONST LPBYTE lpbKeyState, LPDWORD lpdwTransBuf, UINT fuState, HIMC)
{
	return FALSE;
}

BOOL CommitBuffer( IMCLock& imc )
{
	CompStr* cs = imc.getCompStr();
	if( !cs )
		return FALSE;

	if( *cs->getCompStr() )
	{
/*
		// FIX #15284.
        //  If candidate wnd is open, send ESC key to lib, 
        //  then submit composition string, close cand wnd
    	if( g_client->Candidate() > 0 )
        {
//            g_client->Esc();
//            g_client->Enter();
            GenerateIMEMessage( imc.getHIMC(), WM_IME_NOTIFY, IMN_CLOSECANDIDATE, 1 );
            CandList* candList = (CandList*)ImmLockIMCC(imc.getIC()->hCandInfo);
            candList->setTotalCount(0);
        }

        if( g_client )
		{
//			g_client->Enter();	// Commit
			char* cstr = NULL;
			if( g_client->CommitReady() && 
				(cstr = g_client->CommitStr()) )	{
				free(cstr);
			}
		}
*/
		cs->setResultStr( cs->getCompStr() );
		cs->setZuin( L"" );
		cs->setCompStr( L"" );
		cs->setCursorPos(0);
		cs->beforeGenerateMsg();

		GenerateIMEMessage( imc.getHIMC(), WM_IME_COMPOSITION, 
			0,
			(GCS_RESULTSTR|GCS_COMPSTR|GCS_COMPATTR|GCS_COMPREADSTR|
			GCS_COMPREADATTR|GCS_CURSORPOS|GCS_DELTASTART) );

		GenerateIMEMessage( imc.getHIMC(), WM_IME_ENDCOMPOSITION );
	}
	return TRUE;
}


BOOL    APIENTRY NotifyIME(HIMC hIMC, DWORD dwAction, DWORD dwIndex, DWORD dwValue )
{
	if( !hIMC )
		return FALSE;

	switch( dwAction )
	{
	case NI_OPENCANDIDATE:
		break;
	case NI_CLOSECANDIDATE:
		break;
	case NI_SELECTCANDIDATESTR:
		break;
	case NI_CHANGECANDIDATELIST:
		break;
	case NI_SETCANDIDATE_PAGESTART:
		break;
	case NI_SETCANDIDATE_PAGESIZE:
		break;
	case NI_CONTEXTUPDATED:
		{
			switch(dwValue)
			{
			case IMC_SETCANDIDATEPOS:
				break;
			case IMC_SETCOMPOSITIONFONT:
				break;
			case IMC_SETCOMPOSITIONWINDOW:
				break;
			case IMC_SETCONVERSIONMODE:
				break;
			case IMC_SETSENTENCEMODE:
				break;
			case IMC_SETOPENSTATUS :
				break;
			}
			break;
		}
	case NI_COMPOSITIONSTR:
		{
			IMCLock imc( hIMC );
			CompStr* cs = imc.getCompStr();
			if( !cs )
				return FALSE;
			switch( dwIndex )
			{
			case CPS_COMPLETE:
				return CommitBuffer( imc );
				break;
			case CPS_CONVERT:
				break;
			case CPS_CANCEL:
				cs->setCompStr( L"" );
				cs->setZuin( L"" );
				break;
			}
		}
		break;
	}
	return TRUE;
}

BOOL    APIENTRY ImeRegisterWord(LPCTSTR, DWORD, LPCTSTR)
{
	return 0;
}

BOOL    APIENTRY ImeUnregisterWord(LPCTSTR, DWORD, LPCTSTR)
{
	return 0;
}

UINT    APIENTRY ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF)
{
	return 0;
}

DWORD WINAPI  ImeGetImeMenuItems(  HIMC  hIMC,  DWORD  dwFlags,  DWORD  dwType,
    LPIMEMENUITEMINFO  lpImeParentMenu, LPIMEMENUITEMINFO  lpImeMenu, DWORD  dwSize )
{
	return 0;
}

UINT    APIENTRY ImeEnumRegisterWord(REGISTERWORDENUMPROC, LPCTSTR, DWORD, LPCTSTR, LPVOID)
{
	return 0;
}

BOOL    APIENTRY ImeSetCompositionString(HIMC, DWORD dwIndex, LPCVOID lpComp, DWORD, LPCVOID lpRead, DWORD)
{
	return FALSE;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls((HMODULE)hModule);
			g_dllInst = (HINSTANCE)hModule;

			OSVERSIONINFO osv = {0};
			osv.dwOSVersionInfoSize = sizeof(osv);
			GetVersionEx( &osv );

			g_isWindowNT = (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);

			if( osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )	{
				// We are under Windows 95, 98, ME
				if( osv.dwMajorVersion == 4 && osv.dwMinorVersion == 0 )	// Windows  95
					g_useUnicode = false;	// There is no unicode IME support in Windows 95
			}

			if( !IMEUI::registerUIClasses() )
				return FALSE;
			break;
		}

	case DLL_PROCESS_DETACH:
		IMEUI::unregisterUIClasses();

		if( g_client )
			delete g_client;

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}

static int _InvertCase(int key)
{
	if( key >= 'A' && key <= 'Z' )
		key = tolower(key);
	else if( key >= 'a' && key <= 'z' )
		key = toupper( key );
    return  key;
}

bool _FilterKey( int key ) {
	string table = "`~=+[{]}|\\'\"";
	return table.find( key ) != -1;
}

/*
BOOL FilterKeyByChewing( IMCLock& imc, UINT key, KeyInfo ki, const BYTE* keystate )
{
	bool isChinese = imc.isChinese();
	bool isFullShape = imc.isFullShape();

	if( !g_client )
	{
		if( ! isChinese && !isFullShape )
			return FALSE;	//delay the loading of Chewing engine
		// Enable numpad even in Chinese mode
		if( key >= VK_NUMPAD0 && key <= VK_DIVIDE )
			return FALSE;
		if( ! (g_client = LoadChewingEngine()) )
			return FALSE;
		if( ( IsKeyToggled( keystate[VK_CAPITAL]) && g_enableCapsLock ) || !isChinese )
				g_client->Capslock();	// switch to English mode
	}
	else
	{
		// Correct Chinese/English mode
		if( g_enableCapsLock ) {
			if( g_client->ChineseMode() )
			{
				if( IsKeyToggled( keystate[VK_CAPITAL]) || !isChinese )
						g_client->Capslock();
			}
			else if( !IsKeyToggled( keystate[VK_CAPITAL]) && isChinese )
					g_client->Capslock();
		}
		else if( isChinese ) {
			if( !g_client->ChineseMode() ) {
				g_client->Capslock();
			}
		}
		// When type english, zuin str should be clean.
		if( IsKeyDown( keystate[VK_SHIFT] ) || !g_client->ChineseMode() || !isChinese )
			if( g_client->ZuinStr() )
				g_client->Esc();

		CompStr* cs = imc.getCompStr();
		// In English mode, Bypass chewing if there is no composition string but may be call symbol table!
		if( ( !cs || !*cs->getCompStr() ) && !g_client->Candidate() )
		{
			if( isChinese )
			{
				// Enable numpad even in Chinese mode
				if( key >= VK_NUMPAD0 && key <= VK_DIVIDE )
					return FALSE;
				if( ( key == VK_SPACE && !isFullShape ) || key == VK_RETURN )
					return FALSE;
				char ascii[2];
				int ret = ToAscii( key, ki.scanCode, (BYTE*)keystate, (LPWORD)ascii, 0);
				if( ret )
					if( ascii[0] >= 'A' && ascii[0] <= 'Z' &&
					!IsKeyToggled( keystate[VK_CAPITAL]) && g_shiftCapital )
						return FALSE;
			}
			else if( !isFullShape )	// Chewing has been loaded but in English mode
				return FALSE;
		}


	}

	CandList* candList = imc.getCandList();

	if( IsKeyDown( keystate[VK_CONTROL] ) )
	{
		if(  key >= '0' && key <= '9' )
		{
			g_client->CtrlNum( key );
			CompStr* cs = imc.getCompStr();
			if( g_client->ShowMsgLen() > 0 )
			{
				char* msg = g_client->ShowMsg();
				if( msg )
				{
					wchar_t wmsg[100];
					MultiByteToWideChar( CP_UTF8, 0, msg, -1, wmsg, sizeof(wmsg)/sizeof(wchar_t) );
					cs->setShowMsg( wmsg );
					free(msg);
					GenerateIMEMessage( imc.getHIMC(), WM_IME_NOTIFY, IMN_PRIVATE, 0 );
				}
			}
			else if( ! g_client->Candidate() )
				return FALSE;
			candList->setSelection( 0 );
			return ! g_client->KeystrokeIgnore();
		}
		else
			return FALSE;
	}
	if( g_client->Candidate() > 0 )
	{	
		// control candlsit cursor
		if( g_cursorCandList ) {
			if( ControlListCursor( key, candList ) )
				return TRUE;
		}
		else {
			switch( key )
			{
			case VK_NEXT:
				key = VK_RIGHT;
				break;
			case VK_PRIOR:
				key = VK_LEFT;
				break;
			// Fix #15218, allow backspace/enter to close cand win
			case    VK_BACK: 
			case    VK_RETURN:
				key = VK_ESCAPE;
				break;
			}
		}
	}
	else if( imc.isVerticalComp() && ! g_fixCompWnd )
	{
		//Vertical composition mode
		switch( key )
		{
		case VK_LEFT:
		case VK_RIGHT:
			key = VK_DOWN;
			break;
		case VK_UP:
			key = VK_LEFT;
			break;
		case VK_DOWN:
			key = VK_RIGHT;
			break;
		}
	}
	switch( key )
	{
	case VK_LEFT:
		if( IsKeyDown( keystate[VK_SHIFT] ) )
			g_client->ShiftLeft();
		else
			g_client->Left();
		break;
	case VK_RIGHT:
		if( IsKeyDown( keystate[VK_SHIFT] ) )
			g_client->ShiftRight();
		else
			g_client->Right();
		break;
	case VK_UP:
		g_client->Up();
		candList->setSelection( 0 );
		break;
	case VK_DOWN:
		g_client->Down();
		candList->setSelection( 0 );
		break;
	case VK_HOME:
		g_client->Home();
		break;
	case VK_END:
		g_client->End();
		break;
	case VK_BACK:
		g_client->Backspace();
		break;
	case VK_RETURN:
		g_client->Enter();
		break;
	case VK_ESCAPE:
		g_client->Esc();
		break;
	case VK_DELETE:
		g_client->Delete();
		break;
	case VK_TAB:
		g_client->Tab();
		break;
	case VK_SPACE:
		if( !g_client->Candidate() )
			candList->setSelection( 0 );
		g_client->Space();
		break;
	case VK_CAPITAL:
		if( isChinese && g_enableCapsLock ) {
			g_client->Capslock();
		}
		return FALSE;
		break;
	default:
		{
			char ascii[2];
			int ret = ToAscii( key, ki.scanCode, (BYTE*)keystate, (LPWORD)ascii, 0);
			if( ret )
			{
				if( key >= VK_NUMPAD0 && key <= VK_DIVIDE )
				{
					g_client->NumPad( ascii[0] );
					return TRUE;
				}
				key = ascii[0];
			}
			else
				return FALSE;

			if( isChinese )
			{

				//handle symbol
				if( !g_shiftFullShape && !isalpha( key ) && !isFullShape )
					if( IsKeyDown(keystate[VK_SHIFT]) || _FilterKey( key ) ) {
						g_client->Capslock();
						g_client->SetFullShape( FALSE );
						g_client->Key( key );
						return TRUE;
					}
				
				if( !g_enableCapsLock ) {
					if( IsKeyToggled( keystate[VK_CAPITAL] ) ) {
						if( IsKeyDown(keystate[VK_SHIFT]) ) {
							if( key >= 'a' && key <= 'z' ) {
								if( !g_shiftCapital )
									key = _InvertCase( key );
								g_client->Capslock();
								g_client->Key( key );
								return TRUE;
							}
						}
					}
					else {
						if( IsKeyDown(keystate[VK_SHIFT]) ) 
							if( key >= 'A' && key <= 'Z' ) {
								if( !g_shiftCapital )
									key = _InvertCase( key );
								g_client->Capslock();
								g_client->Key( key );
								return TRUE;
							}
					}
				} 
				else {
					if( IsKeyToggled( keystate[VK_CAPITAL] ) )
					{
						key = _InvertCase(key);
						if ( IsKeyDown(keystate[VK_SHIFT]) )
						{
							g_client->Key( key );
							return TRUE;
						}
					}
					if( g_shiftCapital && key >= 'A' && key <= 'Z' )
					{
						/// FIXME: Temporary hack to enable typing
						//  capital English characters in Chinese mode
						//  with shift key. There should be a better way.
						//  We temporary switch to English mode here, and
						//  that's ok since this wrong state will be corrected 
						//  when FilterKeyByChewing() is called next time.
						//  We always examine whether English/Chinese mode
						//  is properly set and correct it in the beginning
						//  of this function.
						g_client->Capslock();
						g_client->Key( key );
						return TRUE;
					}
					else {
						g_client->Key( key );
						return TRUE;
					}
				}
			}
			//if chewing is chinese mode and key is capital
			if( isupper( key ) && g_client->ChineseMode() )	
				key = tolower( key );
			g_client->Key( key );
		}
	}
	return ! g_client->KeystrokeIgnore();
}
*/

int ControlListCursor( UINT &key, CandList* candList ) {
/*
	switch( key )
	{
	case VK_UP:
		candList->setSelection( candList->getSelection() - g_candPerRow );
		if( candList->getSelection() < 0 ) {
			g_client->Up();
			candList->setSelection( 0 );
		}
		if( candList->getSelection() < candList->getPageStart() ) {
			g_client->Left();
			candList->setSelection( 0 );
		}
		return ! g_client->KeystrokeIgnore();
	case VK_DOWN:
		candList->setSelection( candList->getSelection() + g_candPerRow );
		if( candList->getSelection() > candList->getTotalCount() - 1 ) {
			g_client->Down();
			candList->setSelection( 0 );
		}
		if( candList->getSelection() - candList->getPageStart() > candList->getPageSize() - 1 ) {
			g_client->Right();
			candList->setSelection( 0 );
		}
		return ! g_client->KeystrokeIgnore();
	case VK_LEFT:
		if( ( candList->getSelection() % candList->getPageSize() ) % g_candPerRow == 0 ) {
			g_client->Left();
			if( g_client->CurrentPage() == g_client->TotalPage() - 1 ) {
				g_client->Up();
				candList->setSelection( 0 );
			}
		} else {
			candList->setSelection( candList->getSelection() - 1 );
		}
		return ! g_client->KeystrokeIgnore();
		break;
	case VK_RIGHT:
		if( ( candList->getSelection() % candList->getPageSize() ) % g_candPerRow == g_candPerRow - 1 ||
			( ( candList->getSelection() % candList->getPageSize() ) == candList->getPageSize() - 1 ) ||
			candList->getSelection() == candList->getTotalCount() - 1 ) {
			g_client->Right();
			if( g_client->CurrentPage() == 0 ) {
				g_client->Down();
				candList->setSelection( 0 );
			}
		} else {
			candList->setSelection( candList->getSelection() + 1 );
		}
		return ! g_client->KeystrokeIgnore();
		break;
	case VK_NEXT:
		key = VK_RIGHT;
		break;
	case VK_PRIOR:
		key = VK_LEFT;
		break;
	case VK_RETURN:
		g_client->Key( *(g_selKeyNames[ g_selKeyType ] + candList->getSelection() % candList->getPageSize()) );
		candList->setSelection( 0 );
		return ! g_client->KeystrokeIgnore();
		break;
	case    VK_BACK:
	case	VK_ESCAPE:
		candList->setSelection( 0 );
		key = VK_ESCAPE;
		break;
	}
*/
	return FALSE;
}