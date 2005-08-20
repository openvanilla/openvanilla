#include <stdio.h>
#include "PCMan.h"
#include "resource.h"

//	SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
//  This standard toolbar message provided by Windows has some known bugs.
//  So I implemented a new function myself to prevent the problem.
void GetToolbarSize(HWND toolbar, SIZE *sz)
{
	sz->cx = sz->cy = 0;
	int c = SendMessage( toolbar, TB_BUTTONCOUNT, 0, 0);
	for( int i = 0; i < c ; ++i )
	{
		RECT itemrc;
		SendMessage( toolbar, TB_GETITEMRECT, i, LPARAM(&itemrc));
		sz->cx += (itemrc.right - itemrc.left);
		itemrc.bottom -= itemrc.top;
		if( itemrc.bottom > sz->cy )
			sz->cy = itemrc.bottom;
	}
}

LRESULT APIENTRY StatusWndProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam)
{
	POINT pt;
	UINT id;
	switch (msg)
	{
		case WM_PAINT:
			PaintStatusWindow(hWnd);
			break;
		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:

			if( GetCapture() != hWnd && GetCursorPos( &pt ) && 
				ScreenToClient( hWnd, &pt ) && pt.x >= 12 )
				return DefWindowProc(hWnd, msg, wParam, lParam);

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
		case WM_NOTIFY:
			{
				switch( ((NMHDR*)lParam)->code ) 
				{
				case TTN_GETDISPINFO:
				{ 
					LPTOOLTIPTEXT pttt;
					pttt = (LPTOOLTIPTEXT)lParam;
					pttt->hinst = hInstDLL;
					pttt->lpszText = MAKEINTRESOURCE(pttt->hdr.idFrom);
					break;
				}
				}
			}
			break;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
			break;
		case WM_SIZE:
			{
				int cx = LOWORD(lParam);
				int cy = HIWORD(lParam);
				MoveWindow( hToolbar, 12, 2, cx-13, cy-4, TRUE);
			}
			break;
		case WM_COMMAND:
			id = LOWORD(wParam);
			switch( id )
			{
			case ID_CHANGE_IME:
				{
					RECT rc;
					SendMessage( hToolbar, TB_GETITEMRECT, 0, LPARAM(&rc) );
					MapWindowPoints( hToolbar, HWND_DESKTOP, LPPOINT(&rc), 2 );
					CheckMenuItem( hIMESelMenu, ID_IME_LIST_FIRST + CurrentIC, MF_CHECKED );
					TrackPopupMenu( hIMESelMenu, TPM_LEFTALIGN, rc.left, rc.bottom, 0, hWnd, NULL);
					CheckMenuItem( hIMESelMenu, ID_IME_LIST_FIRST + CurrentIC, MF_UNCHECKED );
				}
				break;
			case ID_CHI_ENG:
				{
					isChinese = !isChinese;
					SendMessage( hToolbar, TB_CHANGEBITMAP, ID_CHI_ENG, MAKELPARAM(isChinese ? 2 : 3, 0));
				}
				break;
			case ID_FULL_HALF:
				{
					isFull = !isFull;
					SendMessage( hToolbar, TB_CHANGEBITMAP, ID_FULL_HALF, MAKELPARAM(isFull ? 4 : 5, 0));
				}
				break;
			case ID_CONFIG:
				{
					TCHAR exe_path[MAX_PATH];
					TCHAR file_name[] = _T("\\OpenVanilla\\OVPreferences.exe");
				    GetWindowsDirectory(exe_path, MAX_PATH - _tcslen( file_name ) );
				    _stprintf(exe_path, _T("%s\\%s"), exe_path, file_name);

					ShellExecute( NULL, _T("open"), exe_path, NULL, NULL, SW_SHOWNORMAL );
				}
				break;
			case ID_IME_HELP:
				
				break;
			default:
				if( id >= ID_IME_LIST_FIRST && id < ID_IME_LIST_LAST )
				{
					id -= ID_IME_LIST_FIRST;
					CurrentIC = id;

					TBBUTTONINFO tbi;	tbi.cbSize = sizeof(tbi);
					tbi.dwMask = TBIF_TEXT;		tbi.pszText = IC.at(CurrentIC);

					SendMessage( hToolbar, TB_SETBUTTONINFO, ID_CHANGE_IME, LPARAM(&tbi));

					SIZE sz;
//					SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
					GetToolbarSize( hToolbar, &sz );

					uiStatus.sz.cx = sz.cx + 14;
					uiStatus.sz.cy = sz.cy + 4;
					RECT rc;
					GetWindowRect(uiStatus.hWnd, &rc);
					UIMoveStatusWindow( uiStatus.hWnd, rc.left, rc.top );
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


TBBUTTON toolbar_btns[]={
	{0, ID_CHANGE_IME, TBSTATE_ENABLED ,TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, {0}, 0, 0 }, 
	{1, ID_CONFIG, TBSTATE_ENABLED ,TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, {0}, 0, 0 },
	{2, ID_CHI_ENG, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 },
	{5, ID_FULL_HALF, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 },
	{6, ID_IME_HELP, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 }
};


void UICreateStatusWindow(HWND hUIWnd)
{
	if (!IsWindow(uiStatus.hWnd))
	{
		SIZE sz;

		uiStatus.hWnd = 
			CreateWindowEx(WS_EX_CONTROLPARENT|WS_EX_TOPMOST, UISTATUSCLASSNAME, NULL,
					WS_POPUP | WS_CLIPCHILDREN,
					0, 0, 1, 1, hUIWnd, NULL, hInst, NULL);
		SetWindowLong(uiStatus.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);

		hToolbar = CreateToolbarEx( uiStatus.hWnd, 
			TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_LIST|CCS_NODIVIDER|CCS_NORESIZE|
			WS_CHILD|WS_VISIBLE|CCS_NOPARENTALIGN, 
			10, 3, hInstDLL, 0, 
			toolbar_btns, sizeof(toolbar_btns)/sizeof(TBBUTTON), 
			16, 16, 16, 16, sizeof(TBBUTTON));

//		hToolbar = CreateWindowEx( 0, TOOLBARCLASSNAME, NULL, CCS_NOPARENTALIGN|WS_CHILD|WS_VISIBLE, 
//			100, 1, 20, 24, uiStatus.hWnd, NULL, hInstDLL, NULL);

		HIMAGELIST himl = ImageList_Create( 16, 16, ILC_COLOR24|ILC_MASK, 7, 0);
		HBITMAP htbbmp = LoadBitmap( hInstDLL, LPCTSTR(IDB_STATUS_TB) );
		ImageList_RemoveAll(himl);
		ImageList_AddMasked( himl, htbbmp, RGB(192, 192, 192) );
		DeleteObject(htbbmp);
		himl = (HIMAGELIST)SendMessage( hToolbar, TB_SETIMAGELIST, 0, LPARAM(himl));
		if( himl )
			ImageList_Destroy( himl );

		TBBUTTONINFO tbi;	tbi.cbSize = sizeof(tbi);
		tbi.dwMask = TBIF_TEXT;		tbi.pszText = IC.at(CurrentIC);
		SendMessage( hToolbar, TB_SETBUTTONINFO, ID_CHANGE_IME, LPARAM(&tbi));

//		SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
		GetToolbarSize( hToolbar, &sz );

		uiStatus.sz.cx = sz.cx + 14;
		uiStatus.sz.cy = sz.cy + 4;
		if( uiStatus.sz.cy < 26 )
			uiStatus.sz.cy = 26;
	}
	UIHideStatusWindow();
	return;
}

void UIMoveStatusWindow(HWND hUIWnd, int X, int Y)
{
	if (!IsWindow(uiStatus.hWnd))
		UICreateStatusWindow(hUIWnd);

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

	hDC = BeginPaint(hStatusWnd,&ps);
	GetClientRect(hStatusWnd,&rc);

	FillSolidRect( hDC, &rc, GetSysColor(COLOR_BTNFACE));

	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_BTNHILIGHT), 0/*GetSysColor(COLOR_BTNSHADOW)*/);

	InflateRect( &rc, -3, -3 );
	rc.left++;
	rc.right = rc.left + 3;
	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
	OffsetRect( &rc, 4, 0);
	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));

	EndPaint(hStatusWnd,&ps);
}

void UIShowStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		ShowWindow(uiStatus.hWnd, SW_SHOWNOACTIVATE);
}

void UIHideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		ShowWindow(uiStatus.hWnd, SW_HIDE);
}

int CompIndexToXPos(int index);

void UISetCursorPos(int i)
{
	int cursor = CompIndexToXPos( CompCursorPos );
	RECT rc;
	GetClientRect( uiComp.hWnd, &rc );
	HDC dc = GetDC( uiComp.hWnd );
	BitBlt( dc, cursor, 0, 1, rc.bottom, dc, cursor, 0, SRCINVERT );
	CompCursorPos = i;
	cursor = CompIndexToXPos( CompCursorPos );
	BitBlt( dc, cursor, 0, 1, rc.bottom, dc, cursor, 0, SRCINVERT );
}

void UISetMarkFrom(int i)
{
	CompSelStart = i;
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mf=%d"),  i);
	TextOut( tmpdc, 0, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
}

void UISetMarkTo(int i)
{
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mt=%d"),  i);
	TextOut( tmpdc, 40, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
	CompSelEnd = i;
}
