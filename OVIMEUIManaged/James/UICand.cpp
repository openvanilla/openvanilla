#define OV_DEBUG

#include <stdio.h>
#include "PCMan.h"
#include "AVDictionary.h"
#include "AVConfig.h"
#include "DotNETHeader.h"

#define		ITEMS_PER_ROW	4

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
		//SIZE sz;	sz.cx = sz.cy = 0;

		/*<comment author='b6s'>
		// CreateWindowEx is not equivalent to SetParent.
		// Set WS_POPUP in each C# widget instead.
		uiCand.hWnd = 
			CreateWindowEx(0, UICANDCLASSNAME ,NULL,
					WS_DISABLED | WS_POPUP,
					0, 0, 1, 1, hUIWnd,NULL,hInst,NULL);
		
		SetWindowLong(uiCand.hWnd, FIGWL_SVRWND, (DWORD)hUIWnd);
		//changes an attribute of the specified window.The function also sets the 32-bit (long) value at the specified offset into the extra window memory.
		*/
		//</comment>

		Watch watch;
		watch.start();
		uiCand.hWnd = _CreateCandPage();
		watch.stop();
		murmur("%1.3f sec:\tC# candidate window, create", watch.getSec());

		//uiCand.sz.cx = sz.cx + 2;
		//uiCand.sz.cy = sz.cy + 4;
	}
	//ShowWindow(uiCand.hWnd, SW_HIDE);
	//UIHideCandWindow();	// by b6s
	
	return;
}

int CompIndexToXPos( int i );

BOOL GetCandPosFromCompWnd(LPSIZE lpsz)
{
	if (IsWindow(uiComp.hWnd))
	{
		RECT rc, screenrc;
		POINT pt;

		GetWindowRect(uiComp.hWnd, &rc);

		pt.x = rc.left;
		pt.y = rc.bottom + 2;

		pt.x += CompIndexToXPos( CompCursorPos );
		GetWindowRect(uiCand.hWnd, &rc);
		pt.x -= 16;
		if( pt.x < 0 )
			pt.x = 0;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);
		if( (pt.x + lpsz->cx) > screenrc.right)
			pt.x = screenrc.right - lpsz->cx - 5;//rc.left - lpsz->cx - 5;
		if( (pt.y + lpsz->cy) > screenrc.bottom)
			pt.y = screenrc.bottom - lpsz->cy;

		uiCand.pt.x = pt.x;
		uiCand.pt.y = pt.y;
		return TRUE;
	}
	return FALSE;
}

