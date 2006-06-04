#include "OVIME2.h"
#include ".\statuswnd.h"
#include "DrawUtil.h"
#include "resource.h"

#include "CompStr.h"
#include "IMCLock.h"
#include "IMEUI.h"
#include "IMEUILock.h"

#include <commctrl.h>

#define ID_IME_LIST_FIRST               20
#define ID_IME_LIST_LAST                100
#define ID_CHANGE_IME                   101
// #define ID_CONFIG                       102
#define ID_IME_HELP                     103
// #define ID_CHI_ENG                      106
// #define ID_FULL_HALF                    107


BOOL AboutDlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if( msg == WM_COMMAND )
	{
		EndDialog( hwnd, LOWORD(wp) );
		return TRUE;
	}
	return FALSE;
}

StatusWnd::StatusWnd() : dragging(false)
{
}

StatusWnd::~StatusWnd(void)
{
//	DestroyIcon( iconChi );
//	DestroyIcon( iconEng );
//	DestroyIcon( iconFull );
//	DestroyIcon( iconHalf );
//	DestroyIcon( iconConfig );
}

BOOL StatusWnd::registerClass(void)
{
	WNDCLASSEX wc;
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_IME;
	wc.lpfnWndProc    = (WNDPROC)StatusWnd::wndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = g_dllInst;
	wc.hCursor        = LoadCursor( NULL, IDC_SIZEALL );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = g_statusWndClass;
	wc.hbrBackground  = HBRUSH(COLOR_BTNFACE+1);
	wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	return TRUE;
}

LRESULT StatusWnd::wndProc( HWND hwnd, UINT msg, WPARAM wp , LPARAM lp )
{
	IMEUILock lock( GetParent(hwnd) );
	IMEUI* ui = lock.getIMEUI();
	if( ui && ui->statusWnd.getHwnd() )
		return ui->statusWnd.wndProc( msg, wp, lp );
	return DefWindowProc(hwnd, msg, wp, lp);
}

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

TBBUTTON toolbar_btns[]={
	{0, ID_CHANGE_IME, TBSTATE_ENABLED ,TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, {0}, 0, 0 }, 
	{1, ID_CONFIG, TBSTATE_ENABLED ,TBSTYLE_BUTTON|TBSTYLE_AUTOSIZE, {0}, 0, 0 },
	{2, ID_CHI_ENG, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 },
	{5, ID_FULL_HALF, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 },
	{6, ID_IME_HELP, TBSTATE_ENABLED ,TBSTYLE_BUTTON, {0}, 0, 0 }
};

bool StatusWnd::create(HWND imeUIWnd)
{
	HIMC hIMC = (HIMC)GetWindowLong( imeUIWnd, IMMGWL_IMC );
	IMCLock imc(hIMC);

	hwnd = CreateWindowEx(0, g_statusWndClass, NULL,
					WS_POPUP|WS_CLIPCHILDREN,
					0, 0, 0, 0, imeUIWnd, NULL, g_dllInst, NULL);
	if( !hwnd )
		return false;

	// Initialize common controls
	static bool init_common_controls = false;
	if( !init_common_controls ){
		INITCOMMONCONTROLSEX iccex;
		iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccex.dwICC = ICC_BAR_CLASSES;
		InitCommonControlsEx(&iccex);
		init_common_controls = true;
	}

	g_client->getModList( modlist );

	toolbar = CreateToolbarEx( hwnd, 
		TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|TBSTYLE_LIST|CCS_NODIVIDER|CCS_NORESIZE|
		WS_CHILD|WS_VISIBLE|CCS_NOPARENTALIGN, 
		10, 3, g_dllInst, 0, 
		toolbar_btns, sizeof(toolbar_btns)/sizeof(TBBUTTON), 
		16, 16, 16, 16, sizeof(TBBUTTON));

//		toolbar = CreateWindowEx( 0, TOOLBARCLASSNAME, NULL, CCS_NOPARENTALIGN|WS_CHILD|WS_VISIBLE, 
//			100, 1, 20, 24, uiStatus.hWnd, NULL, hInstDLL, NULL);

	HIMAGELIST himl = ImageList_Create( 16, 16, ILC_COLOR24|ILC_MASK, 7, 0);
	HBITMAP htbbmp = LoadBitmap( g_dllInst, LPCTSTR(IDB_STATUS_TB) );
	ImageList_RemoveAll(himl);
	ImageList_AddMasked( himl, htbbmp, RGB(192, 192, 192) );
	DeleteObject(htbbmp);
	himl = (HIMAGELIST)SendMessage( toolbar, TB_SETIMAGELIST, 0, LPARAM(himl));
	if( himl )
		ImageList_Destroy( himl );

	TBBUTTONINFO tbi;	tbi.cbSize = sizeof(tbi);
	tbi.dwMask = TBIF_TEXT;		tbi.pszText = "TEST";//IC.at(CurrentIC);
	SendMessage( toolbar, TB_SETBUTTONINFO, ID_CHANGE_IME, LPARAM(&tbi));

	SIZE sz;
	GetToolbarSize( toolbar, &sz );

	sz.cx += 14;
	sz.cy += 4;
	if( sz.cy < 26 )
		sz.cy = 26;
	SetWindowPos( hwnd, NULL, 0, 0, sz.cx, sz.cy, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER );

/*
	TCHAR bmppath[MAX_PATH];
	GetSystemDirectory( bmppath, MAX_PATH );
	_tcscat( bmppath, _T("\\IME\\Chewing\\statuswnd.bmp") );
	HBITMAP tbbmp = (HBITMAP)LoadImage( NULL, bmppath, IMAGE_BITMAP, 
		0, 0, LR_DEFAULTCOLOR|LR_LOADFROMFILE);

	this->setTheme(tbbmp);
	iconChi = (HICON)LoadImage( g_dllInst, LPCTSTR (IDI_CHI), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR );
	iconEng = (HICON)LoadImage( g_dllInst, LPCTSTR (IDI_ENG), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR  );
	iconFull = (HICON)LoadImage( g_dllInst, LPCTSTR (IDI_FULL), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR  );
	iconHalf = (HICON)LoadImage( g_dllInst, LPCTSTR (IDI_HALF), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR  );
	iconConfig = (HICON)LoadImage( g_dllInst, LPCTSTR (IDI_CONFIG), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR  );

	TCHAR tip[128];
	LoadString(g_dllInst, ID_CHI_ENG, tip, sizeof(tip) );
	addBtn( ID_CHI_ENG, imc.isChinese() ? iconChi : iconEng, tip );
	LoadString(g_dllInst, ID_FULL_HALF, tip, sizeof(tip) );
	addBtn( ID_FULL_HALF, imc.isFullShape() ? iconFull : iconHalf, tip );
	LoadString(g_dllInst, ID_MENU, tip, sizeof(tip) );
	addBtn( ID_MENU, iconConfig, tip );
*/

	IMEUILock ui(GetParent(hwnd));
//	setTooltip( &ui.getIMEUI()->tooltip );
//	setCmdTarget(hwnd);

//	int w, h;
//	XPToolbar::getSize(&w, &h);
//	SetWindowPos( hwnd, NULL, 0, 0, w, h, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER );

	return true;
}

