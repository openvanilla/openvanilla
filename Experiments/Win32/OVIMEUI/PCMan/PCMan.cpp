#include <stdio.h>
#include "PCMan.h"
#include "resource.h"

#ifndef	CS_DROPSHADOW
	#define	CS_DROPSHADOW	0x00020000
#endif

BOOL IMEUIRegisterClass( HINSTANCE hInstance )
{
	UINT cs_dropshadow = 0;
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if( GetVersionEx(&vi) && vi.dwMajorVersion >= 5 
		&& vi.dwMinorVersion != 0 )	// version >= Windows XP
		cs_dropshadow = CS_DROPSHADOW;

	WNDCLASSEX wc;
	//
	// register class of UI window.
	//
	hInst = hInstance;

	hUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject( hUIFont, sizeof(lf), &lf);
	lf.lfHeight = 16;
	hUIFont = CreateFontIndirect( &lf );

	//
	// register class of status window.
	//
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_OVIME | CS_IME | cs_dropshadow;
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
	wc.style          = CS_OVIME | CS_IME | cs_dropshadow;
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
	wc.style          = CS_OVIME | CS_IME | cs_dropshadow;
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

void UIPushInputMethod( wchar_t *lpStr )
{
	IC.push_back(wcsdup(lpStr));
	AppendMenu( hIMESelMenu, MF_STRING, ID_IME_LIST_FIRST + IC.size() -1, lpStr );
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

void FillSolidRect( HDC dc, LPRECT rc, COLORREF color )
{
	SetBkColor( dc, color );
	::ExtTextOut(dc, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
}

void FillSolidRect( HDC dc, int l, int t, int w, int h, COLORREF color )
{
	RECT rc;
	rc.left = l;
	rc.top = t;
	rc.right = rc.left + w;
	rc.bottom = rc.top + h;
	SetBkColor( dc, color );
	::ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
}

void Draw3DBorder(HDC hdc, LPRECT rc, COLORREF light, COLORREF dark, int width)
{
	MoveToEx( hdc, rc->left, rc->bottom, NULL);

	HPEN light_pen = CreatePen( PS_SOLID|PS_INSIDEFRAME, width, light );
	HGDIOBJ oldPen = SelectObject( hdc, light_pen );
	LineTo( hdc, rc->left, rc->top );
	LineTo( hdc, rc->right-width, rc->top );
	SelectObject( hdc, oldPen );
	DeleteObject( light_pen );

	HPEN dark_pen = CreatePen( PS_SOLID|PS_INSIDEFRAME, width, dark );
	oldPen = SelectObject( hdc, dark_pen );
	LineTo( hdc, rc->right-width, rc->bottom-width );
	LineTo( hdc, rc->left, rc->bottom-width );
	DeleteObject( dark_pen );
	SelectObject( hdc, oldPen );
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

BOOL WINAPI DllMain( HINSTANCE hInst,  DWORD fdwReason,  LPVOID lpvReserved )
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
		INITCOMMONCONTROLSEX iccex;
		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccex.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&iccex);

		hInstDLL = hInst;
		DisableThreadLibraryCalls( HMODULE(hInstDLL) );
		hIMESelMenu = CreatePopupMenu();
		return TRUE;
		}
	case DLL_PROCESS_DETACH:
		DestroyMenu(hIMESelMenu);
		break;
	}
	return TRUE;
}
