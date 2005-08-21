#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVDisplayServer.h"
#include "OVLibrary.h"
#include "Utility.h"

#include <windows.h>


    #include <stdarg.h>
    #include <stdio.h>
    inline static void DebugLog(const char* format,...) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("f:\\opq.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("f:\\opq.txt", "a")) {
		va_list args;
		va_start (args, format);
		vfprintf (fp, format, args);
		va_end (args);
		fprintf (fp, "\n");
		fclose(fp);
	}
	}

#define OV_DEBUG
#define CS_OVIME (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)

class OVDCPCMan : public OVDisplayComponent
{
public:
	OVDCPCMan();
	virtual int initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath);
	virtual ~OVDCPCMan();
	virtual void update();
	virtual const char *identifier() { return "OVDCPCMan"; }

	static LRESULT APIENTRY CompWndProc(HWND hWnd,	UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateCompWindow();
	void PaintCompWindow();
private:
	HWND hCompWnd;

};

static OVDCPCMan* self;

OVDCPCMan::OVDCPCMan()
{
	self = this;
	DebugLog("PCMAN: %s", moduleType());	
}

void OVDCPCMan::update()
{
	DebugLog("PCMAN: %s", "Update");
	if(svr->isCandiShow() || svr->getCandiString() != "") {
		DebugLog("PCMAN: %s", svr->getCandiString().c_str());
		CreateCompWindow();
		MoveWindow(hCompWnd,
				400,
				400,
				100,
				50,
				TRUE);
		InvalidateRect(hCompWnd,NULL, FALSE);
		ShowWindow(hCompWnd, SW_SHOWNOACTIVATE);
		//wchar_t *decoded = UTF16toWCHAR(svr->getBufString().c_str());
		//MessageBoxW(NULL, decoded, L"Orz", MB_OK);
		//free(decoded);
	} else {
		ShowWindow(hCompWnd, SW_HIDE);
	}
}

int OVDCPCMan::initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath)
{
	DebugLog("PCMAN: %s", "Initialize");
	//DebugLog("PCMAN: hinst %x", svr->getHInst());
	UINT cs_dropshadow = 0;
	/*
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	if( GetVersionEx(&vi) && vi.dwMajorVersion >= 5 
		&& vi.dwMinorVersion != 0 )	// version >= Windows XP
		cs_dropshadow = CS_DROPSHADOW;
	*/

	WNDCLASSEX wc;
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_OVIME | CS_IME | cs_dropshadow;
	wc.lpfnWndProc    = CompWndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = NULL;//svr->getHInst();
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
	wc.hIcon          = NULL;
	wc.lpszMenuName   = (LPTSTR)NULL;
	wc.lpszClassName  = "COMPWND";
	wc.hbrBackground  = NULL;
	wc.hIconSm        = NULL;
	DebugLog("PCMAN: %s", "IntEnd");
	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return 0;
	
	return 1;
}

OVDCPCMan::~OVDCPCMan()
{
	UnregisterClass("COMPWND", NULL);//svr->getHInst());
}

LRESULT APIENTRY OVDCPCMan::CompWndProc(HWND hWnd,	UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_PAINT:
			self->PaintCompWindow();
			break;
		case WM_ERASEBKGND:
			return TRUE;
			break;
		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			/*
			DragUI(hWnd, NULL, msg, wParam, lParam, TRUE);
			if ((msg == WM_SETCURSOR) &&
					(HIWORD(lParam) != WM_LBUTTONDOWN) &&
					(HIWORD(lParam) != WM_RBUTTONDOWN))
				return DefWindowProc(hWnd, msg, wParam, lParam);
			if ((msg == WM_LBUTTONUP) || (msg == WM_RBUTTONUP))
				SetWindowLong(hWnd, FIGWL_MOUSE, 0L);
			break;
			*/
		default:
			//if (!MyIsIMEMessage(msg))
				return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
	}
	return 0;
}

void OVDCPCMan::CreateCompWindow()
{	
	if (!IsWindow(hCompWnd))
	{
		HDC hDC;
		HFONT oldFont;
		SIZE sz;
		wchar_t szStr[100];

		hCompWnd = 
			CreateWindowEx(0, "COMPWND", NULL,
					WS_DISABLED | WS_POPUP,
					0, 0, 1, 1, svr->getHWnd(), NULL, NULL, NULL);
		DebugLog("PCMAN: %s %x", "CreateWindow", hCompWnd);
		//SetWindowLong(hCompWnd, FIGWL_SVRWND, (DWORD)svr->getHWnd());

		wprintf(szStr, L"AAAAAAAAAAAAA");
		hDC = GetDC(hCompWnd);
		//oldFont = (HFONT)SelectObject(hDC, hUIFont);
		GetTextExtentPointW(hDC, szStr, wcslen(szStr), &sz);
		//SelectObject(hDC, oldFont);
		ReleaseDC(hCompWnd,hDC);
		ShowWindow(hCompWnd, SW_HIDE);
	}
}

void OVDCPCMan::PaintCompWindow()
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

	if(svr->getCandiString() != "")
	{
		DebugLog("Str: %s", svr->getCandiString().c_str());
		wchar_t *decoded = UTF16toWCHAR(svr->getCandiString().c_str());
		DebugLog("Str END");
		SetTextColor( memdc, GetSysColor( COLOR_WINDOW ) );
		SetBkColor( memdc, GetSysColor( COLOR_WINDOWTEXT ) );
		
		ExtTextOutW( memdc, 1, 1, ETO_OPAQUE, &rc, decoded, 
			wcslen(decoded), NULL);
		/*
		ExtTextOut( memdc, 1, 1, ETO_OPAQUE, &rc, svr->getBufString().c_str(),
			svr->getBufString().size(), NULL);
		*/
		free(decoded);
		//int selstart = CompIndexToXPos( CompSelStart );
		//int selend = CompIndexToXPos( CompSelEnd );
		//int cursor = CompIndexToXPos( CompCursorPos );
		//BitBlt( memdc, selstart, 0, selend-selstart, rc.bottom, memdc, selstart, 0, NOTSRCCOPY );
		//BitBlt( memdc, cursor, 0, 1, rc.bottom, memdc, cursor, 0, SRCINVERT );
	}

	InflateRect( &rc, 1, 1 );
	//Draw3DBorder( memdc, &rc, GetSysColor(COLOR_3DFACE), 0);

	BitBlt( hDC, ps.rcPaint.left, ps.rcPaint.top, 
		(ps.rcPaint.right-ps.rcPaint.left), 
		(ps.rcPaint.bottom-ps.rcPaint.top), 
		memdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

	SelectObject( memdc, oldbmp );
	DeleteObject( membmp );
	DeleteDC( memdc );

	EndPaint(hCompWnd,&ps);
}

// the module wrapper
OV_SINGLE_MODULE_WRAPPER(OVDCPCMan);
