#include "OVIME.h"

#include <vector>
#include <string>
#include "OVStringToolKit.h"
using namespace std;

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
	*(lpdwTemp++) = wParam;
	*(lpdwTemp++) = lParam;

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

//	UINT uNumTranKey;
	LPMYPRIVATE lpMyPrivate;
	BOOL RetVal = TRUE;
	BOOL fOpen;
	int spec;
	static BOOL fPressOther = FALSE;
	static BOOL fFirst = TRUE;
	int k;
	char str[1024];
	int rlen;
	char result[100][1024];
	if ((lKeyData & 0x80000000) && uVKey != VK_CONTROL)
		return FALSE;

	if (!(lKeyData & 0x80000000) && uVKey == VK_CONTROL)
		return FALSE;

	if (lpbKeyState[VK_MENU] & 0x80 ) return FALSE;

	if(uVKey != VK_CONTROL && lpbKeyState[VK_CONTROL] & 0x80 ) {
		fPressOther = TRUE;
		return FALSE;
	}
		
	if (!hIMC)
		return 0;

	if (!(lpIMC = ImmLockIMC(hIMC)))
		return 0;
	fOpen = lpIMC->fOpen;

	if(uVKey == VK_CONTROL && (lKeyData & 0x80000000) && !fPressOther && !fFirst){
		fPressOther = FALSE;

		if(fOpen) {
			lpIMC->fOpen=FALSE;
			//MakeResultString(hIMC,FALSE);
		}
		else lpIMC->fOpen=TRUE;

		MyGenerateMessage(hIMC, WM_IME_NOTIFY, IMN_SETOPENSTATUS, 0);
		return FALSE;
	}

	fPressOther = FALSE;

	if(fFirst) fFirst = FALSE;

	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);

	if (_tcslen(GETLPCOMPSTR(lpCompStr)) == 0)
		MyGenerateMessage(hIMC, WM_IME_STARTCOMPOSITION, 0, 0);
	k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;
	WORD out[2];
	spec = ToAscii(uVKey, MapVirtualKey(uVKey, 0), lpbKeyState, (LPWORD)&out, 0);
	DebugLog("KEY: %c\n", out[0]);
	switch(LOWORD(uVKey))
	{
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
		DebugLog("uVKey: %x, %c\n", LOWORD(uVKey), LOWORD(uVKey));
	}
	
	memset(str, 0, 1024);
	if(spec == 1)
		k = (char)out[0];
	rlen = KeyEvent(UICurrentInputMethod(), k, str);
	int n = 0;
	int ln = 0;
	DebugLog("str: %s", str);
	string command(str);
	vector<string> commandVector;
	vector<string> delimiterVector;
	delimiterVector.push_back(" ");
	OVStringToolKit::splitString(command, commandVector, delimiterVector, false);

	LPTSTR decoded = NULL;
	_tcscpy(lpMyPrivate->CandStr, _T(""));
	_tcscpy(lpMyPrivate->PreEditStr, _T(""));
	for(vector<string>::iterator j = commandVector.begin();
		j != commandVector.end();
		++j)
	{
		const char* x = j->c_str();
		DebugLog("X: %s", x);
		if(!strcmp(x, "bufclear"))
		{
			_tcscpy(lpMyPrivate->PreEditStr, _T(""));
			MakeCompStr(lpMyPrivate, lpCompStr);
		}
		else if(!strcmp(x, "bufupdate"))
		{
			j++;
			if(!strcmp(const_cast<char *>(j->c_str()), "cursorpos")) {
				j--;
				decoded = _T("");
			} else {
				decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));
			}

			_tcscpy(lpMyPrivate->PreEditStr, decoded);
			MakeCompStr(lpMyPrivate, lpCompStr);
			free(decoded);
		}
		else if(!strcmp(x, "cursorpos"))
		{
			j++;
			UISetCursorPos(atoi(const_cast<char*>(j->c_str())));
			DebugLog("Pos %d", atoi(const_cast<char*>(j->c_str())));
		}
		else if(!strcmp(x, "markfrom"))
		{
			j++;
			UISetMarkFrom(atoi(const_cast<char*>(j->c_str())));
		}
		else if(!strcmp(x, "markto"))
		{
			j++;
			UISetMarkTo(atoi(const_cast<char*>(j->c_str())));
		}
		else if(!strcmp(x, "bufsend"))
		{
			j++;
			decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));

			_tcscpy(GETLPRESULTSTR(lpCompStr),decoded);
			lpCompStr->dwResultStrLen = _tcslen(decoded);
			_tcscpy(lpMyPrivate->PreEditStr, _T(""));
			MakeCompStr(lpMyPrivate, lpCompStr);
			
			DebugLogW(_T("decoded: %s"), decoded);
			MyGenerateMessage(hIMC,
				WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
			MyGenerateMessage(hIMC,
				WM_IME_ENDCOMPOSITION, 0, 0);
			free(decoded);
		}
		else if(!strcmp(x, "candiclear"))
		{
			ClearCandidate((LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo));
			ImmUnlockIMCC(lpIMC->hCandInfo);
		}
		else if(!strcmp(x, "candiupdate"))
		{
			j++;
			decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));
			_tcscpy(lpMyPrivate->CandStr,decoded);
			UpdateCandidate(lpIMC, decoded);
			MyGenerateMessage(hIMC,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
			free(decoded);
		}
		else if(!strcmp(x, "candishow"))
		{
		}
		else if(!strcmp(x, "candihide"))
		{
			UIHideCandWindow();
		}
		else if(!strcmp(x, "unprocessed"))
		{
			RetVal = FALSE;
		}
		else if(!strcmp(x, "processed"))
		{
			RetVal = TRUE;
			MyGenerateMessage(hIMC,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
		}
	}

	ImmUnlockIMCC(lpIMC->hPrivate);
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMC(hIMC);

	return RetVal; 
}

