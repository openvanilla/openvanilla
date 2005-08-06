#include <vector>
#include "OVIMEUI.h"
HFONT hUIFont;
HINSTANCE hInst;
HINSTANCE hInstDLL;
LPTSTR lpCandStr;
LPTSTR lpCompStr;
int CompCursorPos;
int CompSelStart;
int CompSelEnd;
UICHILD uiStatus;
UICHILD uiComp;
UICHILD uiCand;
HICON hMainIcon;
HWND hToolbar;
HMENU hIMESelMenu;
bool isChinese = true;
bool isFull = false;

// InputContext
std::vector<LPTSTR> IC;
int CurrentIC;