void StatusWnd::enableChiEng(bool enable)
{
//	SendMessage( GetDlgItem( hwnd, IDC_STATUS_TB), TB_ENABLEBUTTON, ID_CHI_ENG, enable );
}

void StatusWnd::toggleChiEngMode(HIMC hIMC)
{
	ToggleChiEngMode(hIMC);
/// FIXME: icons should indicate whether capslock is toggled or not.
	updateIcons(hIMC);
}

void StatusWnd::toggleShapeMode(HIMC hIMC)
{
	ToggleFullShapeMode( hIMC );
	updateIcons(hIMC);
}

void StatusWnd::updateIcons(HIMC hIMC)
{
	IMCLock imc(hIMC);
	bool isChinese;
		isChinese = imc.isChinese();
/*
	this->setBtnImage( 0, isChinese ? iconChi : iconEng );
	this->setBtnImage( 1, imc.isFullShape() ? iconFull : iconHalf );
*/
}

LRESULT StatusWnd::wndProc(UINT msg, WPARAM wp, LPARAM lp)
{
	HIMC hIMC = getIMC(hwnd);
	IMCLock imc( hIMC );

	switch (msg)
	{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint( hwnd, &ps );

				RECT rc;
				GetClientRect(hwnd, &rc);

				// FillSolidRect( hDC, &rc, GetSysColor(COLOR_BTNFACE));

				Draw3DBorder( ps.hdc, &rc, GetSysColor(COLOR_BTNHILIGHT), 0/*GetSysColor(COLOR_BTNSHADOW)*/);

				InflateRect( &rc, -3, -3 );
				rc.left++;
				rc.right = rc.left + 3;
				Draw3DBorder( ps.hdc, &rc, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
				OffsetRect( &rc, 4, 0);
				Draw3DBorder( ps.hdc, &rc, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));

				EndPaint(hwnd, &ps);
				break;
			}
		case WM_COMMAND:
			onCommand( LOWORD(wp), hIMC );
			break;
		case WM_SIZE:
			{
				int cx = LOWORD(lp);
				int cy = HIWORD(lp);
				MoveWindow( toolbar, 12, 2, cx-13, cy-4, TRUE);
			}
			break;
/*		case WM_NOTIFY:
			{
				switch( ((NMHDR*)lp)->code ) 
				{
				case TTN_GETDISPINFO:
					{ 
						LPTOOLTIPTEXT pttt;
						pttt = (LPTOOLTIPTEXT)lp;
						pttt->hinst = g_dllInst;
						pttt->lpszText = MAKEINTRESOURCE(pttt->hdr.idFrom);
						break;
					}
				}
			}
			break;
*/		case WM_LBUTTONDOWN:
			dragging = true;
			IMEChildWnd::onLButtonDown(wp, lp);
			break;
		case WM_MOUSEMOVE:
			if( dragging )
				IMEChildWnd::onMouseMove(wp, lp);
			break;
		case WM_LBUTTONUP:
			if( dragging )
			{
				dragging = false;
				IMEChildWnd::onLButtonUp(wp, lp);
			}
			break;
		case WM_MOUSEACTIVATE:
			return MA_NOACTIVATE;
		case WM_SHOWWINDOW:
			if( ! wp )
			{
				RECT rc;
				GetWindowRect( hwnd, &rc );
				INPUTCONTEXT* ic = imc.getIC();
				if( ic )
				{
					ic->ptStatusWndPos.x = rc.left;
					ic->ptStatusWndPos.y = rc.top;
				}
			}
			break;
		case WM_DESTROY:
			{
				RECT rc;
				GetWindowRect( hwnd, &rc );
				INPUTCONTEXT* ic = imc.getIC();
				if( ic )
				{
					ic->ptStatusWndPos.x = rc.left;
					ic->ptStatusWndPos.y = rc.top;
				}
				modlist.clear();
//				DeleteObject(tbbmp);
			}
			break;
//		case WM_CAPTURECHANGED:
//			onCaptureChanged(lp);
//			break;

		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}

