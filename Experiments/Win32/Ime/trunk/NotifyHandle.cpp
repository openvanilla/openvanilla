#include "OVIME.h"

LONG NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;
    HGLOBAL hUIExtra;
    LPUIEXTRA lpUIExtra;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

    hUIExtra = (HGLOBAL)GetWindowLong(hWnd, IMMGWL_PRIVATE);
    lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

    switch (wParam)
    {
	case IMN_CLOSESTATUSWINDOW:
		break;
		
	case IMN_OPENSTATUSWINDOW:
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
			HFONT oldFont;

			ptSrc = lpIMC->cfCompForm.ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			hDC = GetDC(lpIMC->hWnd);
			oldFont = (HFONT)SelectObject(hDC, hUIFont);
			GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
			SelectObject(hDC, oldFont);
			ReleaseDC(lpIMC->hWnd,hDC);

			lpUIExtra->uiComp.pt.x = ptSrc.x + szOffset.cx;
			lpUIExtra->uiComp.pt.y = ptSrc.y + szOffset.cy;
		}
	    if (IsWindow(lpUIExtra->uiComp.hWnd))
			InvalidateRect(lpUIExtra->uiComp.hWnd,NULL,FALSE);

		break;
		
	case IMN_GUIDELINE:
		break;
		
	case IMN_SETSTATUSWINDOWPOS:
		break;
		
	case IMN_PRIVATE:
		break;
		
	default:
		break;
    }
    GlobalUnlock(hUIExtra);
    ImmUnlockIMC(hUICurIMC);

    return lRet;
}