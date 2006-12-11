#define OV_DEBUG
#include <stdio.h>
#include "PCMan.h"
#include "DotNETHeader.h"
#include "resource.h"
#include "AVLoader.h"

#pragma managed
#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
using namespace System;
using namespace System::Reflection;
using namespace System::Windows::Forms;

void CSharpWidgetDemo() {	
	try {
		Assembly* exAsm
			= Reflection::Assembly::LoadFile("C:\\WINDOWS\\OpenVanilla\\CSharpFormLibrary.dll");
		Form* exForm
			= dynamic_cast<Form*>(exAsm->CreateInstance("CSharpFormLibrary.DemoForm", true));
		exForm->ShowDialog();
		/*
		DemoForm __gc *bar = __gc new DemoForm();
		bar->ShowDialog();
		*/
	} catch (Exception* e) {
		Console::WriteLine(e->Message);
	}
}

#pragma unmanaged
//	SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
//  This standard toolbar message provided by Windows has some known bugs.
//  So I implemented a new function myself to prevent the problem.
void GetToolbarSize(HWND toolbar, SIZE *sz)
{
	sz->cx = sz->cy = 0;
	int c = (int)SendMessage( toolbar, TB_BUTTONCOUNT, 0, 0);
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
	RECT drc;
	DWORD        dwT;
	switch (msg)
	{
		case WM_PAINT:
			murmur("-----> WM_PAINT, status window");
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
			
			if(msg == WM_LBUTTONUP || msg == WM_RBUTTONUP) {
				dwT = GetWindowLong(hWnd,FIGWL_MOUSE);
				if (dwT & FIM_MOVED) {
					GetWindowRect(uiStatus.hWnd, &drc);
					UIMoveStatusWindow(hWnd, drc.left, drc.top);
				}
			}
			if ((msg == WM_SETCURSOR) &&
				(HIWORD(lParam) != WM_LBUTTONDOWN) &&
				(HIWORD(lParam) != WM_RBUTTONDOWN)) {
				murmur("StatusWndProc: WM_SETCURSOR");
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
			{
				murmur("StatusWndProc: WM_LBUTTONUP");
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
				if(msg == WM_RBUTTONUP) {
					CurrentIC++;
					if(CurrentIC > (int)IC.size() - 1)
						CurrentIC = 0;
					InvalidateRect(uiStatus.hWnd,NULL, FALSE);
				}
			}
			break;
		case WM_NOTIFY:
			{
				murmur("-----> WM_NOTIFY, status window");
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
				murmur("-----> WM_SIZE, status window");
				int cx = LOWORD(lParam);
				int cy = HIWORD(lParam);
				MoveWindow( hToolbar, 12, 2, cx-13, cy-4, TRUE);
			}
			break;
		case WM_COMMAND:

			murmur("-----> WM_COMMAND, status window");
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
					HIMC imc = ImmGetContext( hIMEWnd );
					if( imc )
					{
						DWORD conv, sentence;
						ImmGetConversionStatus( imc, &conv, &sentence);
						if( isChinese )
							conv |= IME_CMODE_NATIVE;
						else
							conv &= ~IME_CMODE_NATIVE;
						ImmSetConversionStatus( imc, conv, sentence);
					}
				}
				break;
			case ID_FULL_HALF:
				{
					isFull = !isFull;
					SendMessage( hToolbar, TB_CHANGEBITMAP, ID_FULL_HALF, MAKELPARAM(isFull ? 4 : 5, 0));
					HIMC imc = ImmGetContext( hIMEWnd );
					if( imc )
					{
						DWORD conv, sentence;
						ImmGetConversionStatus( imc, &conv, &sentence);
						if( isFull )
							conv |= IME_CMODE_FULLSHAPE;
						else
							conv &= ~IME_CMODE_FULLSHAPE;
						ImmSetConversionStatus( imc, conv, sentence);
					}
				}
				break;
			case ID_CONFIG:
				{
					TCHAR exe_path[MAX_PATH];
					TCHAR file_name[] = _T("\\OpenVanilla\\OVPreferences.exe");
				    GetWindowsDirectory(exe_path, MAX_PATH - (int)_tcslen( file_name ) );
				    _stprintf(exe_path, _T("%s\\%s"), exe_path, file_name);

					ShellExecute( NULL, _T("open"), exe_path, NULL, NULL, SW_SHOWNORMAL );
				}
				break;
			case ID_IME_HELP:
				CSharpWidgetDemo();				
				break;
			default:
				if( id >= ID_IME_LIST_FIRST && id < ID_IME_LIST_LAST )
				{
					id -= ID_IME_LIST_FIRST;
					CurrentIC = id;
					murmur("CurrentIC in ID_IME %d", CurrentIC);

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
					UIMoveStatusWindow(hWnd, rc.left, rc.top );
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
	//murmur("-----> UICreateStatusWindow");
	if (!IsWindow(uiStatus.hWnd))
	{
		uiStatus.hWnd = _CreateStatusPage();
		murmur("UICreateStatusWindow: hUIWnd -> %x", hUIWnd);
		_SetStatusAppHWnd(hUIWnd);
		//_MoveStatusPage(300,300);
		//_ShowStatusPage();
		hIMEWnd = hUIWnd;

//<comment author='b6s'>Jaimie is working on porting these actions to C#
/*
		SIZE sz;

		hIMEWnd = hUIWnd;

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

		hToolbar = CreateWindowEx( 0, TOOLBARCLASSNAME, NULL, CCS_NOPARENTALIGN|WS_CHILD|WS_VISIBLE, 
			100, 1, 20, 24, uiStatus.hWnd, NULL, hInstDLL, NULL);

		HIMAGELIST himl = ImageList_Create( 16, 16, ILC_COLOR24|ILC_MASK, 7, 0);
		HBITMAP htbbmp = LoadBitmap( hInstDLL, LPCTSTR(IDB_STATUS_TB) );
		ImageList_RemoveAll(himl);
		ImageList_AddMasked( himl, htbbmp, RGB(192, 192, 192) );
		DeleteObject(htbbmp);
		himl = (HIMAGELIST)SendMessage( hToolbar, TB_SETIMAGELIST, 0, LPARAM(himl));
		if( himl )
			ImageList_Destroy( himl );

		TBBUTTONINFO tbi;	tbi.cbSize = sizeof(tbi);
		tbi.dwMask = TBIF_TEXT;
		//<comment author='b6s'>Disable this global variable "IC" because of .NET 2.0's problem
		//tbi.pszText = IC.at(CurrentIC);
		//</comment>
		char modNameUTF8[1024];
		wchar_t modNameUCS2[1024];
		AVLoader* loader = AVLoader::getLoader();
		if(loader->moduleName(CurrentIC, modNameUTF8)) {
			MultiByteToWideChar(
				CP_UTF8, 0, modNameUTF8, (int)strlen(modNameUTF8)+1, modNameUCS2, 1024);
			tbi.pszText = modNameUCS2;
		}
		else {
			tbi.pszText = L"ERROR";
			murmur("loader->moduleName() failed.");
		}
		SendMessage( hToolbar, TB_SETBUTTONINFO, ID_CHANGE_IME, LPARAM(&tbi));

		SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
		GetToolbarSize( hToolbar, &sz );

		// for status movement
		uiStatus.sz.cx = sz.cx + 14;
		uiStatus.sz.cy = sz.cy + 4;
		if( uiStatus.sz.cy < 26 )
			uiStatus.sz.cy = 26;
		MoveWindow(uiStatus.hWnd,
				uiStatus.pt.x,
				uiStatus.pt.y,
				uiStatus.sz.cx,
				uiStatus.sz.cy,
				TRUE);
		ShowWindow(uiStatus.hWnd, SW_SHOWNOACTIVATE);
*/
//</comment>
	}
//	UIHideStatusWindow();
	char modNameUTF8[1024];
	wchar_t modNameUCS2[1024];
	
	AVLoader* loader = AVLoader::getLoader();
	wchar_t *modCurrentName;
	wchar_t *modMenuName;
	murmur("--->Current IC at create %d", CurrentIC);
	
	//jaimie for C# menu set module name
	//to set all available module names into C# menu list.
	UIClearStatusMenuModString();
	murmur ("module num=%d",loader->getInputMethodCount());
	for(int i = 0; i < loader->getInputMethodCount(); i++)
	{
		
		if(loader->moduleName(i, modNameUTF8))
		{
			MultiByteToWideChar(CP_UTF8, 0, modNameUTF8, (int)strlen(modNameUTF8)+1, modNameUCS2, 1024);
			//tbi.pszText = modNameUCS2;
			modMenuName = modNameUCS2;
			UISetStatusMenuModStr(modMenuName);
			murmur(" ---> modMenuName : %s", modNameUTF8);
		}
		else
			murmur(" ---> loader->moduleName failed!");
	}
	
	//to show the currentIC module on the status window
	if(loader->moduleName(CurrentIC, modNameUTF8)) 
	{
		MultiByteToWideChar(CP_UTF8, 0, modNameUTF8, (int)strlen(modNameUTF8)+1, modNameUCS2, 1024);
		//tbi.pszText = modNameUCS2;
		modCurrentName = modNameUCS2;
	}
	else 
	{
		//tbi.pszText = L"ERROR";
		modCurrentName = L"ERROR";
		murmur("loader->moduleName() failed.");
	}
	UISetStatusModStr(modCurrentName);
	return;
}

void UISetStatusModStr(wchar_t* lpStr)
{
	//lpCompStr = wcsdup(lpStr);
	std::wstring wsStatusStr(wcsdup(lpStr));
	//watch.start();
	_SetStatusModString(wsStatusStr);
	//watch.stop();
	//murmur("%1.3f sec:\tC# comp window, setstring", watch.getSec());

}

void UISetStatusMenuModStr(wchar_t* lpStr)
{
	std::wstring wsStatusStr(wcsdup(lpStr));
	//watch.start();
	_SetStatusMenuModString(wsStatusStr);
}

void UIClearStatusMenuModString()
{
	_ClearStatusMenuModString();
}

void UIMoveStatusWindow(HWND hUIWnd, int X, int Y)
{
	if (!IsWindow(uiStatus.hWnd))
		UICreateStatusWindow(hUIWnd);

	if (IsWindow(uiStatus.hWnd))
	{
		POINT pt;
		RECT screenrc;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;
		uiStatus.pt.x = X;
		uiStatus.pt.y = Y;
		sz.cx = uiStatus.sz.cx;
		sz.cy = uiStatus.sz.cy;
		pt.x = uiStatus.pt.x;
		pt.y = uiStatus.pt.y;

		SystemParametersInfo(SPI_GETWORKAREA, 0, &screenrc, 0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - 200;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - 50;		

		_MoveStatusPage(pt.x, pt.y);
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
	{
		_SetStatusChiEng(isChinese);
		_ShowStatusPage();

		//ShowWindow(uiStatus.hWnd, SW_SHOWNOACTIVATE);
	}
}

void UIHideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		//ShowWindow(uiStatus.hWnd, SW_HIDE);
		_HideStatusPage();
}

int CompIndexToXPos(int index);


void UISetCursorPos(int i)
{
//	int cursor = CompIndexToXPos( CompCursorPos );
//	RECT rc;
//	GetClientRect( uiComp.hWnd, &rc );
//	HDC dc = GetDC( uiComp.hWnd );
//	BitBlt( dc, cursor, 0, 1, rc.bottom, dc, cursor, 0, SRCINVERT );
	CompCursorPos = i;
//	cursor = CompIndexToXPos( CompCursorPos );
//	BitBlt( dc, cursor, 0, 1, rc.bottom, dc, cursor, 0, SRCINVERT );
}
#if 0
void UISetMarkFrom(int i)
{
//	murmur("-----> UISetMarkFrom, status window");
	CompSelStart = i;
	murmur( "---> UISetMarkFrom Start %d", CompSelStart);
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mf=%d"),  i);
	TextOut( tmpdc, 0, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
}

void UISetMarkTo(int i)
{
//	murmur("-----> UISetMarkTo, status window");
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mt=%d"),  i);
	TextOut( tmpdc, 40, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
	CompSelEnd = i;
	murmur( "---> UISetMarkFrom End %d", CompSelEnd);
}
#endif
void UIChangeModule(HWND hWnd)
{
    //TBBUTTONINFO tbi;	tbi.cbSize = sizeof(tbi);
	
	//CurrentIC++;
	//if(CurrentIC > (int)IC.size() - 1)
	//		CurrentIC = 0;
	
	//tbi.dwMask = TBIF_TEXT;		tbi.pszText = IC.at(CurrentIC);

	//SendMessage( hToolbar, TB_SETBUTTONINFO, ID_CHANGE_IME, LPARAM(&tbi));

	//SIZE sz;
//	SendMessage( hToolbar, TB_GETMAXSIZE, 0, LPARAM(&sz));
	//GetToolbarSize( hToolbar, &sz );

	//uiStatus.sz.cx = sz.cx + 14;
	//uiStatus.sz.cy = sz.cy + 4;

	char modNameUTF8[1024];
	wchar_t modNameUCS2[1024];
	
	AVLoader* loader = AVLoader::getLoader();
	wchar_t *modCurrentName;
	//murmur("---> current IC %d", CurrentIC);
	//murmur("---> loader->getInputMethodCount %d",loader->getInputMethodCount() );
	
	CurrentIC++;
	//module change to the next available module
	if(CurrentIC >= loader->getInputMethodCount())
		CurrentIC = 0;
	
	if(loader->moduleName(CurrentIC, modNameUTF8)) 
	{
		MultiByteToWideChar(CP_UTF8, 0, modNameUTF8, (int)strlen(modNameUTF8)+1, modNameUCS2, 1024);
		//tbi.pszText = modNameUCS2;
		modCurrentName = modNameUCS2;
		murmur(" ---> module name: %s", modNameUTF8);
		
	}
	else 
	{
		//tbi.pszText = L"ERROR";
		modCurrentName = L"ERROR";
		murmur("loader->moduleName() failed.");
	}
	UISetStatusModStr(modCurrentName);
	//murmur("UIStatus UIChangeModule %s",modCurrentName);
	//RECT rc;
	//GetWindowRect(uiStatus.hWnd, &rc);
	//UIMoveStatusWindow(hWnd, rc.left, rc.top );
	_ShowStatusPage();
}

void UIChangeModuleByMouse(HWND hWnd)
{
	char modNameUTF8[1024];
	wchar_t modNameUCS2[1024];
	
	AVLoader* loader = AVLoader::getLoader();
	wchar_t *modCurrentName;
	
	CurrentIC = _GetStatusSelectedModuleIndex();
	if(loader->moduleName(CurrentIC, modNameUTF8)) 
	{
		MultiByteToWideChar(CP_UTF8, 0, modNameUTF8, (int)strlen(modNameUTF8)+1, modNameUCS2, 1024);
		modCurrentName = modNameUCS2;
		murmur(" ---> module name: %s", modNameUTF8);		
	}
	else 
	{
		modCurrentName = L"ERROR";
		murmur("loader->moduleName() failed.");
	}
	UISetStatusModStr(modCurrentName);
	_ShowStatusPage();
}

void UIChangeHalfFull(HWND hWnd)
{
	isFull = !isFull;
	SendMessage( hToolbar, TB_CHANGEBITMAP, ID_FULL_HALF, MAKELPARAM(isFull ? 4 : 5, 0));
	HIMC imc = ImmGetContext( hIMEWnd );
	if( imc )
	{
		DWORD conv, sentence;
		ImmGetConversionStatus( imc, &conv, &sentence);
		if( isFull )
			conv |= IME_CMODE_FULLSHAPE;
		else
			conv &= ~IME_CMODE_FULLSHAPE;
			ImmSetConversionStatus( imc, conv, sentence);
	}
}

void UIChangeChiEng(HWND hWnd)
{
	isChinese = !isChinese;
	//SendMessage( hToolbar, TB_CHANGEBITMAP, ID_CHI_ENG, MAKELPARAM(isChinese ? 2 : 3, 0));
	UIShowStatusWindow();
	HIMC imc = ImmGetContext( hIMEWnd );
	if( imc )
	{
		DWORD conv, sentence;
		ImmGetConversionStatus( imc, &conv, &sentence);
		if( isChinese )
			conv |= IME_CMODE_NATIVE;
		else
			conv &= ~IME_CMODE_NATIVE;
		ImmSetConversionStatus( imc, conv, sentence);
	}
}