void StatusWnd::getSize(int *w, int *h)
{
//	XPToolbar::getSize(w, h);
	SIZE sz;
	GetToolbarSize( toolbar, &sz );

	*w = sz.cx + 14;
	*h = sz.cy + 4;
	if( *h < 26 )
		*h = 26;
}

void StatusWnd::onCommand(UINT id, HIMC hIMC)
{
	switch( id )
	{
	case ID_CHI_ENG:
		toggleChiEngMode(hIMC);
		break;
	case ID_FULL_HALF:
		toggleShapeMode(hIMC);
		break;
	case ID_MENU:
		{
			HMENU menu = LoadMenu( g_dllInst, LPCTSTR(IDR_POPUP));
			HMENU popup = GetSubMenu( menu, 0 );
			RECT rc;
			GetWindowRect(hwnd, &rc);
			rc.left += 9;
			UINT flag;
			long y;
			if((rc.top * 2) > GetSystemMetrics(SM_CYSCREEN) )
			{
				y = rc.top;
				flag = TPM_BOTTOMALIGN|TPM_LEFTALIGN;
			}
			else
			{
				y = rc.bottom;
				flag = TPM_TOPALIGN|TPM_LEFTALIGN;
			}
			TrackPopupMenu( popup, flag, rc.left, y, 0, hwnd, NULL);
			DestroyMenu(menu);
			break;
		}
	case ID_CONFIG:
	case ID_ABOUT:
		try {
			HWND top = hwnd;
			HWND desktop = GetDesktopWindow(), parent = NULL;
			while( (parent=GetParent(top)) != desktop && parent )
				top = parent;
			EnableWindow( hwnd, FALSE );
			if( id == ID_CONFIG ) {
//				ConfigureChewingIME(top);
			}
			else
				DialogBox(g_dllInst, LPCTSTR(IDD_ABOUT), top, (DLGPROC)AboutDlgProc);
			EnableWindow( hwnd, TRUE );
		}   catch (...)
        {
            OutputDebugString(_T("Using die wnd"));
        }
		break;
	case ID_WEBSITE:
		ShellExecuteA( NULL, "open", "http://chewing.csie.net/", NULL, NULL, SW_SHOWNORMAL );
		break;
	case ID_BUGREPORT:
		ShellExecuteA( NULL, "open", "http://rt.openfoundry.org/Foundry/Project/Tracker/?Queue=271", NULL, NULL, SW_SHOWNORMAL );
		break;
	case ID_DICT_BUGREPORT:
		ShellExecuteA( NULL, "open", "http://rt.openfoundry.org/Foundry/Project/Tracker/?Type=defect&Queue=436", NULL, NULL, SW_SHOWNORMAL );
		break;
/*	case ID_HASHED:
		{
			TCHAR path[MAX_PATH];
			GetSystemDirectory( path, MAX_PATH );
			_tcscat( path, _T("\\IME\\Chewing\\") );
			int showcmd;
			if( id == ID_CHEWING_HELP ) {
				_tcscat( path, _T("Chewing.chm"));
				showcmd = SW_SHOWMAXIMIZED;
			}
			else {
				_tcscat( path, _T("HashEd.exe"));
				showcmd = SW_SHOWNORMAL;
			}
			ShellExecute( NULL, _T("open"), path, NULL, NULL, showcmd );
			break;
		}
*/
	case ID_DICT:
	case ID_SIMPDICT:
	case ID_LITTLEDICT:
	case ID_PROVERBDICT:
		{
			const char* url[]={
			"http://140.111.34.46/dict/",
			"http://140.111.34.46/jdict/main/cover/main.htm",
			"http://140.111.1.43/",
			"http://140.111.34.46/chengyu/sort_pho.htm"};
			ShellExecuteA( NULL, "open", url[ id - ID_DICT ], NULL, NULL, SW_SHOWNORMAL );
			break;
		}

	}
}
