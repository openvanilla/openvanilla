#include <stdio.h>
#include "Original.h"

BOOL IMEUIRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wc;
	//
	// register class of UI window.
	//
	hInst = hInstance;
	hUIFont = CreateFont(16, 0, 0, 0, FW_NORMAL,
			FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH, _T("·s²Ó©úÅé"));
	//
	// register class of status window.
	//
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_OVIME | CS_IME;
	wc.lpfnWndProc    = StatusWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = UICHILDEXTRASIZE;
	wc.hInstance      = hInstance;
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = UISTATUSCLASSNAME;
	wc.hbrBackground  = NULL;
	wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;
	//
	// register class of composition window.
	//
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_OVIME | CS_IME;
	wc.lpfnWndProc    = CompWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = UICHILDEXTRASIZE;
	wc.hInstance      = hInstance;
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = UICOMPCLASSNAME;
	wc.hbrBackground  = NULL;
	wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	//
	// register class of candadate window.
	//
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_OVIME | CS_IME;
	wc.lpfnWndProc    = CandWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = UICHILDEXTRASIZE;
	wc.hInstance      = hInstance;
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = UICANDCLASSNAME;
	wc.hbrBackground  = NULL;
	wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	return TRUE;
}

BOOL IMEUIUnRegisterClass( HINSTANCE hInstance )
{
	UnregisterClass(UISTATUSCLASSNAME, hInstance);
	UnregisterClass(UICOMPCLASSNAME, hInstance);
	UnregisterClass(UICANDCLASSNAME, hInstance);
	for(int i = 0; i < IC.size(); i++)
		free(IC.at(i));
	CurrentIC = 0;
	return TRUE;
}

void UIPushInputMethod( LPTSTR lpStr )
{
	IC.push_back(_tcsdup(lpStr));
}

int UICurrentInputMethod()
{
	return CurrentIC;
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

					/*
					if(fIsCompWnd) {
						HWND hUIWnd;
						LPARAM mylParam;

						*((LPWORD)(&mylParam)) = (WORD)(pt.x - ptdif.x);
						*((LPWORD)(&mylParam)+1) = (WORD)(pt.y - ptdif.y);

						hUIWnd = (HWND)GetWindowLong(hWnd, FIGWL_SVRWND);
						if (IsWindow(hUIWnd))
							SendMessage(hUIWnd, WM_UI_COMPMOVE, 0, mylParam);
					}
					*/

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

