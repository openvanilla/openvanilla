#include <stdio.h>
#include "Original.h"

LRESULT APIENTRY CompWndProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			PaintCompWindow( hWnd);
			break;
		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			DragUI(hWnd, NULL, msg, wParam, lParam, TRUE);
			if ((msg == WM_SETCURSOR) &&
					(HIWORD(lParam) != WM_LBUTTONDOWN) &&
					(HIWORD(lParam) != WM_RBUTTONDOWN))
				return DefWindowProc(hWnd, msg, wParam, lParam);
			if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
			break;
		default:
			if (!MyIsIMEMessage(msg))
				return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
	}
	return 0;
}

void CreateCompWindow(HWND hUIWnd)
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
	}
	HideCompWindow();
	return;
}

void MoveCompWindow(HWND hUIWnd, LPTSTR lpStr)
{
	free(lpCompStr);
	lpCompStr = _tcsdup(lpStr);
	if(!_tcscmp(lpStr, _T("")))
	{
		HideCompWindow();
		return;
	}

	if (!IsWindow(lpUIExtra->uiComp.hWnd))
		CreateCompWindow(hUIWnd);

	if (IsWindow(lpUIExtra->uiComp.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		POINT pt;
		RECT screenrc;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;

		if(lpCompStr)
		{
			hDC = GetDC(lpUIExtra->uiComp.hWnd);
			oldFont = (HFONT)SelectObject(hDC, hUIFont);
			GetTextExtentPoint(hDC, lpCompStr, _tcslen(lpCompStr), &sz);
			SelectObject(hDC, oldFont);
			ReleaseDC(lpUIExtra->uiComp.hWnd, hDC);
			if(_tcslen(lpCompStr))
				sz.cx += 2 * sz.cx / _tcslen(lpCompStr);
		}
		else
		{
			HideCompWindow();
			return;
		}

		if(sz.cx < lpUIExtra->uiComp.sz.cx)
			sz.cx = lpUIExtra->uiComp.sz.cx;

		sz.cy = lpUIExtra->uiComp.sz.cy;
		sz.cx += 4 * GetSystemMetrics(SM_CXEDGE);
		sz.cy += 4 * GetSystemMetrics(SM_CYEDGE);

		pt.x = lpUIExtra->uiComp.pt.x;
		pt.y = lpUIExtra->uiComp.pt.y;

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
		ShowWindow(lpUIExtra->uiComp.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(lpUIExtra->uiComp.hWnd,NULL, FALSE);
	}
}

void PaintCompWindow(HWND hCompWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HFONT oldFont;
	RECT rc;
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


	if(lpCompStr)
	{
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 2, 2, lpCompStr, _tcslen(lpCompStr));
	}

	SelectObject(hDC, oldFont);
	EndPaint(hCompWnd,&ps);
}

void HideCompWindow()
{
	if (IsWindow(lpUIExtra->uiComp.hWnd))
	{
		ShowWindow(lpUIExtra->uiComp.hWnd, SW_HIDE);
	}
}
