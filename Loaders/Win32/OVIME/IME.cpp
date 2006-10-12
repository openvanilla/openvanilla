#include "OVIME.h"
#include "AVKeyCode.h"
#include "ExtraStructs.h"
#include <commctrl.h>



#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

long g_shiftPressedTime = -1;

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

void
MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LPINPUTCONTEXT lpIMC;

	if((lpIMC = ImmLockIMC(hIMC)) == NULL)
		return;

    if (IsWindow(lpIMC->hWnd))
    {
        LPTRANSMSG lpTransMsg;
        if (!(lpIMC->hMsgBuf = ImmReSizeIMCC(lpIMC->hMsgBuf,
			(lpIMC->dwNumMsgBuf + 1) * sizeof(TRANSMSG))))
            return;
		
        if (!(lpTransMsg = (LPTRANSMSG)ImmLockIMCC(lpIMC->hMsgBuf)))
            return;
		
        lpTransMsg += (lpIMC->dwNumMsgBuf);
        //lpTransMsg->uMsg = msg;
		lpTransMsg->uMsg = msg;
		lpTransMsg->wParam=wParam;
		lpTransMsg->lParam=lParam;

        ImmUnlockIMCC(lpIMC->hMsgBuf);
        lpIMC->dwNumMsgBuf++;
		
        ImmGenerateMessage(hIMC);
    }
	ImmUnlockIMC(hIMC);
}

BOOL
MyGenerateMessageToTransKey(LPDWORD lpdwTransKey, UINT *uNumTranMsgs,
							UINT msg, WPARAM wParam, LPARAM lParam) 
{
	// This code is from FreePY. 
	// It seems that the first byte is the number of bytes.
	LPDWORD lpdwTemp;
	
	if (((*uNumTranMsgs) + 1) >= (UINT)*lpdwTransKey)
        return FALSE;
	
	lpdwTemp = (LPDWORD)lpdwTransKey+1+(*uNumTranMsgs)*3;
	*(lpdwTemp++) = msg;
	*(lpdwTemp++) = (DWORD)wParam;
	*(lpdwTemp++) = (DWORD)lParam;

	(*uNumTranMsgs)++;

    return TRUE;
}

BOOL APIENTRY 
ImeInquire(LPIMEINFO lpIMEInfo, LPTSTR lpszUIClass, LPCTSTR lpszOption)
{
    lpIMEInfo->dwPrivateDataSize = sizeof(MYPRIVATE);

    lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST | IME_PROP_UNICODE;
                             //IME_PROP_SPECIAL_UI;
							 //IME_PROP_UNICODE

    lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass, UICLASSNAME);

    return TRUE;
}

BOOL APIENTRY 
ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
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

DWORD APIENTRY 
ImeConversionList(HIMC hIMC,LPCTSTR lpSource,LPCANDIDATELIST lpCandList,DWORD dwBufLen,UINT uFlag)
{
    return 0;
}

BOOL APIENTRY 
ImeDestroy(UINT uForce)
{
    return FALSE;
}

LRESULT APIENTRY 
ImeEscape(HIMC hIMC,UINT uSubFunc,LPVOID lpData)
{
	return FALSE;
}

