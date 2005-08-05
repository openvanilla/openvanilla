#include <stdio.h>
#include "Original.h"

LRESULT APIENTRY StatusWndProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			PaintStatusWindow(hWnd);
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
			{
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
				if(msg == WM_RBUTTONUP) {
					CurrentIC++;
					if(CurrentIC > IC.size() - 1)
						CurrentIC = 0;
					InvalidateRect(uiStatus.hWnd,NULL, FALSE);
				}
			}
			break;
		default:
			if (!MyIsIMEMessage(msg))
				return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
	}
	return 0;
}

void CreateStatusWindow(HWND hUIWnd)
{
	if (!IsWindow(uiStatus.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		SIZE sz;
		TCHAR szStr[100];

		uiStatus.hWnd = 
			CreateWindowEx(WS_EX_WINDOWEDGE, UISTATUSCLASSNAME, NULL,
					WS_DISABLED | WS_POPUP | WS_DLGFRAME,
					0, 0, 1, 1, hUIWnd, NULL, hInst, NULL);
		SetWindowLong(uiStatus.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);

		_stprintf(szStr, _T("AAAAAAAAAAAAAAAAAA"));
		hDC = GetDC(uiStatus.hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, szStr, _tcslen(szStr), &sz);
		SelectObject(hDC, oldFont);
		ReleaseDC(uiStatus.hWnd,hDC);

		uiStatus.sz.cx = sz.cx;
		uiStatus.sz.cy = sz.cy+4;
	}
	HideStatusWindow();
	return;
}

void MoveStatusWindow(HWND hUIWnd, int X, int Y)
{
	if (!IsWindow(uiStatus.hWnd))
		CreateStatusWindow(hUIWnd);

	if (IsWindow(uiStatus.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		POINT pt;
		RECT screenrc;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;
		uiStatus.pt.x = X;
		uiStatus.pt.y = Y;

		if(sz.cx < uiStatus.sz.cx)
			sz.cx = uiStatus.sz.cx;

		sz.cy = uiStatus.sz.cy;
		sz.cx += 4 * GetSystemMetrics(SM_CXEDGE);
		sz.cy += 4 * GetSystemMetrics(SM_CYEDGE);

		pt.x = uiStatus.pt.x;
		pt.y = uiStatus.pt.y;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - sz.cx;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - sz.cy;

		MoveWindow(uiStatus.hWnd,
				pt.x,
				pt.y,
				sz.cx,
				sz.cy,
				TRUE);
		ShowWindow(uiStatus.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(uiStatus.hWnd,NULL, FALSE);
	}
}

void PaintStatusWindow(HWND hStatusWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rc;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

	hDC = BeginPaint(hStatusWnd,&ps);

	GetClientRect(hStatusWnd,&rc);

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


	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, 2, 2, IC.at(CurrentIC), _tcslen(IC.at(CurrentIC)));

	EndPaint(hStatusWnd,&ps);
}

void ShowStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		ShowWindow(uiStatus.hWnd, SW_SHOWNOACTIVATE);
}

void HideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		ShowWindow(uiStatus.hWnd, SW_HIDE);
}

void UISetCursorPos(int i)
{
}

void UISetMarkFrom(int i)
{
}

void UISetMarkTo(int i)
{
}
