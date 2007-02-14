#include <stdio.h>
#include "PCMan.h"
#include "DotNETHeader.h"

LRESULT APIENTRY NotifyWndProc(HWND hWnd, 
		UINT msg, 
		WPARAM wParam, 
		LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			//<comment author='b6s'>
			// Uses the managed UI function instead
			//PaintNotifyWindow(hWnd);
			//</comment>
			break;

		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			//<comment author='b6s'>
			// Uses the managed UI function instead
			/*
			DragUI(hWnd, NULL, msg, wParam, lParam, FALSE);
			if ((msg == WM_SETCURSOR) &&
					(HIWORD(lParam) != WM_LBUTTONDOWN) &&
					(HIWORD(lParam) != WM_RBUTTONDOWN)) 
				return DefWindowProc(hWnd, msg, wParam, lParam);
			if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
			*/
			//</comment>
			break;

		default:
			if (!MyIsIMEMessage(msg)){
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}
			break;
	}
	return 0L;
}

void UICreateNotifyWindow(HWND hUIWnd)
{
	if (!IsWindow(uiNotify.hWnd))
	{
		/*
		SIZE sz;	sz.cx = sz.cy = 0;
		HDC hDC;
		HFONT oldFont;
		TCHAR szStr[100];

		uiNotify.hWnd = 
			CreateWindowEx(0, UINOTIFYCLASSNAME ,NULL,
					WS_DISABLED | WS_POPUP,
					0, 0, 1, 1, hUIWnd,NULL,hInst,NULL);
		SetWindowLong(uiNotify.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);

		_stprintf(szStr, _T("A"));
		hDC = GetDC(uiNotify. hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, szStr, (int)_tcslen(szStr), &sz);
		SelectObject(hDC, oldFont);
		ReleaseDC(uiNotify.hWnd,hDC);

		uiNotify.sz.cy = sz.cy + 4;
		*/
		uiNotify.hWnd=_CreateNotifyPage();

	}
	//_ShowNotifyPage();
	//ShowWindow(uiNotify.hWnd, SW_HIDE);
	return;
}

/*
BOOL GetNotifyPosFromCompWnd(LPSIZE lpsz)
{
	if (IsWindow(uiComp.hWnd))
	{
		RECT rc, screenrc;
		POINT pt;

		pt.x = uiComp.pt.x;
		pt.y = uiComp.pt.y;

		GetWindowRect(uiNotify.hWnd, &rc);
		if( pt.x < 0 )
			pt.x = 0;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);
		if( (pt.x + lpsz->cx) > screenrc.right)
			pt.x = rc.left - lpsz->cx - 5;
		if( (pt.y + lpsz->cy) > screenrc.bottom)
			pt.y = screenrc.bottom - lpsz->cy;

		uiNotify.pt.x = pt.x;
		uiNotify.pt.y = pt.y;
		return TRUE;
	}
	else
	{
		uiNotify.pt.x = uiComp.pt.x;
		uiNotify.pt.y = uiComp.pt.y;
	}
	return FALSE;
}
*/

void UIShowNotifyWindow()
{
	/*
	if( !*lpStr || !lpStr)
	{
		UIHideNotifyWindow();
		return;
	}
    */
	
	/*if (!IsWindow(uiNotify.hWnd))
		UICreateNotifyWindow(hUIWnd);*/
	
	//_SetNotifyString(lpNotifyStr);
	_ShowNotifyPage();
/*
	if (IsWindow(uiNotify.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;

		hDC = GetDC(uiNotify.hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, lpNotifyStr, (int)wcslen(lpNotifyStr), &sz);
		SelectObject(hDC, oldFont);

		sz.cy = uiNotify.sz.cy;
		sz.cx += 2;
		uiNotify.pt.x = uiComp.pt.x;
		uiNotify.pt.y = uiComp.pt.y;

		SelectObject(hDC, oldFont);
		ReleaseDC(uiNotify.hWnd,hDC);

//		GetNotifyPosFromCompWnd(&sz);
		MoveWindow(uiNotify.hWnd,
				uiNotify.pt.x,
				uiNotify.pt.y,
				sz.cx,
				sz.cy,
				TRUE);
		ShowWindow(uiNotify.hWnd, SW_SHOWNOACTIVATE);
		InvalidateRect(uiNotify.hWnd, NULL, FALSE);
		
	}*/
}

//<comment author='b6s'>
// Uses the managed UI function instead
/*
void PaintNotifyWindow(HWND hNotifyWnd)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HFONT oldFont;
	RECT rc;
	//DWORD i;

	hDC = BeginPaint(hNotifyWnd,&ps);
	oldFont = (HFONT)SelectObject(hDC, hUIFont);

	GetClientRect(hNotifyWnd,&rc);

	if(lpNotifyStr)
	{
		ExtTextOut( hDC, 1, 1, ETO_OPAQUE, &rc, lpNotifyStr, (int)wcslen(lpNotifyStr), NULL);
	}
	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_3DFACE), 0
	//, GetSysColor(COLOR_3DDKSHADOW)
	);
	SelectObject(hDC, oldFont);
	EndPaint(hNotifyWnd,&ps);
}
*/
//</comment>

void UIHideNotifyWindow()
{
	if (IsWindow(uiNotify.hWnd))
		ShowWindow(uiNotify.hWnd, SW_HIDE);
}

void UIMoveNotifyWindow(int X, int Y)
{

}

void UISetNotifyStr(wchar_t* lpStr)
{
	free(lpNotifyStr);
	lpNotifyStr = wcsdup(lpStr);
	_SetNotifyString(lpNotifyStr);
}