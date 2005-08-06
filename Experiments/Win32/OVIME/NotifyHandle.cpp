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
		break;
		
	case IMN_CHANGECANDIDATE:
		break;
		
	case IMN_CLOSECANDIDATE:
		break;
		
	case IMN_SETCONVERSIONMODE:
		break;
		
	case IMN_SETSENTENCEMODE:
		break;
		
	case IMN_SETOPENSTATUS:
		UIShowStatusWindow();
		break;
		
	case IMN_SETCANDIDATEPOS:
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
