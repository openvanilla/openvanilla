#include "OVIME.h"

LRESULT APIENTRY UIWndProc(HWND hWnd,
						   UINT msg,
						   WPARAM wParam,
						   LPARAM lParam)
{
	HIMC			hUICurIMC;
	LPINPUTCONTEXT	lpIMC;
	LPUIEXTRA		lpUIExtra;
	LONG			lRet = 0L;

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
		//
		// Allocate UI's extra memory block.
		//
		hUIExtra = GlobalAlloc(GHND, sizeof(UIEXTRA));
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

		lpUIExtra->uiComp.pt.x = -1;
		lpUIExtra->uiComp.pt.y = -1;

		CreateCompWindow(hWnd, lpUIExtra);
		CreateCandWindow(hWnd, lpUIExtra);

		GlobalUnlock(hUIExtra);
		SetWindowLong(hWnd, IMMGWL_PRIVATE, (DWORD)hUIExtra);
		break;
		
	case WM_IME_SETCONTEXT:
		if (wParam)
		{
			hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
			lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
			if (hUICurIMC)
			{
				lpIMC = ImmLockIMC(hUICurIMC);
				if (lpIMC)
				{
					MoveCandWindow(hWnd,lpUIExtra,lpIMC);
					MoveCompWindow(hWnd,lpUIExtra,lpIMC);
				}
				else
				{
					HideCandWindow(lpUIExtra);
					HideCompWindow(lpUIExtra);
				}
				ImmUnlockIMC(hUICurIMC);
			}
			else   // it is NULL input context.
			{
				HideCandWindow(lpUIExtra);
				HideCompWindow(lpUIExtra);
				
			}
			GlobalUnlock(hUIExtra);
		}
		break;
		
	case WM_IME_STARTCOMPOSITION:
		break;
		
	case WM_IME_COMPOSITION:
		lpIMC = ImmLockIMC(hUICurIMC);
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		MoveCompWindow(hWnd, lpUIExtra, lpIMC);
		MoveCandWindow(hWnd,lpUIExtra,lpIMC);
		GlobalUnlock(hUIExtra);
		ImmUnlockIMC(hUICurIMC);
		break;
		
	case WM_IME_ENDCOMPOSITION:
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		HideCompWindow(lpUIExtra);
		HideCandWindow(lpUIExtra);
		GlobalUnlock(hUIExtra);
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

	case WM_UI_COMPMOVE:
		hUIExtra = (HGLOBAL)GetWindowLong(hWnd,IMMGWL_PRIVATE);
		lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);
		lpUIExtra->uiComp.pt.x = (long)LOWORD(lParam);
		lpUIExtra->uiComp.pt.y = (long)HIWORD(lParam);
		GlobalUnlock(hUIExtra);
		break;

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
    }
    return lRet;
}

void DrawUIBorder( LPRECT lprc )
{
    HDC hDC;
    int sbx, sby;
	
    hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
    SelectObject(hDC, GetStockObject(GRAY_BRUSH));
    sbx = GetSystemMetrics( SM_CXBORDER );
    sby = GetSystemMetrics( SM_CYBORDER );
    PatBlt( hDC, lprc->left, 
		lprc->top, 
		lprc->right - lprc->left-sbx, 
		sby, PATINVERT );
    PatBlt( hDC, lprc->right - sbx, 
		lprc->top, 
		sbx, 
		lprc->bottom - lprc->top-sby, PATINVERT );
    PatBlt( hDC, lprc->right, 
		lprc->bottom-sby, 
		-(lprc->right - lprc->left-sbx), 
		sby, PATINVERT );
    PatBlt( hDC, lprc->left, 
		lprc->bottom, 
		sbx, 
		-(lprc->bottom - lprc->top-sby), PATINVERT );
    DeleteDC( hDC );
}

void DragUI(HWND hWnd, HWND hWnd1,
			UINT msg, WPARAM wParam, LPARAM lParam,
			BOOL fIsCompWnd)
{
    POINT     pt;
    static    POINT ptdif, ptdif1;
    static    RECT drc, drc1;
    static    SIZE sz, sz1;
    DWORD     dwT;
	
    switch (msg)
    {
	case WM_SETCURSOR:
		if ( HIWORD(lParam) == WM_LBUTTONDOWN
			|| HIWORD(lParam) == WM_RBUTTONDOWN ) 
		{
			GetCursorPos( &pt );
			SetCapture(hWnd);
			GetWindowRect(hWnd,&drc);
			ptdif.x = pt.x - drc.left;
			ptdif.y = pt.y - drc.top;
			sz.cx = drc.right - drc.left;
			sz.cy = drc.bottom - drc.top;

			if (IsWindow(hWnd1)) {
				GetWindowRect(hWnd1,&drc1);
				ptdif1.x = pt.x - drc1.left;
				ptdif1.y = pt.y - drc1.top;
				sz1.cx = drc1.right - drc1.left;
				sz1.cy = drc1.bottom - drc1.top;
			}

			SetWindowLong(hWnd, FIGWL_MOUSE, FIM_CAPUTURED);
		}
		break;
		
	case WM_MOUSEMOVE:
		dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
		if (dwT & FIM_MOVED)
		{
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1))
				DrawUIBorder(&drc1);
			GetCursorPos( &pt );
			drc.left   = pt.x - ptdif.x;
			drc.top    = pt.y - ptdif.y;
			drc.right  = drc.left + sz.cx;
			drc.bottom = drc.top + sz.cy;
			if (IsWindow(hWnd1)) 
			{
				drc1.left   = pt.x - ptdif1.x;
				drc1.top    = pt.y - ptdif1.y;
				drc1.right  = drc1.left + sz1.cx;
				drc1.bottom = drc1.top + sz1.cy;
			}
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1))
				DrawUIBorder(&drc1);
		}
		else if (dwT & FIM_CAPUTURED)
		{
			DrawUIBorder(&drc);
			if (IsWindow(hWnd1))
				DrawUIBorder(&drc1);
			SetWindowLong(hWnd,FIGWL_MOUSE,dwT | FIM_MOVED);
		}
		break;
		
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
		
		if (dwT & FIM_CAPUTURED)
		{
			ReleaseCapture();
			if (dwT & FIM_MOVED)
			{
				DrawUIBorder(&drc);
				if (IsWindow(hWnd1))
					DrawUIBorder(&drc1);
				GetCursorPos( &pt );
				MoveWindow(hWnd,pt.x - ptdif.x,
					pt.y - ptdif.y,
					sz.cx,
					sz.cy,TRUE);

				if(fIsCompWnd) {
					HWND hUIWnd;
					LPARAM mylParam;

					*((LPWORD)(&mylParam)) = (WORD)(pt.x - ptdif.x);
					*((LPWORD)(&mylParam)+1) = (WORD)(pt.y - ptdif.y);

					hUIWnd = (HWND)GetWindowLong(hWnd, FIGWL_SVRWND);
					if (IsWindow(hUIWnd))
						SendMessage(hUIWnd, WM_UI_COMPMOVE, 0, mylParam);
				}

				if (IsWindow(hWnd1)) {
					MoveWindow(hWnd1,pt.x - ptdif1.x,
						pt.y - ptdif1.y,
						sz1.cx,
						sz1.cy,TRUE);
				}
			}
		}
		break;
    }
}