void UIMoveCandWindow(HWND hUIWnd, int X, int Y, wchar_t* lpStr)
{
	
//	free(lpCandStr);
//	numCand = 0;
//	lpCandStr = wcsdup(lpStr);
//	if( !*lpStr )
//	{
//		UIHideCandWindow();
//		return;
//	}
//
//	if (!IsWindow(uiCand.hWnd))
//		UICreateCandWindow(hUIWnd);
//
//	// FIXME: UIMoveCandWindow will be called twice almost at the same time.
//	// The first call cause some problems.
//	// This should be fixed in the future.
//	// It's impossible to have lpStr != NULL and lpCompStr ==NULL.
//	// Since there is no composition string, is candidate window needed?
////	if( !lpCompStr || !*lpCompStr )
////		return;
//
//	if (IsWindow(uiCand.hWnd))
//	{
//		HDC hDC;
//		HFONT oldFont;
//		SIZE sz;
//		SIZE candsz;
//
//		sz.cx = 0;
//		sz.cy = 0;
//
//		AVConfig cfg;
//		AVDictionary *dict = AVDictionary::getDict(cfg.getUserDir(), "OVIMEUI");
//		int items_per_row = dict->keyExist("items_per_row") ? dict->getInteger("items_per_row") : ITEMS_PER_ROW;
//
//		// No Cand
//		if(lpStr == NULL) {
//			ShowWindow(uiCand.hWnd, SW_HIDE);
//			return;
//		}
//		hDC = GetDC(uiCand.hWnd);
//		oldFont = (HFONT)SelectObject(hDC, hUIFont);
//
//		LPCTSTR cand = wcstok( lpCandStr, L" ");	// strtok, delimited by space
//
//		int num = 0;
//		int width = 0;
//		while( cand )
//		{
//			++numCand;
//			int len = (int)wcslen( cand );
//			GetTextExtentPoint32(hDC, cand, len, &candsz);
//			candsz.cx += 4;
//			candsz.cy += 2;
//			width += candsz.cx;
//			++num;
//			if( num >= items_per_row )
//			{
//				sz.cy += candsz.cy;
//				if( width > sz.cx )
//					sz.cx = width;
//				width = 0;
//				num = 0;
//			}
//			cand = wcstok( NULL, L" " );
//		}
//		if( width > sz.cx )
//			sz.cx = width;
//		if( num > 0 && num < items_per_row )
//			sz.cy += candsz.cy;
//
//		sz.cy += 2;
//		sz.cx += 2;
//
//		SelectObject(hDC, oldFont);
//		ReleaseDC(uiCand.hWnd,hDC);
//
//		if( X > 0 && Y > 0)
//		{
//			RECT screenrc;
//			SystemParametersInfo(SPI_GETWORKAREA,
//					0,
//					&screenrc,
//					0);
//			if( (X + sz.cx) > screenrc.right)
//				X = screenrc.right - sz.cx - 5;//rc.left - lpsz->cx - 5;
//			if( (Y + sz.cy) > screenrc.bottom)
//				Y = screenrc.bottom - sz.cy;
//			uiCand.pt.x = X;
//			uiCand.pt.y = Y;
//			MoveWindow(uiCand.hWnd,
//				uiCand.pt.x,
//				uiCand.pt.y,
//				sz.cx,
//				sz.cy,
//				TRUE);
//			//ShowWindow(uiCand.hWnd, SW_SHOWNOACTIVATE);
//			InvalidateRect(uiCand.hWnd, NULL, FALSE);
//		}
//		else if( GetCandPosFromCompWnd(&sz) )
//		{
//			MoveWindow(uiCand.hWnd,
//				uiCand.pt.x,
//				uiCand.pt.y,
//				sz.cx,
//				sz.cy,
//				TRUE);
//			//ShowWindow(uiCand.hWnd, SW_SHOWNOACTIVATE);
//			InvalidateRect(uiCand.hWnd, NULL, FALSE);
//		}
//	}
	//James
	free(lpCandStr);
	lpCandStr = wcsdup(lpStr);
	if(lpStr) {
		if( !*lpStr )
		{
			UIHideCandWindow();
			return;
		}
	}

	// FIXME: UIMoveCandWindow will be called twice almost at the same time.
	// The first call cause some problems.
	// This should be fixed in the future.
	// It's impossible to have lpStr != NULL and lpCompStr ==NULL.
	// Since there is no composition string, is candidate window needed?
	if( !lpCompStr || !*lpCompStr )
		return;

	if (!IsWindow(uiCand.hWnd))
		UICreateCandWindow(hUIWnd);

	if (IsWindow(uiCand.hWnd))
	{
		HDC hDC;
		HFONT oldFont;
		SIZE sz;
		SIZE candsz;

		sz.cx = 0;
		sz.cy = 0;

		AVConfig cfg;
		AVDictionary *dict = AVDictionary::getDict(cfg.getUserDir(), "OVIMEUI");
		int items_per_row = dict->keyExist("items_per_row") ? dict->getInteger("items_per_row") : ITEMS_PER_ROW;

		// No Cand
		if(lpStr == NULL) {
			//ShowWindow(uiCand.hWnd, SW_HIDE);
			UIHideCandWindow();
			return;
		}

		Watch watch;
		watch.start();
		std::wstring wsCandStr(lpCandStr);
		_SetCandString(wsCandStr);
		watch.stop();
		murmur("%1.3f sec:\tC# candidate window, set string", watch.getSec());
		murmur("set candidate string:%s", lpCandStr);
		//<comment author='b6s'>
		//UIShowCandWindow();
		//</comment>

		hDC = GetDC(uiCand.hWnd);
		oldFont = (HFONT)SelectObject(hDC, hUIFont);
		
		LPCTSTR cand = wcstok( lpCandStr, L" ");	// strtok, delimited by space

		int num = 0;
		int width = 0;
		numCand = 0;
		while( cand )
		{
			++numCand;
			int len = (int)wcslen( cand );
			GetTextExtentPoint32(hDC, cand, len, &candsz);
			candsz.cx += 4;
			candsz.cy += 2;
			width += candsz.cx;
			++num;
			if( num >= items_per_row )
			{
				sz.cy += candsz.cy;
				if( width > sz.cx )
					sz.cx = width;
				width = 0;
				num = 0;
			}
			cand = wcstok( NULL, L" " );
		}
		if( width > sz.cx )
			sz.cx = width;
		if( num > 0 && num < items_per_row )
			sz.cy += candsz.cy;

		sz.cy += 2;
		sz.cx += 2;

		SelectObject(hDC, oldFont);
		ReleaseDC(uiCand.hWnd,hDC);

		if( X > 0 && Y > 0)
		{
			RECT screenrc;
			SystemParametersInfo(SPI_GETWORKAREA,
					0,
					&screenrc,
					0);
			if( (X + sz.cx) > screenrc.right)
				X = screenrc.right - sz.cx - 5;//rc.left - lpsz->cx - 5;
			if( (Y + sz.cy) > screenrc.bottom)
				Y = screenrc.bottom - sz.cy;
			uiCand.pt.x = X;
			uiCand.pt.y = Y;
			/*MoveWindow(uiCand.hWnd,
				uiCand.pt.x,
				uiCand.pt.y,
				sz.cx,
				sz.cy,
				TRUE);*/
			Watch watch;
			watch.start();
			_MoveCandPage(uiCand.pt.x,uiCand.pt.y);
			watch.stop();
			murmur("%1.3f sec:\tC# candidate window, move", watch.getSec());
			murmur("uiCand.pt.x:%i, uiCand.pt.y:%i", uiCand.pt.x,uiCand.pt.y);
			//<comment author='b6s'>
			// Test
			//UIShowCandWindow();
			//</comment>
			//InvalidateRect(uiCand.hWnd, NULL, FALSE);
		}
		else if( GetCandPosFromCompWnd(&sz) )
		{
			/*MoveWindow(uiCand.hWnd,
				uiCand.pt.x,
				uiCand.pt.y,
				sz.cx,
				sz.cy,
				TRUE);*/

			Watch watch;
			watch.start();		
			_MoveCandPage(uiCand.pt.x,uiCand.pt.y);
			watch.stop();
			murmur("%1.3f sec:\tC# candidate window, move", watch.getSec());
			murmur("From CompWnd, uiCand.pt.x:%i, uiCand.pt.y:%i", uiCand.pt.x,uiCand.pt.y);
			//<comment author='b6s'>
			// Test
			//UIShowCandWindow();
			//</comment>
			//InvalidateRect(uiCand.hWnd, NULL, FALSE);
		}
	}
	
}

