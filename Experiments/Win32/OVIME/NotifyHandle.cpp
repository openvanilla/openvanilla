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
		break;
		
	case IMN_OPENSTATUSWINDOW:
		UICreateStatusWindow(hWnd);
		UIMoveStatusWindow(hWnd, 500, 100);
		break;
		
	case IMN_OPENCANDIDATE:
		UIShowCandWindow();
		break;
		
	case IMN_CHANGECANDIDATE:
		break;
		
	case IMN_CLOSECANDIDATE:
		UIHideCandWindow();
		break;
		
	case IMN_SETCONVERSIONMODE:
		break;
		
	case IMN_SETSENTENCEMODE:
		break;
		
	case IMN_SETOPENSTATUS:
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
