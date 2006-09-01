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
	case IMN_CLOSESTATUSWINDOW:
		UIHideStatusWindow();
		break;
		
	case IMN_OPENSTATUSWINDOW:
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
		
	case IMN_OPENCANDIDATE:
		dsvr->showCandi(true);
		break;
		
	case IMN_CHANGECANDIDATE:
		break;
		
	case IMN_CLOSECANDIDATE:
		dsvr->showCandi(false);
		break;
		
	case IMN_SETCONVERSIONMODE:
		{
			DWORD conv, sentence;
			ImmGetConversionStatus( ImmGetContext(hWnd), &conv, &sentence);
			isChinese = (conv & IME_CMODE_NATIVE);
			isFullShape = (conv & IME_CMODE_FULLSHAPE);
		}
		break;

	case IMN_SETSENTENCEMODE:
		break;
		
	case IMN_SETOPENSTATUS: // toggle status open or close
		UIShowStatusWindow();
		break;
		
	case IMN_SETCANDIDATEPOS:
		{
			POINT ptSrc;
			SIZE szOffset;
			HDC hDC;

			ptSrc = lpIMC->cfCandForm[0].ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
			ReleaseDC(lpIMC->hWnd,hDC);
			LPMYPRIVATE lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
			UIMoveCandWindow(lpIMC->hWnd, ptSrc.x +szOffset.cx, ptSrc.y + szOffset.cy, lpMyPrivate->CandStr);
			ImmUnlockIMCC(lpIMC->hPrivate);
		}
		break;
		
	case IMN_SETCOMPOSITIONFONT:
		break;
		
	case IMN_SETCOMPOSITIONWINDOW:
		{
			POINT ptSrc;
			SIZE szOffset;
			HDC hDC;

			ptSrc = lpIMC->cfCompForm.ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
			ReleaseDC(lpIMC->hWnd,hDC);

			CompX = ptSrc.x + szOffset.cx;
			CompY = ptSrc.y + szOffset.cy;

			//dsvr.setBufPos(CompX, CompY)->notify();
		}
		DebugLog("IMN_SETCOMPOSITIONWINDOW x->%d y->%d", CompX, CompY);
		/*
		if (IsWindow(lpUIExtra->uiComp.hWnd))
			InvalidateRect(lpUIExtra->uiComp.hWnd,NULL,FALSE);
		*/

		break;
		
	case IMN_GUIDELINE:
		break;
		
	case IMN_SETSTATUSWINDOWPOS:
//		MoveStatusWindow(hWnd, );
		break;
		
	case IMN_PRIVATE:
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