void PaintCandWindow(HWND hCandWnd)
{
//	PAINTSTRUCT ps;
//	HDC hDC;
//	HFONT oldFont;
//	RECT rc;
//	//DWORD i;
//
//	hDC = BeginPaint(hCandWnd,&ps);
//	oldFont = (HFONT)SelectObject(hDC, hUIFont);
//
//	GetClientRect(hCandWnd,&rc);
//
//	if(lpCandStr)
//	{
//		SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );
//		SetBkColor( hDC, GetSysColor( COLOR_WINDOW ) );
//
//		AVConfig cfg;
//		AVDictionary *dict = AVDictionary::getDict(cfg.getBaseDir(), "OVIMEUI");
//		int items_per_row = dict->keyExist("items_per_row") ? dict->getInteger("items_per_row") : ITEMS_PER_ROW;
//
//		RECT cand_rc;	cand_rc.left = 1;	cand_rc.top = 1;
//		LPCTSTR cand = lpCandStr;
//		//Array foo = cand.split(new char[]{' '});
//		int num = 0;
//		for( int i = 0; i < numCand; ++i )
//		{
//			++num;
//			int len = (int)wcslen( cand );
//			SIZE candsz;
//			GetTextExtentPoint32(hDC, cand, len, &candsz);
//			candsz.cx += 4;
//			candsz.cy += 2;
//
//			cand_rc.right = cand_rc.left + candsz.cx;
//			cand_rc.bottom = cand_rc.top + candsz.cy;
//
//			if( (i + 1) == numCand )
//				SetTextColor( hDC, RGB(0, 0, 192) );
//
//			ExtTextOut( hDC, cand_rc.left + 2, cand_rc.top, ETO_OPAQUE, &cand_rc, cand, 
//				len, NULL);
//
//			if( num >= items_per_row && (i + 1) < numCand )
//			{
//				cand_rc.left = 1;
//				cand_rc.top += candsz.cy;
//				num = 0;
//			}
//			else
//				cand_rc.left = cand_rc.right;
//			cand = cand + wcslen(cand) + 1;
//		}
//		cand_rc.left = cand_rc.right;
//		cand_rc.right = rc.right;
//		ExtTextOut( hDC, cand_rc.left, cand_rc.top, ETO_OPAQUE, &cand_rc, NULL, 0, NULL);
//	}
//	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_3DFACE), 0/*GetSysColor(COLOR_3DDKSHADOW)*/);
//	SelectObject(hDC, oldFont);
//	EndPaint(hCandWnd,&ps);
//	
	if(lpCandStr)
	{
		UIShowCandWindow(); //James comments
		return;
	}
}

void UIShowCandWindow()
{
	if (IsWindow(uiCand.hWnd)) {
		Watch watch;
		watch.start();
		_ShowCandPage();
		watch.stop();
		murmur("%1.3f sec:\tC# candidate window, show", watch.getSec());
	}
}

void UIHideCandWindow()
{
	if (IsWindow(uiCand.hWnd)) {
		Watch watch;
		watch.start();
		_HideCandPage();
		watch.stop();
		murmur("%1.3f sec:\tC# candidate window, hide", watch.getSec());

		watch.start();
		_ClearCandPage();
		watch.stop();
		murmur("%1.3f sec:\tC# candidate window, clear", watch.getSec());
	}
}