BOOL APIENTRY 
ImeProcessKey(HIMC hIMC, UINT uVKey, LPARAM lKeyData, CONST LPBYTE lpbKeyState)
{
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	
	int spec;
	int k;

	if (!hIMC) return FALSE;
	if (!(lpIMC = ImmLockIMC(hIMC))) 
		return FALSE;
	//if (lKeyData & 0x80000000)	
	//	return FALSE;

	//if( (uVKey == VK_SHIFT)&&((GetKeyState(uVKey)& 0xF0) != 0xF0) )	
	if ( GetKeyInfo(lKeyData).isKeyUp )
	{	// Key up      
		if( g_shiftPressedTime > 0 )	
		{
        	DWORD time = (GetTickCount() - g_shiftPressedTime);
			if( uVKey == VK_SHIFT &&  time <= 20000 )
            {
				// Toggle Chinese/English mode.
				//Only Shift: lParam == 2
				MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);
				//return TRUE;
            }
            g_shiftPressedTime = -1;
        }         
        return FALSE;
    }

	if( uVKey == VK_SHIFT  )	
	{
    	//if( ! IsKeyDown( lpbKeyState[VK_CONTROL] ) && g_shiftPressedTime < 0 )
		if( g_shiftPressedTime < 0 )
		{
			g_shiftPressedTime = GetTickCount();
		}
    }
    else if( g_shiftPressedTime > 0 )
	{
		g_shiftPressedTime = -1;
	}

	if (/*uVKey == VK_SHIFT ||*/ uVKey == VK_CONTROL || uVKey == VK_MENU) 
		return FALSE;
	
	if(LOWORD(uVKey) == VK_SPACE && (lpbKeyState[VK_CONTROL] & 0x80))
		return TRUE;  //ctrl+space

    if(LOWORD(uVKey) == VK_SPACE && IsKeyDown(lpbKeyState[VK_SHIFT]))
	{
		//shift+space: lParam == 1
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 1);	
		return FALSE;  //shift+space
	}

	//Change the module by Ctrl+"\"
	if(LOWORD(uVKey) == VK_OEM_5 && ((lpbKeyState[VK_CONTROL] & 0x80)))
		return TRUE;

	//Change CHI/ENG by CAPS
	//if(LOWORD(uVkey) == VK_CAPS)
	//	return TRUE;

	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	if(wcslen(GETLPCOMPSTR(lpCompStr)) == 0)
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);
	
	k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;
	WORD out[2];
	spec = ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	murmur("KEY: %c\n", out[0]);
	switch(LOWORD(uVKey))	{
	case VK_PRIOR: // pageup
		k = 11;
		break;
	case VK_NEXT: // pagedown
		k = 12;
		break;
	case VK_END:
		k = 4;
		break;
	case VK_HOME:
		k = 1;
		break;
	case VK_LEFT:
		k = 28;
		break;
	case VK_UP:
		k = 30;
		break;
	case VK_RIGHT:
		k = 29;
		break;
	case VK_DOWN:
		k = 31;
		break;
	case VK_DELETE:
		k = 127;
		break;
	default:
		//DebugLog("uVKey: %x, %c\n", LOWORD(uVKey), LOWORD(uVKey));
		break;
	}
	
	if(spec == 1)
		k = (char)out[0];

	AVKeyCode keycode(k);
	DWORD conv, sentence;
	ImmGetConversionStatus( hIMC, &conv, &sentence);

	if( !(conv & IME_CMODE_NATIVE) )	// AlphaNumaric mode
		keycode.setCapslock(1);
	if(LOWORD(lpbKeyState[VK_CAPITAL]))
		keycode.setCapslock(1);
	if(lpbKeyState[VK_SHIFT] & 0x80)
		keycode.setShift(1);
	if(lpbKeyState[VK_CONTROL] & 0x80)
		keycode.setCtrl(1);
	if(lpbKeyState[VK_MENU] & 0x80)
		keycode.setAlt(1);
	if((lpbKeyState[VK_NUMLOCK])
		&& (uVKey >= VK_NUMPAD0)
		&& (uVKey <= VK_DIVIDE))
		keycode.setNum(1);


	dsvr->lockIMC(hIMC);
	loader = AVLoader::getLoader();
	BOOL retVal = FALSE;
	if(loader->keyEvent(UICurrentInputMethod(), keycode)) {
		retVal = TRUE;
		MyGenerateMessage(hIMC,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
		
	} else {
		retVal = FALSE;
		MyGenerateMessage(hIMC,
				WM_IME_ENDCOMPOSITION, 0, 0);
	}
	dsvr->releaseIMC();
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMC(hIMC);

	return retVal; 
}

