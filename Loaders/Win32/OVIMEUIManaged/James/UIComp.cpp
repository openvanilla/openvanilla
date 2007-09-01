//#define OV_DEBUG
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
		//murmur("WM_PAINT, composition window");
		//<comment author='b6s'>
		// Uses the managed UI function instead
		//PaintCompWindow( hWnd);
		//</comment>
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_SETCURSOR:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		//<comment author='b6s'>
		// Uses the managed UI function instead
		/*
		DragUI(hWnd, NULL, msg, wParam, lParam, TRUE);
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
		_SetCompAppHWnd(hUIWnd);
	}
}

void UIMoveCompWindow(
	int x, int y, int fontSize, int fontHeight, const wchar_t* fontName)
{
	_MoveCompPage(x, y, fontSize, fontHeight, fontName);
}

void UISetCompStr(wchar_t* lpStr)
{
	//_ClearCompPage();  //James:®³±¼, no need

	if(wcslen(lpStr))
	{
		lpCompStr = wcsdup(lpStr);
		_SetCompString(lpCompStr);
	}
	/*else  //James:®³±¼, no need
	{
		_ClearCompPage();		
	}*/
}
void UIClearCompStr()
{
	_ClearCompPage();
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

//<comment author='b6s'>
// Uses the managed UI function instead
/*
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

#if 0		
		HDC tmpdc = GetDC(NULL);
		TCHAR debug_info[100];
		_stprintf(debug_info, _T("ss=%d, se=%d, cp=%d"), selstart, selend, cursor);
		TextOut( tmpdc, 0, 0, debug_info, lstrlen(debug_info));
		ReleaseDC( NULL, tmpdc );
#endif
	}

	InflateRect( &rc, 1, 1 );
	Draw3DBorder( memdc, &rc, GetSysColor(COLOR_3DFACE), 0
	//,GetSysColor(COLOR_3DDKSHADOW)
	);

	BitBlt( hDC, ps.rcPaint.left, ps.rcPaint.top, 
		(ps.rcPaint.right-ps.rcPaint.left), 
		(ps.rcPaint.bottom-ps.rcPaint.top), 
		memdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

	SelectObject( memdc, oldbmp );
	DeleteObject( membmp );
	DeleteDC( memdc );

	SelectObject(memdc, oldFont);

	EndPaint(hCompWnd,&ps);
}
*/
//</comment>

void UIShowCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, show");
	if (IsWindow(uiComp.hWnd))
	{
		_ShowCompPage();
		//	ShowWindow(uiComp.hWnd, SW_SHOWNOACTIVATE);
	}
	/*
	if(lpCompStr)
	{
		_SetCompCaretPosX(CompCursorPos);
		murmur("\t---> CompCursorPos : %d", CompCursorPos);
	}
	*/
}

void UIHideCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, hide");
	if (IsWindow(uiComp.hWnd))
	{
		_HideCompPage();		
	}
}

void UISetMarkFrom(int i)
{
	CompSelStart = i;
	_SetCompMarkFrom(CompSelStart);
}

void UISetMarkTo(int i)
{
	CompSelEnd = i;
	_SetCompMarkTo(CompSelEnd);
}

int UIGetHeight()
{
	return _GetHeight();
}

int UIGetCaretPosX()
{
	return _GetCaretPosX();
}
