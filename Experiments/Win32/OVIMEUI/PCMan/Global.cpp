#include <vector>
#include "OVIMEUI.h"
HFONT hUIFont;
HINSTANCE hInst;
HINSTANCE hInstDLL;
wchar_t* lpCandStr;
int numCand;
wchar_t* lpCompStr;
int CompCursorPos;
int CompSelStart;
int CompSelEnd;
UICHILD uiStatus;
UICHILD uiComp;
UICHILD uiCand;
HICON hMainIcon;
HWND hIMEWnd;
HWND hToolbar;
HMENU hIMESelMenu;
bool isChinese = true;
bool isFull = false;

// InputContext
std::vector<wchar_t*> IC;
int CurrentIC;
