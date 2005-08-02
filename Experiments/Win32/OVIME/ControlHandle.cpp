#include "OVIME.h"

LONG ControlHandle(HIMC hUICurIMC,
				   HWND hWnd,
				   UINT msg,
				   WPARAM wParam,
				   LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONFORM lpCompForm;

    if (!(lpIMC = ImmLockIMC(hUICurIMC)))
        return 0L;

	switch(wParam)
	{
	case IMC_SETCOMPOSITIONWINDOW:
		if(lParam)
		{
			lpCompForm = (LPCOMPOSITIONFORM) lParam;
			DebugLog("SetCompWindow");//, (void*)lpCompForm->ptCurrentPos.x);
			CompX = lpCompForm->ptCurrentPos.x;
			CompY = lpCompForm->ptCurrentPos.y;
		}
		break;
	default:
		lRet = 0L;
		break;
	}
    ImmUnlockIMC(hUICurIMC);
	return lRet;
}
