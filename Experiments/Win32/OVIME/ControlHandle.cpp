#include "OVIME.h"

LONG ControlHandle(HIMC hUICurIMC,
				   HWND hWnd,
				   UINT msg,
				   WPARAM wParam,
				   LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;
    HGLOBAL hUIExtra;
    LPUIEXTRA lpUIExtra;
	LPCOMPOSITIONFORM lpCompForm;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

    hUIExtra = (HGLOBAL)GetWindowLong(hWnd, IMMGWL_PRIVATE);
    lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
	switch(wParam)
	{
	case IMC_SETCOMPOSITIONWINDOW:
		if(lParam)
		{
			lpCompForm = (LPCOMPOSITIONFORM) lParam;
			DebugLog("SetCompWindow\n", NULL);//, (void*)lpCompForm->ptCurrentPos.x);
			lpUIExtra->uiComp.pt.x = lpCompForm->ptCurrentPos.x;
			lpUIExtra->uiComp.pt.y = lpCompForm->ptCurrentPos.y;
		}
		break;
	default:
		lRet = 0L;
		break;
	}
    GlobalUnlock(hUIExtra);
    ImmUnlockIMC(hUICurIMC);
	return lRet;
}