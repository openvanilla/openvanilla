//#define OV_DEBUG
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


void RefreshUI(HWND hWnd)  //½Õ¾ãcomp cand
{		
	HIMC hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);
	ImmModel* model = ImmModel::open(hUICurIMC);

	dsvr->setHIMC(hUICurIMC);

	LPINPUTCONTEXT lpIMC = model->getIMC();
	//lpIMC = ImmLockIMC(hUICurIMC);

	POINT ptSrc;
	if(dsvr->isCompEnabled)  // comp exist, cand follow comp's position
	{
		//murmur("isCompEnabled");
		ptSrc = lpIMC->cfCompForm.ptCurrentPos;
	}
	else if(dsvr->isCandiEnabled) // comp doesn't exist, get cand posistion
	{
		//murmur("isCandiEnabled");
		ptSrc = lpIMC->cfCandForm[0].ptCurrentPos;
	}

	ClientToScreen(lpIMC->hWnd, &ptSrc);

	//computes the width and height of the specified string of text.
	//GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);

	LOGFONT* lfptr = (LOGFONT*)(&lpIMC->lfFont);
	LOGFONT lfClone;
	memcpy(&lfClone, lfptr, sizeof(lfClone));

	//@defgroup Device/DPI specific code
	//@warning
	// All device/DPI specific code in the following block were moved into C#,
	// although they remain dirty work-around currently.
	//@{
	// Fills the specified buffer with the metrics for
	// the currently selected font.
	//HDC hDC = GetDC(lpIMC->hWnd);
	//TEXTMETRIC tm;
	//GetTextMetrics(hDC, &tm);
	//int localDpiY = GetDeviceCaps(hDC, LOGPIXELSY);
	//ReleaseDC(lpIMC->hWnd, hDC);

	//murmur("localDpiY:%i", localDpiY);
	//murmur("tmDigitizedAspectY:%i", tm.tmDigitizedAspectY);
	//float fontSize = fontHeight*(localDpiY/tm.tmDigitizedAspectY);	
	//murmur("fontSize: %f", fontSize);

	//murmur("original ptSrc(%i, %i)", ptSrc.x, ptSrc.y);
	//murmur("lfptr->lfHeight: %i", lfClone.lfHeight);
	//if(lfClone.lfHeight > 0)
	//	ptSrc.y -= lfClone.lfHeight;
	//murmur("adjusted ptSrc(%i, %i)", ptSrc.x, ptSrc.y);
	//@}

	//LPMYPRIVATE lpMyPrivate = model->getMyPrivate();
	//lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
	if (dsvr->isCompEnabled)
	{
		//@warning
		// We may have better control flow on all lfHeight, UIGetCaretPosX, and
		// UIGetHeight stuffs, later.
		CompX = ptSrc.x;
		CompY = ptSrc.y;
		murmur(
			"moveBuf(%i, %i, %i)",
			CompX, CompY, lfClone.lfHeight);
		dsvr->moveBuf(
			CompX, CompY,
			lfClone.lfHeight, lfClone.lfFaceName);
		if (dsvr->isCandiEnabled)
		{			
			CandX = CompX + UIGetCaretPosX();
			CandY = CompY;
			if(lfClone.lfHeight >= 0)
				CandY -= UIGetHeight();
			murmur(
				"moveCandi(%i, %i, %i)",
				CandX, CandY, UIGetHeight());
			dsvr->moveCandi(CandX, CandY, UIGetHeight());
		}
	}
	else if (dsvr->isCandiEnabled)
	{							
		CandX= ptSrc.x;
		CandY= ptSrc.y;
		dsvr->moveCandi(CandX, CandY, abs(lfClone.lfHeight));
	}

	ImmModel::close();
}
