#define OV_DEBUG
#include "OVIME.h"
#include <cstdio>

void MakeCompStr(LPMYPRIVATE lpMyPrivate, LPCOMPOSITIONSTRING lpCompStr)
{
	wcscpy(GETLPCOMPSTR(lpCompStr), L"");
	swprintf(GETLPCOMPSTR(lpCompStr), lpMyPrivate->PreEditStr);
	lpCompStr->dwCompStrLen = (DWORD)wcslen(GETLPCOMPSTR(lpCompStr));
}

void InitCompStr(LPCOMPOSITIONSTRING lpCompStr)
{
	lpCompStr->dwSize = sizeof(MYCOMPSTR);
	lpCompStr->dwCompStrOffset =
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szCompStr - (DWORD)lpCompStr;
	lpCompStr->dwCompStrLen = 0;
	*GETLPCOMPSTR(lpCompStr) = L'\0';

	lpCompStr->dwResultStrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szResultStr - (DWORD)lpCompStr;
	lpCompStr->dwResultStrLen = 0;
	*GETLPRESULTSTR(lpCompStr) = L'\0';
	
	lpCompStr->dwCursorPos = 0;
}

void InitCandInfo(LPCANDIDATEINFO lpCandInfo)
{
	LPCANDIDATELIST lpCandList;
	
	int i;

	lpCandInfo->dwSize = sizeof(MYCAND);
	lpCandInfo->dwCount = 1;
	lpCandInfo->dwOffset[0] = 
			(DWORD)(&(((LPMYCAND)lpCandInfo)->cl)) - (DWORD)lpCandInfo;

	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	for(i = 0; i < MAXCANDSTRNUM; i++)
	{
		lpCandList->dwOffset[i] = 
                   (DWORD)((LPMYCAND)lpCandInfo)->szCandStr[i] - (DWORD)lpCandList;
	}
	lpCandList->dwSize = sizeof(MYCAND) - sizeof(CANDIDATEINFO); 
	lpCandList->dwCount = 0L;
	lpCandList->dwSelection = 0L;
	lpCandList->dwPageStart = 0L;
	lpCandList->dwPageSize = 0L;
}

void ClearCandidate(LPCANDIDATEINFO lpCandInfo)
{
    ((LPMYCAND)lpCandInfo)->cl.dwCount =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwSelection =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageStart =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageSize =0L;
}

void UpdateCandidate(LPINPUTCONTEXT lpIMC, const wchar_t* candis)
{
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
	{
		wchar_t* lpStr;
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo
			+ lpCandInfo->dwOffset[0]);
		int rlen = (int)wcslen(candis);
		int n = 0;
		int ln = 0;
		for( int i = 0; i < rlen; i++ )
		{
			if(candis[i] == L' ' || i == rlen - 1)
			{				
				int dst = i - 1;
				if( i == rlen - 1)
					dst = i + 1;
				lpStr = GETLPCANDSTR(lpCandList, n);
				memset(lpStr, 0, 100*sizeof(wchar_t));
				wcsncpy(lpStr, candis+ln, dst - ln + 1);
				ln = i + 1;
				n++;
			}
		}
		lpCandList->dwCount = n - 2;
		lpCandList->dwPageSize = 11;
		lpCandList->dwPageStart = 0;

		ImmUnlockIMCC(lpIMC->hCandInfo);
	}
}


void RefreshUI(HWND hWnd )  //½Õ¾ã comp/cand position
{		
	HIMC			hUICurIMC;
	LPINPUTCONTEXT	lpIMC;
	LONG			lRet = 0L;
	LPMYPRIVATE lpMyPrivate;
	hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);

	lpIMC = ImmLockIMC(hUICurIMC);
	POINT pt;
	if(CompX < 0) 
	{
		murmur("\tCompX<0");
		LONG lRet = 0L;

		static BOOL first = false;
		LOGFONT* lfptr;
		LOGFONT lf2;
		RECT rec;
		
		POINT ptSrc;
		
		int localDPIY; //for device dpiY
		ptSrc = lpIMC->cfCompForm.ptCurrentPos;
		ClientToScreen(lpIMC->hWnd, &ptSrc);

		
		murmur("\thWnd->%x", lpIMC->hWnd);

		//computes the width and height of the specified string of text.
		HDC hDC; //Handle to the device context.
		hDC = GetDC(lpIMC->hWnd);
		SIZE szOffset;//Pointer to a SIZE structure that receives the dimensions of the string, in logical units.		
		GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);

		//fills the specified buffer with the metrics for the currently selected font.
		TEXTMETRIC tm; //Pointer to the TEXTMETRIC structure that receives the text metrics.
		GetTextMetrics(hDC, &tm);

		localDPIY = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC(lpIMC->hWnd,hDC);

		lfptr = (LOGFONT*)(&lpIMC->lfFont);
		memcpy( &lf2, lfptr, sizeof( lf2) );					
		CompX = ptSrc.x ;
		CompY = ptSrc.y + abs(lf2.lfHeight)*localDPIY/tm.tmDigitizedAspectY;			
		//int tmpY=abs(lf2.lfHeight)*localDPIY/tm.tmDigitizedAspectY;


		lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);		

		if(dsvr->isCompEnabled) 
		{
			dsvr->moveBuf(CompX,CompY);	
			if(dsvr->isCandiEnabled)
				dsvr->moveCandi(CompX+UIGetCaretPosX(),CompY+UIGetHeight());	
		}
		else
		{
			if(dsvr->isCandiEnabled)
				dsvr->moveCandi(CompX,CompY);
		}
	}	
	return;
}


