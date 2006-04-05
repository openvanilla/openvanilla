#include ".\candwnd.h"
#include "IMCLock.h"
#include "OVIME2.h"
#include "DrawUtil.h"
#include "CandList.h"
#include "imm.h"
#include "IMCLock.h"
#include "IMEUI.h"
#include "IMEUILock.h"

#include "CompWnd.h"

#include <tchar.h>
#include <windows.h>


void CandWnd::UpdateFont()
{
/*
    if ( font_size==g_FontSize )
        return;
    
    font_size = g_FontSize;
    if ( font!=NULL )
        DeleteObject(font);
	font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	GetObject( font, sizeof(lf), &lf);
	lf.lfHeight = font_size;
	font = CreateFontIndirect( &lf );
*/
}

CandWnd::CandWnd()
{
    font_size = 0;
	candPerRow = 1;
    UpdateFont();
}

CandWnd::~CandWnd(void)
{
}


BOOL CandWnd::registerClass(void)
{
	WNDCLASSEX wc;
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_IME;
	wc.lpfnWndProc    = (WNDPROC)CandWnd::wndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = g_dllInst;
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = g_candWndClass;
	wc.hbrBackground  = NULL;
	wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	return TRUE;
}

LRESULT CandWnd::wndProc( HWND hwnd , UINT msg, WPARAM wp , LPARAM lp)
{
	HIMC hIMC = getIMC(hwnd);
	IMEUILock lock( GetParent( hwnd ) );
	IMEUI* ui = lock.getIMEUI();

	switch (msg)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint( hwnd, &ps );
				if(ui)
					ui->candWnd.onPaint(hIMC, ps);
				EndPaint(hwnd, &ps);
				break;
			}
		case WM_ERASEBKGND:
			return TRUE;
			break;
		case WM_LBUTTONDOWN:
			if(ui)
				ui->candWnd.onLButtonDown(wp, lp);
			break;
		case WM_MOUSEMOVE:
			if(ui)
				ui->candWnd.onMouseMove(wp, lp);
			break;
		case WM_LBUTTONUP:
			if(ui)
				ui->candWnd.onLButtonUp(wp, lp);
			break;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
		default:
			if (!IsImeMessage(msg))
				return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

static void _DecideCandStringcolor(DWORD &CandIndex, DWORD &CandBody, BOOL bCandPage)
{
#define RGB_CAND_CHAR_IDX       RGB(230, 20, 20)
#define RGB_CAND_CHAR_BODY      RGB(20, 20, 230)
#define RGB_CAND_BLACK          RGB(0, 0, 0)
/*
    CandIndex = CandBody = RGB_CAND_BLACK;
    if ( g_ColorCandWnd==true && bCandPage==FALSE )
    {
        CandIndex = RGB_CAND_CHAR_IDX;
        CandBody = RGB_CAND_CHAR_BODY;
    }
*/
}

