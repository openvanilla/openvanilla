#include "OVIME.h"

LRESULT APIENTRY UIWndProc(HWND hWnd,
						   UINT msg,
						   WPARAM wParam,
						   LPARAM lParam)
{
	HIMC			hUICurIMC;
	LPINPUTCONTEXT	lpIMC;
	LONG			lRet = 0L;
	LPMYPRIVATE lpMyPrivate;

	hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);

	//
	// Even if there is no current UI. these messages should not be pass to 
	// DefWindowProc().
	//
	if(!hUICurIMC)
	{        
		switch (msg)
		{
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_NOTIFY:
		case WM_IME_CONTROL:
		case WM_IME_COMPOSITIONFULL:
		case WM_IME_SELECT:
		case WM_IME_CHAR:
			return 0L;
		default:
			break;
		}
	}

	switch (msg)
	{
	case WM_CREATE:
		CompX = CompY = -1;
		CreateCompWindow(hWnd);
		CreateCandWindow(hWnd);
		break;
		
	case WM_IME_SETCONTEXT:
		if (wParam)
		{
			if (hUICurIMC)
			{
				lpIMC = ImmLockIMC(hUICurIMC);
				if (lpIMC)
				{
					lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
					MoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
					MoveCandWindow(hWnd, lpMyPrivate->CandStr);
					ImmUnlockIMCC(lpIMC->hPrivate);
				}
				else
				{
					HideCandWindow();
					HideCompWindow();
				}
				ImmUnlockIMC(hUICurIMC);
			}
			else   // it is NULL input context.
			{
				HideCandWindow();
				HideCompWindow();
			}
		}
		break;
		
	case WM_IME_STARTCOMPOSITION:
		break;
		
	case WM_IME_COMPOSITION:
		lpIMC = ImmLockIMC(hUICurIMC);
		POINT pt;
		if(CompX < 0) {
			pt.x = 15;
			pt.y = 15;
			ClientToScreen(lpIMC->hWnd, &pt);
			CompX = pt.x;
			CompY = pt.y;
		}
		lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
		MoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
		MoveCandWindow(hWnd, lpMyPrivate->CandStr);
		ImmUnlockIMCC(lpIMC->hPrivate);
		ImmUnlockIMC(hUICurIMC);
		break;
		
	case WM_IME_ENDCOMPOSITION:
		HideCompWindow();
		HideCandWindow();
		break;
		
	case WM_IME_COMPOSITIONFULL:
		break;
		
	case WM_IME_SELECT:
		break;
		
	case WM_IME_CONTROL:
		lRet = ControlHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;
		
		
	case WM_IME_NOTIFY:
		lRet = NotifyHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;
		
	case WM_DESTROY:
		break;

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
    }
    return lRet;
}
