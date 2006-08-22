#include <stdio.h>
#include "Original.h"

LRESULT APIENTRY CandWndProc(HWND hWnd, 
		UINT msg, 
		WPARAM wParam, 
		LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			PaintCandWindow(hWnd);
			break;

		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			DragUI(hWnd, NULL, msg, wParam, lParam, FALSE);
			if ((msg == WM_SETCURSOR) &&
					(HIWORD(lParam) != WM_LBUTTONDOWN) &&
					(HIWORD(lParam) != WM_RBUTTONDOWN)) 
				return DefWindowProc(hWnd, msg, wParam, lParam);
			if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
			break;

		default:
			if (!MyIsIMEMessage(msg)){
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}
			break;
	}
	return 0L;
}

void UICreateCandWindow(HWND hUIWnd)
{
	if (!IsWindow(uiCand.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		TCHAR szStr[100];
		SIZE sz;

		uiCand.hWnd = 
			CreateWindowEx(WS_EX_WINDOWEDGE, UICANDCLASSNAME ,NULL,
					WS_DISABLED | WS_POPUP | WS_DLGFRAME,
					0, 0, 1, 1, hUIWnd,NULL,hInst,NULL);
		SetWindowLong(uiCand.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);

		hDC = GetDC(uiCand.hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);

		_stprintf(szStr,_T("<< 1代琌 2代琌 3代琌 4代琌 5代琌 6代琌 7代琌 8代琌 9代琌 0代琌 >>"));
		GetTextExtentPoint(hDC, szStr, _tcslen(szStr), &sz);

		SelectObject(hDC, oldFont);
		ReleaseDC(uiCand.hWnd,hDC);

		uiCand.sz.cx = sz.cx;
		uiCand.sz.cy = sz.cy + 15;
	}
	ShowWindow(uiCand.hWnd, SW_HIDE);
	return;
}

BOOL GetCandPosFromCompWnd(LPSIZE lpsz)
{
	if (IsWindow(uiComp.hWnd))
	{
		RECT rc, screenrc;
		POINT pt;

		GetWindowRect(uiComp.hWnd, &rc);

		pt.x = rc.left;
		pt.y = rc.bottom + 2;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);
		if( (pt.x + lpsz->cx) > screenrc.right)
			pt.x = rc.left - lpsz->cx - 5;
		if( (pt.y + lpsz->cy) > screenrc.bottom)
			pt.y = screenrc.bottom - lpsz->cy;

		uiCand.pt.x = pt.x;
		uiCand.pt.y = pt.y;
		return TRUE;
	}
	return FALSE;
}

void UIMoveCandWindow(HWND hUIWnd, LPTSTR lpStr)
{
	free(lpCandStr);
	lpCandStr = _tcsdup(lpStr);
	if(!_tcscmp(lpStr, _T("")))
	{
		UIHideCandWindow();
		return;
	}

	if (!IsWindow(uiCand.hWnd))
		UICreateCandWindow(hUIWnd);

	if (IsWindow(uiCand.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;

		// ⊿Τ Cand
		if(lpStr == NULL) {
			ShowWindow(uiCand.hWnd, SW_HIDE);
			return;
		}
		hDC = GetDC(uiCand.hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, lpStr, _tcslen(lpStr), &sz);
		SelectObject(hDC, oldFont);
		ReleaseDC(uiCand.hWnd,hDC);
		if(_tcslen(lpStr))
			sz.cx += 3 * sz.cx / _tcslen(lpStr);
		if(sz.cx < uiCand.sz.cx)
			sz.cx = uiCand.sz.cx;
		sz.cy = uiCand.sz.cy;

		GetCandPosFromCompWnd(&sz);

		MoveWindow(uiCand.hWnd,
				uiCand.pt.x,
				uiCand.pt.y,
				sz.cx,
				sz.cy,
				TRUE);
		ShowWindow(uiCand.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(uiCand.hWnd, NULL, FALSE);
	}
}

void PaintCandWindow(HWND hCandWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HFONT oldFont;
	RECT rc;
	DWORD i;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

	hDC = BeginPaint(hCandWnd,&ps);
	oldFont = (HFONT)SelectObject(hDC, hUIFont);

	GetClientRect(hCandWnd,&rc);

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
	MoveToEx(hDC,0,rc.bottom - GetSystemMetrics(SM_CXEDGE)/2,NULL);
	LineTo(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,rc.bottom - GetSystemMetrics(SM_CXEDGE)/2);
	LineTo(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,0);

	hPen = CreatePen(PS_SOLID ,0,RGB(128,128,128));
	SelectObject(hDC,hPen);
	MoveToEx(hDC,rc.right-GetSystemMetrics(SM_CXEDGE)/2,0,NULL);
	LineTo(hDC,0,0);
	LineTo(hDC,0,rc.bottom-GetSystemMetrics(SM_CYEDGE)/2);

	SelectObject(hDC,hOldPen);
	DeleteObject(hPen);


	if(lpCandStr)
	{
		SetBkMode(hDC,TRANSPARENT);
		TextOut(hDC, 1, 3, lpCandStr, _tcslen(lpCandStr));
	}

	SelectObject(hDC, oldFont);
	EndPaint(hCandWnd,&ps);
}

void UIShowCandWindow()
{
	if (IsWindow(uiCand.hWnd))
		ShowWindow(uiCand.hWnd, SW_SHOWNOACTIVATE);
}

void UIHideCandWindow()
{
	if (IsWindow(uiCand.hWnd))
		ShowWindow(uiCand.hWnd, SW_HIDE);
}
