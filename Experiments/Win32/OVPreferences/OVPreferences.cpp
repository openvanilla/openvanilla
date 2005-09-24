#include <wx/wx.h>
#include <wx/snglinst.h>

#include "ovprefdlg.h"
#include "OVIME.h"
#include "NetIO.h"

#if defined(WIN32)
	#include <windows.h>
#endif

class OVPreferencesApp : public wxApp
{
public:
	virtual bool OnInit();
};

IMPLEMENT_APP(OVPreferencesApp)

static wxLocale g_Locale;
#if defined(__WXMSW__)
	static BOOL CALLBACK EnumAllOVIMEWindows(HWND hwnd, LPARAM lp)
	{
		TCHAR tmp[8];
		GetClassName( hwnd, tmp, 8 );
		if( 0 == _tcscmp( tmp, wxT("OVIMEUI") ) )
			SendMessage( hwnd, WM_IME_RELOADCONFIG, 0, 0 );
		return TRUE;
	}
#else
	static wxSingleInstanceChecker g_SingleInstChecker;
#endif

bool OVPreferencesApp::OnInit()
{
	g_Locale.Init();
	g_Locale.AddCatalog( wxT("OVPreferences") );

	const wxChar* title = _("Open Vanilla Preferences");
#if defined(WIN32)
	HWND hwnd;
	if( ( hwnd = FindWindow( wxT("#32770"), title) ) )
	{
		BringWindowToTop(hwnd);
		SetForegroundWindow(hwnd);
		return false;
	}
#else
	if( g_SingleInstChecker.Create( wxT("open_vanilla_settings:")+::wxGetUserId()) && g_SingleInstChecker.IsAnotherRunning() )
		return false;
#endif

	OVPrefDlg* dlg = new OVPrefDlg(NULL, -1, title);
	dlg->ShowModal();
	dlg->Destroy();

	UDPSocket sock;
	sock.send("127.0.0.1", 5100, RELOADCONFIG, "");
#if defined(WIN32)
	// Force all OVIMEs to reload
	EnumChildWindows( GetDesktopWindow(), EnumAllOVIMEWindows, NULL);
#endif
	return false;
}

