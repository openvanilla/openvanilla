#include "OVIME.h"

LRESULT APIENTRY CompWndProc(HWND hWnd,
							 UINT msg,
							 WPARAM wParam,
							 LPARAM lParam)
{
	HWND  hUIWnd;
    HGLOBAL hUIExtra;
    LPUIEXTRA lpUIExtra;

	hUIWnd = (HWND)GetWindowLong(hWnd,FIGWL_SVRWND);
    hUIExtra = (HGLOBAL)GetWindowLong(hUIWnd,IMMGWL_PRIVATE);
	lpUIExtra = (LPUIEXTRA)GlobalLock(hUIExtra);

    switch (msg)
    {
	case WM_PAINT:
		PaintCompWindow( hWnd);
		break;
	case WM_SETCURSOR:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		DragUI(hWnd, lpUIExtra->uiCand.hWnd, msg, wParam, lParam, TRUE);
		if ((msg == WM_SETCURSOR) &&
			(HIWORD(lParam) != WM_LBUTTONDOWN) &&
			(HIWORD(lParam) != WM_RBUTTONDOWN))
			return DefWindowProc(hWnd, msg, wParam, lParam);
		if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
			SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
		break;
	default:
		if (!MyIsIMEMessage(msg))
		{
			GlobalUnlock(hUIExtra);
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		break;
    }
	GlobalUnlock(hUIExtra);
	return 0;
}

void CreateCompWindow(HWND hUIWnd, LPUIEXTRA lpUIExtra)
{
    if (!IsWindow(lpUIExtra->uiComp.hWnd))
    {
		HDC hDC;
		HFONT oldFont;
		SIZE sz;
		TCHAR szStr[100];

		lpUIExtra->uiComp.hWnd = 
			CreateWindowEx(WS_EX_WINDOWEDGE, UICOMPCLASSNAME, NULL,
						   WS_DISABLED | WS_POPUP | WS_DLGFRAME,
						   0, 0, 1, 1, hUIWnd, NULL, hInst, NULL);
		SetWindowLong(lpUIExtra->uiComp.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);
	
		_stprintf(szStr, _T("AAAAAAAAAAAAA"));
		hDC = GetDC(lpUIExtra->uiComp. hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, szStr, _tcslen(szStr), &sz);
		SelectObject(hDC, oldFont);
		ReleaseDC(lpUIExtra->uiComp.hWnd,hDC);

		lpUIExtra->uiComp.sz.cx = sz.cx;
		lpUIExtra->uiComp.sz.cy = sz.cy+4;
		DebugLog("CreateCompWindow: %d\n", (void*)IsWindow(lpUIExtra->uiComp.hWnd));
    }
	HideCompWindow(lpUIExtra);
    return;
}

void MoveCompWindow(HWND hUIWnd, LPUIEXTRA lpUIExtra, LPINPUTCONTEXT lpIMC)
{
	DebugLog("MoveCompWindow\n", NULL);
	if (!IsWindow(lpUIExtra->uiComp.hWnd))
		CreateCompWindow(hUIWnd, lpUIExtra);

	if (IsWindow(lpUIExtra->uiComp.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		LPCOMPOSITIONSTRING lpCompStr;
		POINT pt;
		RECT screenrc;
		LPTSTR lpStr;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;

		if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
		{
			DebugLog("dwCompStrLen %u\n", (void*)lpCompStr->dwCompStrLen);
			if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING))
				&& (lpCompStr->dwCompStrLen > 0))
			{
				lpStr = GETLPCOMPSTR(lpCompStr);
					//((LPMYCOMPSTR)lpCompStr)->szCompStr;// FreePYComp.szPaintCompStr;

				hDC = GetDC(lpUIExtra->uiComp.hWnd);
				oldFont = (HFONT)SelectObject(hDC, hUIFont);
				GetTextExtentPoint(hDC, lpStr, _tcslen(lpStr), &sz);
				SelectObject(hDC, oldFont);
				ReleaseDC(lpUIExtra->uiComp.hWnd, hDC);
				if(_tcslen(lpStr))
					sz.cx += 2 * sz.cx / _tcslen(lpStr);
				ImmUnlockIMCC(lpIMC->hCompStr);
			}
			else
			{
				HideCompWindow(lpUIExtra);
				ImmUnlockIMCC(lpIMC->hCompStr);
				return;
			}

		}

		if(sz.cx < lpUIExtra->uiComp.sz.cx)
			sz.cx = lpUIExtra->uiComp.sz.cx;

		sz.cy = lpUIExtra->uiComp.sz.cy;
		sz.cx += 4 * GetSystemMetrics(SM_CXEDGE);
		sz.cy += 4 * GetSystemMetrics(SM_CYEDGE);

		if(lpUIExtra->uiComp.pt.x < 0)
		{
			pt.x = 15;
			pt.y = 15;
			ClientToScreen(lpIMC->hWnd, &pt);
		}
		else
		{
			pt.x = lpUIExtra->uiComp.pt.x;
			pt.y = lpUIExtra->uiComp.pt.y;
		}

		SystemParametersInfo(SPI_GETWORKAREA,
							0,
							&screenrc,
							0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - sz.cx;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - sz.cy;

		MoveWindow(lpUIExtra->uiComp.hWnd,
			pt.x,
			pt.y,
			sz.cx,
			sz.cy,
			TRUE);
		DebugLog("MoveWindow X: %ld", (void*)pt.x);
		DebugLog(" Y: %ld\n", (void*)pt.y);
		ShowWindow(lpUIExtra->uiComp.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(lpUIExtra->uiComp.hWnd,NULL, FALSE);
	}
	else
	{
		DebugLog("IsWindow Comp: %d\n", (void*)IsWindow(lpUIExtra->uiComp.hWnd));
	}
}

void PaintCompWindow(HWND hCompWnd)
{
    PAINTSTRUCT ps;
    HIMC hIMC;
    LPINPUTCONTEXT lpIMC;
    LPCOMPOSITIONSTRING lpCompStr;
    HDC hDC;
	HFONT oldFont;
	RECT rc;
    HWND hSvrWnd;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

    hDC = BeginPaint(hCompWnd,&ps);
	oldFont = (HFONT)SelectObject(hDC, hUIFont);

	GetClientRect(hCompWnd,&rc);

	hBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	PatBlt(hDC,
			rc.left,
			rc.top ,
			rc.right - GetSystemMetrics(SM_CXEDGE)/2,
			rc.bottom - GetSystemMetrics(SM_CYEDGE)/2,
			PATCOPY);
	if(hBrush && hOldBrush)
		SelectObject(hDC,hOldBrush);

	hPen = (HPEN)GetStockObject(WHITE_PEN);
	hOldPen = (HPEN)SelectObject(hDC,hPen);
	MoveToEx(hDC,0,rc.bottom - GetSystemMetrics(SM_CXEDGE) / 2, NULL);
	LineTo(hDC, rc.right - GetSystemMetrics(SM_CXEDGE) / 2,
		rc.bottom - GetSystemMetrics(SM_CXEDGE) / 2);
	LineTo(hDC, rc.right - GetSystemMetrics(SM_CXEDGE) / 2, 0);

	hPen = CreatePen(PS_SOLID ,0,RGB(128,128,128));
	SelectObject(hDC,hPen);
	MoveToEx(hDC, rc.right - GetSystemMetrics(SM_CXEDGE) / 2, 0, NULL);
	LineTo(hDC, 0, 0);
	LineTo(hDC, 0, rc.bottom - GetSystemMetrics(SM_CYEDGE) / 2);

	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);

    hSvrWnd = (HWND)GetWindowLong(hCompWnd, FIGWL_SVRWND);

    if (hIMC = (HIMC)GetWindowLong(hSvrWnd, IMMGWL_IMC))
    {
		if( (lpIMC = ImmLockIMC(hIMC)) == NULL ){
			SelectObject(hDC, oldFont);
			EndPaint(hCompWnd,&ps);
			return;
		}
        if (lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr))
        {
            if ((lpCompStr->dwSize > sizeof(COMPOSITIONSTRING))
               && (lpCompStr->dwCompStrLen > 0))
            {
                LPTSTR lpStr,lpPaintStr;
				SIZE sz,sz1;
				SHORT wEditCaret;
				SHORT wCharNum,wCount;
				int i;

				wEditCaret = ((LPMYCOMPSTR)lpCompStr)->cs.dwCursorPos;
				lpStr = GETLPCOMPSTR(lpCompStr);
				SetBkMode(hDC, TRANSPARENT);
				TextOut(hDC, 2, 2, lpStr, _tcslen(lpStr));

				wCount = 0;
				for(i = _tcslen(lpStr); i; i--) {
					if(wCount == wEditCaret)
						break;
					if( *(lpPaintStr + i -1 ) != _T(' '))
						wCount++;
				}

				GetTextExtentPoint(hDC, lpStr, i, &sz);
				GetTextExtentPoint(hDC, _T("A"), 1, &sz1);

				hPen = CreatePen(PS_SOLID, 2,RGB(0,0,0));
				hOldPen = (HPEN)SelectObject(hDC,hPen);
				MoveToEx(hDC, sz.cx + sz1.cx / 4, sz1.cy * 11 / 10, NULL);
				LineTo(hDC, sz.cx + sz1.cx * 2 / 3, sz1.cy * 11 / 10);
				SelectObject(hDC,hOldPen);
				DeleteObject(hPen);
            }
            ImmUnlockIMCC(lpIMC->hCompStr);
        }
        ImmUnlockIMC(hIMC);
    }

	SelectObject(hDC, oldFont);
    EndPaint(hCompWnd,&ps);
}

void HideCompWindow(LPUIEXTRA lpUIExtra)
{
	DebugLog("HideCompWindow\n", NULL);
    if (IsWindow(lpUIExtra->uiComp.hWnd))
    {
        ShowWindow(lpUIExtra->uiComp.hWnd, SW_HIDE);
    }
}