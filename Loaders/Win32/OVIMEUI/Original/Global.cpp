#include <vector>
#include "OVIMEUI.h"
HFONT hUIFont;
HINSTANCE hInst;
LPTSTR lpCandStr;
LPTSTR lpCompStr;
UICHILD uiStatus;
UICHILD uiComp;
UICHILD uiCand;

// InputContext
std::vector<LPTSTR> IC;
int CurrentIC;
