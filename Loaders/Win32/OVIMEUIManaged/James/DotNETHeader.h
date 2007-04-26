#include<string>
#pragma managed
#include<vector>

//status
HWND _CreateStatusPage();
void _ShowStatusPage();
void _MoveStatusPage(int, int);
void _HideStatusPage();
void _SetStatusChiEng();
void _SetStatusSimpifiedOrTraditional(bool);
void _SetStatusModString(int);
void _SetStatusMenuModString(const wchar_t* menuModStr);
void _ClearStatusMenuModString();
void _SetStatusAppHWnd(HWND hwnd);
int _GetStatusSelectedModuleIndex();
void _RotateStatusSelectedModule();

//composition
HWND _CreateCompPage();
void _ShowCompPage();
int _GetHeight();
int _GetCaretPosX();
void _HideCompPage();
void _ClearCompPage();
void _MoveCompPage(int x, int y);
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
void _MoveCandiPage(int x,int y);
void _ClearCandiPage();
void _ExpandCandi();

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