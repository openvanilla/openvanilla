#include <wx/wx.h>
#include <wx/snglinst.h>

#include "ovprefdlg.h"


class OVPreferencesApp : public wxApp
{
public:
	virtual bool OnInit();
};

IMPLEMENT_APP(OVPreferencesApp)

static wxLocale g_Locale;
static wxSingleInstanceChecker g_SingleInstChecker;
bool OVPreferencesApp::OnInit()
{
	if( g_SingleInstChecker.Create( wxT("open_vanilla_settings:")+::wxGetUserId()) && g_SingleInstChecker.IsAnotherRunning() )
		return false;

	g_Locale.Init();
	g_Locale.AddCatalog( wxT("OVPreferences") );
	OVPrefDlg* dlg = new OVPrefDlg(NULL, -1, _("Open Vanilla Preferences"));
	dlg->ShowModal();
	dlg->Destroy();

	return false;
}

