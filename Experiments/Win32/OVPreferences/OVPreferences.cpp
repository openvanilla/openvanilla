#include <wx/wx.h>

class OVPreferencesApp : public wxApp
{
public:
  virtual bool OnInit();
};

IMPLEMENT_APP(OVPreferencesApp)

bool OVPreferencesApp::OnInit()
{

  return true;
}

