#include<string>

//#pragma managed
#include<vector>


//status
HWND _CreateStatusPage();
void _ShowStatusPage();
void _MoveStatusPage(int, int);
void _DockStatusPage(int x,int y);
void _HideStatusPage();
void _SetStatusChiEng();
void _SetStatusHalfFull();
bool _GetItemChecked(int index);
void _SetItemChecked(int index, bool c);
void _SetStatusSimpifiedOrTraditional(bool);
void _SetStatusModString(int);
void _SetStatusMenuModString(const wchar_t* menuModStr);
void _ClearStatusMenuModString();
void _SetStatusAppHWnd(HWND hwnd);
void _setStatuskpCount(int count);
void _setStatusofCount(int count);
void _setStatusimCount(int count);
int _GetStatusSelectedModuleIndex();
bool _GetIsStatusDocked() ;
void _SetIsStatusDocked(bool t) ;
void _RotateStatusSelectedModule();

//composition
HWND _CreateCompPage();
void _ShowCompPage();
int _GetHeight();
int _GetCaretPosX();
void _HideCompPage();
void _ClearCompPage();
void _MoveCompPage(
	int x, int y,
	int fontHeight, const wchar_t* fontName);
void _SetCompString(const wchar_t* compStr);
void _SetCompCaretPosX(int);
void _SetCompMarkFrom(int);
void _SetCompMarkTo(int);
void _SetCompAppHWnd(HWND);

//candidate
void _SetCandiString(const wchar_t* candiStr);
void _EndCandiPage();
HWND _CreateCandiPage();
HWND _CreateCandiPageWithHandle(HWND hwnd);
void _HideCandiPage();
void _ShowCandiPage();
void _MoveCandiPage(int x, int y, int compHeight);
void _ClearCandiPage();
void _ExpandCandi();
void _SelectNextItem(int i);
int _GetSelectedItemKey();
int _GetCandWidth();
int _GetCandHeight();
void _SetCandiFont(const wchar_t* ff, int fontsize);
void _SetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor);
void _SetCandiOpacity(int o);

//Symbol
void _SetSymbolString(const wchar_t* symbolStr);
void _EndSymbolPage();
HWND _CreateSymbolPage();
HWND _CreateSymbolPageWithHandle(HWND hwnd);
void _HideSymbolPage();
void _ShowSymbolPage();
void _MoveSymbolPage(int x,int y);
void _ClearSymbolPage();
void _ExpandSymbol();


//notify
void _ShowNotifyPageArray(const std::vector<std::wstring>& candidates);
void _SetNotifyString(const wchar_t* notifyStr);
void _EndNotifyPage();
int _GetNotifyValue();
HWND _CreateNotifyPage();
HWND _CreateNotifyWithHandle(HWND hwnd);
void _HideNotifyPage();
void _ShowNotifyPage();
void _MoveNotifyPage(int x,int y);
void _ClearNotifyPage();
void _ExpandNotify();
int _GetNotifyWidth();
int _GetNotifyHeight();
void _SetNotifyFont(const wchar_t* ff, int fontsize);
void _SetNotifyColor(int forecolor, int backcolor);
void _SetNotifyEnabled(bool b);
void _SetNotifyOpacity(int o);



// move from pcman.h
// Private variable
extern HFONT hUIFont;
extern HICON hMainIcon;
extern HWND hIMEWnd;
extern HWND hToolbar;
extern HMENU hIMESelMenu;
extern HINSTANCE hInst;
extern HINSTANCE hInstDLL;
extern wchar_t* lpCandStr;
extern int numCand;
extern wchar_t* lpCompStr;
extern wchar_t* lpNotifyStr;
extern int CompCursorPos;
extern int CompSelStart;
extern int CompSelEnd;
extern UICHILD uiCand;
extern UICHILD uiComp;
extern UICHILD uiStatus;
extern UICHILD uiNotify;
//extern std::vector<wchar_t*> IC;
extern int CurrentIC;
extern bool isChinese;
extern bool isTraditional;
extern bool isFull;
