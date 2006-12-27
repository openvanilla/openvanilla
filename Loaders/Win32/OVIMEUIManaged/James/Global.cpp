#include <vector>
#include "OVIMEUI.h"
HFONT hUIFont;
HINSTANCE hInst;
HINSTANCE hInstDLL;
wchar_t* lpCandStr;
int numCand;
wchar_t* lpCompStr;
wchar_t* lpNotifyStr;
int CompCursorPos;
int CompSelStart;
int CompSelEnd;
UICHILD uiStatus;
UICHILD uiComp;
UICHILD uiCand;
UICHILD uiNotify;
HICON hMainIcon;
HWND hIMEWnd;
HWND hToolbar;
HMENU hIMESelMenu;
bool isChinese = true;
bool isTraditional = true;
bool isFull = false;

// InputContext
std::vector<wchar_t*> IC;
int CurrentIC;