BOOL APIENTRY 
ImeSelect(HIMC hIMC, BOOL fSelect)
{
    LPINPUTCONTEXT lpIMC;

    if (!hIMC)
        return TRUE;

	if (!fSelect)
		return TRUE;

	DWORD conv, sentence;
	ImmGetConversionStatus( hIMC, &conv, &sentence);
	if( isChinese )
		conv |= IME_CMODE_NATIVE;
	else
		conv &= ~IME_CMODE_NATIVE;
	ImmSetConversionStatus( hIMC, conv, sentence);

    if (lpIMC = ImmLockIMC(hIMC)) {		
		LPCOMPOSITIONSTRING lpCompStr;
		LPCANDIDATEINFO lpCandInfo;
		LPMYPRIVATE lpMyPrivate;

		lpIMC->fOpen = TRUE;

		// Resize of compsiting string of IMC
		lpIMC->hCompStr = ImmReSizeIMCC(lpIMC->hCompStr,sizeof(MYCOMPSTR));
		if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
		{
			InitCompStr(lpCompStr);
			ImmUnlockIMCC(lpIMC->hCompStr);
		}
		lpIMC->hCandInfo = ImmReSizeIMCC(lpIMC->hCandInfo,sizeof(MYCAND));
		if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
		{
			InitCandInfo(lpCandInfo);
			ImmUnlockIMCC(lpIMC->hCandInfo);
		}
		
		if ((lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate)) != NULL) {
			for(int i = 0; i < MAXSTRSIZE; i++)
				lpMyPrivate->PreEditStr[i]='\0';
			for(int i = 0; i < MAXSTRSIZE; i++)
				lpMyPrivate->CandStr[i]='\0';
			ImmUnlockIMCC(lpIMC->hPrivate);	
		}
		ImmUnlockIMC(hIMC);
    }
	return TRUE;
}

BOOL APIENTRY 
ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
    return TRUE;
}

UINT APIENTRY 
ImeToAsciiEx (UINT uVKey, UINT uScanCode,
			  CONST LPBYTE lpbKeyState,
			  LPDWORD lpdwTransKey, UINT fuState,HIMC hIMC)
{
	//Change the module by Ctrl+"\": lParam == 0
	if(LOWORD(uVKey) == VK_OEM_5 && ((lpbKeyState[VK_CONTROL] & 0x80)))
		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 0);

	//Change CHI/END by CAPS
	//if(LOWORD(uVKey) == VK_CAPS)
	//	MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_PRIVATE, 2);

	return 0;
}

BOOL APIENTRY
NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
    BOOL bRet = FALSE;
	//LPINPUTCONTEXT lpIMC;
	
    switch(dwAction)
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
/*	case NI_CONTEXTUPDATED:
		switch (dwValue)
		{
		case IMC_SETCOMPOSITIONWINDOW:
			break;
		case IMC_SETCONVERSIONMODE:
			break;
		case IMC_SETSENTENCEMODE:
			break;
		case IMC_SETCANDIDATEPOS:
			break;
		case IMC_SETCOMPOSITIONFONT:
			break;
		case IMC_SETOPENSTATUS:
			lpIMC = ImmLockIMC(hIMC);
			if (lpIMC)
			{

			}

			bRet = TRUE;
			break;
		default:
			break;
		}
		break;
*/		
	case NI_COMPOSITIONSTR:
		switch (dwIndex)
		{
		case CPS_COMPLETE:
			break;
		case CPS_CONVERT:
			break;
		case CPS_REVERT:
			break;
		case CPS_CANCEL:
			break;
		default:
			break;
		}
		break;
			
	default:
		break;
    }
    return bRet;
}

BOOL APIENTRY
ImeRegisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
    return FALSE;
}

BOOL APIENTRY
ImeUnregisterWord(LPCTSTR lpRead, DWORD dw, LPCTSTR lpStr)
{
    return FALSE;
}

UINT APIENTRY
ImeGetRegisterWordStyle(UINT nItem, LPSTYLEBUF lp)
{
	return 0;
}

UINT APIENTRY
ImeEnumRegisterWord(REGISTERWORDENUMPROC lpfn,
					LPCTSTR lpRead, DWORD dw,
					LPCTSTR lpStr, LPVOID lpData)
{
	return 0;
}

BOOL APIENTRY
ImeSetCompositionString(HIMC hIMC, DWORD dwIndex,
						LPCVOID lpComp, DWORD dwComp,
						LPCVOID lpRead, DWORD dwRead)
{
    return FALSE;
}