BOOL APIENTRY 
ImeSelect(HIMC hIMC, BOOL fSelect)
{
    LPINPUTCONTEXT lpIMC;
    
    if (!hIMC)
        return TRUE;

	if (!fSelect)
		return TRUE;

    if (lpIMC = ImmLockIMC(hIMC)) {		
		LPCOMPOSITIONSTRING lpCompStr;
		LPCANDIDATEINFO lpCandInfo;
		LPMYPRIVATE lpMyPrivate;
		
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
#if 0
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;

	UINT uNumTranKey;
	LPMYPRIVATE lpMyPrivate;

	int k;
	char str[1024];
	int rlen;
	char result[100][1024];
		
	if (!hIMC)
		return 0;

	if (!(lpIMC = ImmLockIMC(hIMC)))
		return 0;

	// 暫時不理會 KeyUP
	if (uScanCode & 0x8000)
		return 0;

	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);

	uNumTranKey = 0;

	if (_tcslen(GETLPCOMPSTR(lpCompStr)) == 0)
		MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
			WM_IME_STARTCOMPOSITION, 0, 0);
	k = LOWORD(uVKey);
	if( k >= 65 && k <= 90)
		k = k + 32;
	switch(LOWORD(uVKey))
	{
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
	case 189: // -
		k = 45;
		break;
	case 0xba: // ;
		k = ';';
		break;
	case 0xc0: // `
		k = '`';
		break;
	case 0xbc: // ,
		k = ',';
		break;
	case 0xbe: // .
		k = '.';
		break;
	case 0xbf: // /
		k = '/';
		break;
	case 0xdc:
		k = '\\';
		break;
	case 0xde: // '
		k = '\'';
		break;
	case 0xdb: // [
		k = '[';
		break;
	case 0xdd: // ]
		k = ']';
		break;
	case 0xbb: // =
		k = '=';
		break;
	case 'Q':
		k = 'q';
	default:
		DebugLog("uVKey: %x\n", (void*)LOWORD(uVKey));
	}
	
	memset(str, 0, 1024);
	rlen = KeyEvent(0, tolower(k), str);
	int n = 0;
	int ln = 0;
	DebugLog("str: %s", str);
	string command(str);
	vector<string> commandVector;
	vector<string> delimiterVector;
	delimiterVector.push_back(" ");
	OVStringToolKit::splitString(command, commandVector, delimiterVector, false);
	/*
	for( int i = 0; i < rlen; i++ )
	{
		if(str[i] == ' ' || i == rlen - 1)
		{
			int dst = i - 1;
			if( i == rlen - 1)
				dst = i + 1;
			memset(result[n], 0, 100);
			strncpy(result[n], str+ln, dst - ln + 1);
			ln = i + 1;
			n++;
		}
	}
	*/

	LPTSTR decoded = NULL;
	_tcscpy(lpMyPrivate->CandStr, _T(""));
	_tcscpy(lpMyPrivate->PreEditStr, _T(""));
	//for( int j = 0; j < n; j++ )
	for(vector<string>::iterator j = commandVector.begin();
		j != commandVector.end();
		++j)
	{
		//char *x = result[j];
		const char* x = j->c_str();
		DebugLog("X: %s", x);
		if(!strcmp(x, "bufclear"))
		{
			_tcscpy(lpMyPrivate->PreEditStr, _T(""));
			MakeCompStr(lpMyPrivate, lpCompStr);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
		}
		else if(!strcmp(x, "bufupdate"))
		{
			j++;
			//decoded = UTF16toWCHAR(result[j]);
			decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));

			_tcscpy(lpMyPrivate->PreEditStr, decoded);
			MakeCompStr(lpMyPrivate, lpCompStr);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
			free(decoded);
		}
		else if(!strcmp(x, "bufsend"))
		{
			j++;
			//decoded = UTF16toWCHAR(result[j]);
			decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));

			_tcscpy(GETLPRESULTSTR(lpCompStr),decoded);
			lpCompStr->dwResultStrLen = _tcslen(decoded);
			_tcscpy(lpMyPrivate->PreEditStr, _T(""));
			MakeCompStr(lpMyPrivate, lpCompStr);
			
			DebugLogW(_T("decoded: %s"), decoded);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_ENDCOMPOSITION, 0, 0);
			free(decoded);
		}
		else if(!strcmp(x, "candiclear"))
		{
			ClearCandidate((LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo));
			ImmUnlockIMCC(lpIMC->hCandInfo);
		}
		else if(!strcmp(x, "candiupdate"))
		{
			j++;
			//decoded = UTF16toWCHAR(result[j]);
			decoded = UTF16toWCHAR(const_cast<char *>(j->c_str()));
			_tcscpy(lpMyPrivate->CandStr,decoded);
			UpdateCandidate(lpIMC, decoded);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_COMPOSITION, 0, GCS_COMPSTR);
			free(decoded);
		}
		else if(!strcmp(x, "candishow"))
		{
		}
		else if(!strcmp(x, "candihide"))
		{
			UIHideCandWindow();
		}
		else if(!strcmp(x, "unprocessed"))
		{
			LPTSTR s = _tcsdup(_T("ORZ"));
			_stprintf(s, _T("%c"), k);
			_tcscpy(GETLPRESULTSTR(lpCompStr), s);
			lpCompStr->dwResultStrLen = _tcslen(s);
			
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
			MyGenerateMessageToTransKey(lpdwTransKey, &uNumTranKey,
				WM_IME_ENDCOMPOSITION, 0, 0);
			free(s);
		}
		else if(!strcmp(x, "processed"))
		{

		}
	}

	ImmUnlockIMCC(lpIMC->hPrivate);
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMC(hIMC);
	
	return uNumTranKey;
#endif
	return 0;
}

BOOL APIENTRY
NotifyIME(HIMC hIMC,DWORD dwAction,DWORD dwIndex,DWORD dwValue)
{
    BOOL bRet = FALSE;
	LPINPUTCONTEXT lpIMC;
	
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
	case NI_CONTEXTUPDATED:
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
