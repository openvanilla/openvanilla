#define OV_DEBUG
#include <stdio.h>
#include "PCMan.h"
#include "DotNETHeader.h"
#include <exception>
#include <windows.h>

LRESULT APIENTRY CompWndProc(HWND hWnd,
							 UINT msg,
							 WPARAM wParam,
							 LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//			murmur("WM_PAINT, composition window");
		PaintCompWindow( hWnd);
		break;
	case WM_ERASEBKGND:
		return TRUE;
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

void UICreateCompWindow(HWND hUIWnd)
{
	//	murmur("%1.3f sec:\tC# comp window, create");
	if (!IsWindow(uiComp.hWnd))
	{
		uiComp.hWnd = _CreateCompPage();
		//		murmur("compHWND=%p",uiComp.hWnd);

#if 0
		HDC hDC;
		HFONT oldFont;
		SIZE sz;
		TCHAR szStr[100];

		uiComp.hWnd = 
			CreateWindowEx(0, UICOMPCLASSNAME, NULL,
			WS_DISABLED | WS_POPUP,
			0, 0, 1, 1, hUIWnd, NULL, hInst, NULL);
		SetWindowLong(uiComp.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);

		_stprintf(szStr, _T("AAAAAAAAAAAAA"));
		hDC = GetDC(uiComp. hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPoint(hDC, szStr, (int)_tcslen(szStr), &sz);
		SelectObject(hDC, oldFont);
		ReleaseDC(uiComp.hWnd,hDC);

		uiComp.sz.cx = sz.cx;
		uiComp.sz.cy = sz.cy + 4;
#endif
	}
	//ShowWindow(uiComp.hWnd, SW_HIDE);
	return;
}

void UIMoveCompWindow(HWND hUIWnd, int X, int Y, wchar_t* lpStr)
{
	int newX=X;
	int newY=Y;
	_MoveCompPage(newX, newY);
	/*
	lpCompStr = wcsdup(lpStr);		
	if (IsWindow(uiComp.hWnd))
	{		
	RECT screenrc;
	SystemParametersInfo(SPI_GETWORKAREA,0,&screenrc,0);
	//James comments
	//if( newX+100 > screenrc.right )
	//newX=screenrc.right-100;
	//if( newY+200 > screenrc.bottom )			
	//newY=Y-190;
	HDC hDC;
	HFONT oldFont;
	SIZE sz;
	TCHAR szStr[100];
	TEXTMETRIC tm;
	HWND hLocalHwnd;

	//		LPWSTR localString;
	//hLocalHwnd = GetActiveWindow();
	//		hLocalHwnd = GetForegroundWindow();

	hDC = GetDC(uiComp.hWnd);
	//		murmur(" ---> hLocalHwnd: %d", hLocalHwnd);
	//		murmur(" ---> Parent hLocalHwnd: %d", GetParent(hLocalHwnd));
	//oldFont = (HFONT) SelectObject(hDC, hUIFont);
	GetTextMetrics(hDC, &tm);
	//		murmur("GetTextMetrics %d", tm.tmHeight);


	//oldFont = (HFONT) SendMessage(hLocalHwnd, WM_GETFONT, 0, 0);
	//murmur (" ---> oldFont %d", oldFont);

	//murmur("GetTextExLeading %d", tm.tmExternalLeading);

	if(lpCompStr)
	{
	//		GetTextExtentPoint(hDC, lpCompStr, (int)wcslen(lpCompStr), &sz);
	//		murmur("YES lpCompStr FONT SIZE HEIGHT : %d", sz.cy);
	//		}
	//		else
	//		{
	//		GetTextExtentPoint(hDC, szStr, (int)_tcslen(szStr), &sz);	
	//		murmur("NO lpCompStr FONT SIZE HEIGHT : %d", sz.cy);
	//		}
	GetTextExtentPoint(hDC, lpCompStr, (int)wcslen(lpCompStr), &sz);
	murmur(" ---> GetTextExtentPoint sz.cx : %ld", sz.cx);
	murmur(" ---> GetTextExtentPoint sz.cy : %ld", sz.cy);
	SelectObject(hDC, oldFont);
	ReleaseDC(hLocalHwnd, hDC);		
	//_MoveCompPage(newX,newY+sz.cy);		
	//				_MoveCompPage(newX, newY);
	//		murmur("---> newX : %d , newY : %d", newX, newY);
	}
	}			*/

}
void UISetCompStr(wchar_t* lpStr)
{
	Watch watch;
	watch.start();
	_ClearCompPage();
	watch.stop();
	//	murmur("%1.3f sec:\tC# comp window, clear", watch.getSec());

	lpCompStr = wcsdup(lpStr);
	std::wstring wsCompStr(lpCompStr);
	watch.start();
	_SetCompString(wsCompStr);
	watch.stop();
	//	murmur("%1.3f sec:\tC# comp window, setstring", watch.getSec());
}

void UISetCompCaretPosX(int xIndex)
{
	_SetCompCaretPosX(xIndex);
}

int CompIndexToXPos(int index)
{
	if( !lpCompStr || !*lpCompStr )
		return 1;
	HDC hdc = GetDC( uiComp.hWnd );
	HGDIOBJ oldFont = SelectObject(hdc, hUIFont);
	SIZE size;
	GetTextExtentPoint32( hdc, lpCompStr, index, &size);
	SelectObject(hdc, oldFont);
	ReleaseDC( uiComp.hWnd, hdc );
	return size.cx + 1;
}

void PaintCompWindow(HWND hCompWnd)
{
	//	murmur("PaintCompWindow");
#if 0
	PAINTSTRUCT ps;
	HDC hDC;
	HFONT oldFont;
	RECT rc;
	HBRUSH hBrush = (HBRUSH)NULL;
	HBRUSH hOldBrush = (HBRUSH)NULL;
	HPEN hPen = (HPEN)NULL;
	HPEN hOldPen = (HPEN)NULL;

	hDC = BeginPaint(hCompWnd,&ps);

	GetClientRect(hCompWnd,&rc);
	HDC memdc = CreateCompatibleDC( ps.hdc );
	HBITMAP membmp = CreateCompatibleBitmap( ps.hdc, rc.right, rc.bottom );
	HGDIOBJ oldbmp = SelectObject( memdc, membmp );

	InflateRect( &rc, -1, -1 );

	oldFont = (HFONT)SelectObject(memdc, hUIFont);
	if(lpCompStr)
	{
		SetTextColor( memdc, GetSysColor( COLOR_WINDOWTEXT ) );
		SetBkColor( memdc, GetSysColor( COLOR_WINDOW ) );
		ExtTextOut( memdc, 1, 1, ETO_OPAQUE, &rc, lpCompStr, 
			lstrlen(lpCompStr), NULL);
		int selstart = CompIndexToXPos( CompSelStart );
		int selend = CompIndexToXPos( CompSelEnd );
		int cursor = CompIndexToXPos( CompCursorPos );
		BitBlt( memdc, selstart, 0, selend-selstart, rc.bottom, memdc, selstart, 0, NOTSRCCOPY );
		BitBlt( memdc, cursor, 0, 1, rc.bottom, memdc, cursor, 0, SRCINVERT );

		/*		HDC tmpdc = GetDC(NULL);
		TCHAR debug_info[100];
		_stprintf(debug_info, _T("ss=%d, se=%d, cp=%d"), selstart, selend, cursor);
		TextOut( tmpdc, 0, 0, debug_info, lstrlen(debug_info));
		ReleaseDC( NULL, tmpdc );
		*/
	}

	InflateRect( &rc, 1, 1 );
	Draw3DBorder( memdc, &rc, GetSysColor(COLOR_3DFACE), 0/*GetSysColor(COLOR_3DDKSHADOW)*/);

	BitBlt( hDC, ps.rcPaint.left, ps.rcPaint.top, 
		(ps.rcPaint.right-ps.rcPaint.left), 
		(ps.rcPaint.bottom-ps.rcPaint.top), 
		memdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

	SelectObject( memdc, oldbmp );
	DeleteObject( membmp );
	DeleteDC( memdc );

	SelectObject(memdc, oldFont);

	EndPaint(hCompWnd,&ps);
#endif
	/*
	if(lpCompStr)
	{
	UIShowCompWindow(); 
	return;
	}*/
}

void UIShowCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, show");
	if (IsWindow(uiComp.hWnd))
	{
		_ShowCompPage();
		//	ShowWindow(uiComp.hWnd, SW_SHOWNOACTIVATE);
	}
	if(lpCompStr)
	{
		_SetCompCaretPosX(CompCursorPos);
		//		murmur(" ---> CompCursorPos : %d", CompCursorPos);
	}
}

void UIHideCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, hide");
	if (IsWindow(uiComp.hWnd))
	{
		_HideCompPage();		
	}


}
