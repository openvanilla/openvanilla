#define OV_DEBUG

#include "OVIME.h"

LONG NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;
	static BOOL first = false;
	RECT rec;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

    switch (wParam)
    {
		murmur("On WM_IME_Notify");	

	case IMN_OPENSTATUSWINDOW:
		murmur("IMN_OPENSTATUSWINDOW");
		UICreateStatusWindow(hWnd);
		if(!first) {
		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&rec,
				0);
			UIMoveStatusWindow(hWnd, rec.right - 50, rec.bottom - 50);
			first = true;
		}
		UIShowStatusWindow();
		break;
		
	case IMN_CLOSESTATUSWINDOW:
		murmur("IMN_CLOSESTATUSWINDOW");
		UIHideStatusWindow();
		//<comment author='b6s'>
		// When attached app lost focus, only this message occurred,
		// so it must also hide following windows.
		UIHideCompWindow();
		UIHideCandWindow();
		UIHideNotifyWindow();
		//</comment>
		break;

	case IMN_OPENCANDIDATE:
		murmur("IMN_OPENCANDIDATE");
		//dsvr->showCandi(true);
		UIShowCandWindow();
		break;

	case IMN_CLOSECANDIDATE:
		murmur("IMN_CLOSECANDIDATE");
		//dsvr->showCandi(false);
		UIHideCandWindow();
		break;

	case IMN_CHANGECANDIDATE:
		murmur("IMN_CHANGECANDIDATE");
		break;

	case IMN_SETCONVERSIONMODE:
		{
			murmur("IMN_SETCONVERSIONMODE");
			DWORD conv, sentence;
			ImmGetConversionStatus( ImmGetContext(hWnd), &conv, &sentence);
			isChinese = (conv & IME_CMODE_NATIVE);
			isFullShape = (conv & IME_CMODE_FULLSHAPE);
		}
		break;

	case IMN_SETSENTENCEMODE:
		murmur("IMN_SETSENTENCEMODE");
		break;
		
	case IMN_SETOPENSTATUS: // toggle status open or close
		murmur("IMN_SETOPENSTATUS");
		UIShowStatusWindow();
		break;
		
	case IMN_SETCANDIDATEPOS: // set candidate position , but no move
		{
			murmur("IMN_SETCANDIDATEPOS");
			POINT ptSrc;
			SIZE szOffset;
			HDC hDC;

			ptSrc = lpIMC->cfCandForm[0].ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
			ReleaseDC(lpIMC->hWnd,hDC);
			LPMYPRIVATE lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
			//James modify
			CandX=ptSrc.x +szOffset.cx;
			CandY=ptSrc.y + szOffset.cy;
			//UIMoveCandWindow(lpIMC->hWnd, CompX+20,CompY+20, lpMyPrivate->CandStr);
			ImmUnlockIMCC(lpIMC->hPrivate);
		}
		break;
		
	case IMN_SETCOMPOSITIONFONT:
		murmur("IMN_SETCOMPOSITIONFONT");
		//<comment author='b6s'>
		// It is weird but... when the attached app got focus back,
		// this message occurred.
		//UIShowCandWindow();
		//murmur("Also try to do UIShowCandWindow()");
		//</comment>		
		break;
		
	case IMN_SETCOMPOSITIONWINDOW:    // set composition window position, move
		{
			murmur("IMN_SETCOMPOSITIONWINDOW");
			POINT ptSrc;
			SIZE szOffset;
			HDC hDC;

			ptSrc = lpIMC->cfCompForm.ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
			ReleaseDC(lpIMC->hWnd,hDC);

			//James modified
			//CompX = ptSrc.x + szOffset.cx;
			//CompY = ptSrc.y + szOffset.cy;			
			CompX = ptSrc.x ;
			CompY = ptSrc.y ;
			//set candi
			
			//dsvr.setBufPos(CompX, CompY)->notify();
		}
		
		murmur("IMN_SETCOMPOSITIONWINDOW x->%d y->%d", CompX, CompY);
		//James add
		//UIMoveCandWindow(hWnd, CompX, CompY, NULL);	//lpMyPrivate->CandStr); by b6s
		/*
		if (IsWindow(lpUIExtra->uiComp.hWnd))
			InvalidateRect(lpUIExtra->uiComp.hWnd,NULL,FALSE);
		*/
		//<comment author='b6s'>Not ready yet because of "Set Candidate String" issues.
		//UIShowCandWindow();
		//murmur("Also try to do UIShowCandWindow()");
		//</comment>

		break;
		
	case IMN_GUIDELINE:
		murmur("IMN_GUIDELINE");
		break;
		
	case IMN_SETSTATUSWINDOWPOS:
		murmur("IMN_SETSTATUSWINDOWPOS");
//		MoveStatusWindow(hWnd, );
		break;
		
	case IMN_PRIVATE:
		murmur("IMN_PRIVATE");
		if(lParam == 0)
			UIChangeModule(hWnd);
		if(lParam == 1)
			UIChangeHalfFull(hWnd); //Change UI Half/Full
		if(lParam == 2)
			UIChangeChiEng(hWnd);  //Chnage UI CHI/ENG

		break;
		
	default:
		break;
    }
    ImmUnlockIMC(hUICurIMC);

    return lRet;
}
