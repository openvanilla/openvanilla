#include "OVIME.h"

LONG NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

    switch (wParam)
    {
	case IMN_CLOSESTATUSWINDOW:
		UIHideStatusWindow();
		dsvr.showStatus(false)->notify();
		break;
		
	case IMN_OPENSTATUSWINDOW:
		UICreateStatusWindow(hWnd);
		UIMoveStatusWindow(hWnd, 500, 100);
		dsvr.showStatus(true)->notify();
		break;
		
	case IMN_OPENCANDIDATE:
		UIShowCandWindow();
		dsvr.showCandi(true)->notify();
		break;
		
	case IMN_CHANGECANDIDATE:
		break;
		
	case IMN_CLOSECANDIDATE:
		UIHideCandWindow();
		dsvr.showCandi(false)->notify();
		break;
		
	case IMN_SETCONVERSIONMODE:
		break;
		
	case IMN_SETSENTENCEMODE:
		break;
		
	case IMN_SETOPENSTATUS:
		UIShowStatusWindow();
		dsvr.showStatus(false)->notify();
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

			dsvr.setCandiPos(ptSrc.x +szOffset.cx, ptSrc.y + szOffset.cy)->notify();
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

			dsvr.setBufPos(CompX, CompY)->notify();
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
		break;
		
	default:
		break;
    }
    ImmUnlockIMC(hUICurIMC);

    return lRet;
}