void CandWnd::onPaint(HIMC hIMC, PAINTSTRUCT& ps)
{
	IMCLock imc( hIMC );
	CandList* candList = imc.getCandList();
	if( !candList )
		return;

	HDC hDC = ps.hdc;
	HFONT oldFont;
	RECT rc;

    UpdateFont();

	oldFont = (HFONT)SelectObject(hDC, font);

	GetClientRect(hwnd,&rc);

	SetTextColor( hDC, GetSysColor( COLOR_WINDOWTEXT ) );
	SetBkColor( hDC, GetSysColor( COLOR_WINDOW ) );

	int items_per_row =  candPerRow;

	RECT cand_rc;	cand_rc.left = 1;	cand_rc.top = 1;	cand_rc.right = 1;

	int pageEnd = candList->getPageStart() + candList->getPageSize();
	if( pageEnd > candList->getTotalCount() )
		pageEnd = candList->getTotalCount();
	int numCand = pageEnd - candList->getPageStart();
	int num = 0;

	int selkey_w = 0;
    DWORD   colorIdx, colorBody;
	SIZE candsz;
	GetTextExtentPoint32W(hDC, L"m.", 2, &candsz);
	selkey_w = candsz.cx;

	for( int i = candList->getPageStart(); i <= pageEnd; ++i )
	{
		++num;

		wchar_t cand[64];
		TCHAR selKey[5] = _T("1.");
		if( i < pageEnd )
		{
			LPCTSTR selKeys = _T("1234567890");
			selKey[0] = selKeys[(i - candList->getPageStart())];
			wcscpy( cand, candList->getCand(i) );
            _DecideCandStringcolor(colorIdx, colorBody, FALSE);
		}
		else
		{
			*selKey = 0;

			int page = 1 + candList->getPageStart() / candList->getPageSize();
			int totalPage = candList->getTotalCount() / candList->getPageSize();
			if( candList->getTotalCount() % candList->getPageSize() )
				++totalPage;
			swprintf( cand, L"  %d/%d", page, totalPage );
            _DecideCandStringcolor(colorIdx, colorBody, TRUE);
		}

		int len = wcslen( cand );
		GetTextExtentPoint32W(hDC, cand, len, &candsz);
		candsz.cx += 4;
		candsz.cy += 2;
		cand_rc.bottom = cand_rc.top + candsz.cy;

		if( *selKey )
		{
			cand_rc.right = cand_rc.left + selkey_w;
			cand_rc.bottom = cand_rc.top + candsz.cy;
			// Draw selKey
            SetTextColor( hDC, colorIdx);
			ExtTextOut( hDC, cand_rc.left+1, cand_rc.top, ETO_OPAQUE, &cand_rc, selKey, 2, NULL);
			cand_rc.left = cand_rc.right;
		}
		cand_rc.right = cand_rc.left + candsz.cx;
        SetTextColor( hDC, colorBody );
		ExtTextOutW( hDC, cand_rc.left, cand_rc.top, ETO_OPAQUE, &cand_rc, cand, 
			len, NULL);
//		if( g_cursorCandList && i == candList->getSelection() )
//			BitBlt(hDC, cand_rc.left, cand_rc.top, font_size * len, font_size, hDC, cand_rc.left, cand_rc.top, NOTSRCCOPY);

		if( num >= items_per_row && i < pageEnd )
		{
			cand_rc.left = cand_rc.right;
			cand_rc.right = rc.right;
			ExtTextOut( hDC, cand_rc.left, cand_rc.top, ETO_OPAQUE, &cand_rc, NULL, 0, NULL);

			cand_rc.left = 1;
			cand_rc.top += candsz.cy;
			num = 0;
		}
		else
			cand_rc.left = cand_rc.right;

	}
	int rcleft = cand_rc.right;
	int rcright = rc.right;
	cand_rc.left = cand_rc.right;
	cand_rc.right = rc.right;
	ExtTextOut( hDC, cand_rc.left, cand_rc.top, ETO_OPAQUE, &cand_rc, NULL, 0, NULL);

	Draw3DBorder( hDC, &rc, GetSysColor(COLOR_3DFACE), 0);
	SelectObject( hDC, oldFont );
}

void CandWnd::getSize(int* w, int* h)
{
	*w = 0; *h = 0;
	HIMC hIMC = getIMC();
	if( !hIMC )
		return;
	INPUTCONTEXT* ic = ImmLockIMC(hIMC);
	if( !ic )
		return;
	CandList* candList = (CandList*)ImmLockIMCC(ic->hCandInfo);

	HDC hDC = GetDC(hwnd);
	HFONT oldFont;

	oldFont = (HFONT)SelectObject(hDC, font);

	int items_per_row =  candPerRow;

	int pageEnd = candList->getPageStart() + candList->getPageSize();
	if( pageEnd > candList->getTotalCount() )
		pageEnd = candList->getTotalCount();
	int numCand = pageEnd - candList->getPageStart();
	int num = 0;
	int width = 0, height = 0;
	int selkey_w = 0;
	SIZE candsz;
	GetTextExtentPoint32A(hDC, "m.", 2, &candsz);
	selkey_w = candsz.cx;
	for( int i = candList->getPageStart(); i <= pageEnd; ++i )
	{
		++num;
		wchar_t cand[64];
		if( i < pageEnd )
			swprintf ( cand, L"%s", candList->getCand(i) );
		else
		{
			int page = 1 + candList->getPageStart() / candList->getPageSize();
			int totalPage = candList->getTotalCount() / candList->getPageSize();
			if( candList->getTotalCount() % candList->getPageSize() )
				++totalPage;
			swprintf ( cand, L"%d/%d", page, totalPage );
		}

		int len = wcslen( cand );
		GetTextExtentPoint32W(hDC, cand, len, &candsz);
		width += candsz.cx + 4;
		width += selkey_w;

		if( candsz.cy > height )
			height = candsz.cy;
        if( num >= items_per_row && i <= pageEnd )
		{
			if( width > *w )
				*w = width;
			*h += height + 2;
			num = 0;
			width = 0;
		}
	}
	if( width > *w )
		*w = width;
	if( num > 0 && num < items_per_row )
		*h += height + 2;

	SelectObject(hDC, oldFont );
// End paint

	ImmUnlockIMCC(ic->hCandInfo);
	ImmUnlockIMC(hIMC);

	ReleaseDC(hwnd, hDC);
	*w += 2;
	*h += 2;
}

void CandWnd::updateSize(void)
{
	int w, h;
	getSize(&w, &h);
	SetWindowPos( hwnd, NULL, 0, 0, w, h, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
}

bool CandWnd::create(HWND imeUIWnd)
{
	hwnd = CreateWindowEx(0, g_candWndClass, NULL,
					WS_POPUP|WS_CLIPCHILDREN,
					0, 0, 0, 0, imeUIWnd, NULL, g_dllInst, NULL);
	return !!hwnd;
}

#if 0
void CandWnd::show(void)
{
	HIMC hIMC = getIMC();
	if( !hIMC )
		return;
	IMCLock imc(hIMC);

	setFont( font );
	updateSize();

	if( !imc.getIC() )
		return;

	POINT pt = imc.getIC()->cfCandForm[0].ptCurrentPos;
	DWORD style = imc.getIC()->cfCandForm[0].dwStyle;
	if( g_fixCompWnd )
		style = CFS_DEFAULT;

	switch( style )
	{
	case CFS_CANDIDATEPOS:
		break;
	case CFS_EXCLUDE:
		{
			RECT rc = imc.getIC()->cfCandForm[0].rcArea;
			RECT &area = imc.getIC()->cfCandForm[0].rcArea;
			RECT crc, intersect_rc;
			GetWindowRect(hwnd, &crc);
			int w = crc.right - crc.left;
			int h = crc.bottom - crc.top;
			crc.left = pt.x;	crc.top = pt.y;
			crc.right = crc.left + w;
			crc.bottom = crc.top + h;
			InflateRect( &rc, 1, 1 );

			RECT wrc;
			IMEUI::getWorkingArea(&wrc, imc.getIC()->hWnd);
			MapWindowPoints( HWND_DESKTOP, imc.getIC()->hWnd, (LPPOINT)&wrc, 2 );

			if( IntersectRect( &intersect_rc, &rc, &crc ) )
			{
				if( imc.isVerticalComp() )
				{
					if( (pt.x = area.left - w) < wrc.left )
						pt.x = area.right;
				}
				else
				{
					if( (pt.y = area.bottom) > wrc.bottom )
						pt.y = area.top - h;
				}
			}
			break;
		}
	case CFS_DEFAULT:
	default:
		{
			IMEUILock lock( GetParent(hwnd) );
			IMEUI* ui = lock.getIMEUI();
			if( ! ui )
				break;
			if( g_fixCompWnd )
			{
				ui->compWnd.getRelativeCandPos(imc, &pt);
				ClientToScreen(ui->compWnd.getHwnd(), &pt);
			}
			else
			{
				pt = ui->getCompWndPos(imc);
				ui->compWnd.getCandPos(imc, &pt);
			}
		}
	}

	if( ! g_fixCompWnd )
		ClientToScreen( imc.getIC()->hWnd, &pt );

	move( pt.x, pt.y );
	IMEChildWnd::show();
}

#endif
