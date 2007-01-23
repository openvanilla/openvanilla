#include<string>
#pragma managed
#include<vector>

//status
HWND _CreateStatusPage();
void _ShowStatusPage();
void _MoveStatusPage(int, int);
void _HideStatusPage();
void _SetStatusChiEng(bool);
void _SetStatusSimpifiedOrTraditional(bool);
void _SetStatusModString(int);
void _SetStatusMenuModString(const std::wstring&);
void _ClearStatusMenuModString();
void _SetStatusAppHWnd(HWND hwnd);
int _GetStatusSelectedModuleIndex();
void _SetUserDir();
void _RotateStatusSelectedModule();

//composition
HWND _CreateCompPage();
void _ShowCompPage();
int _GetHeight();
int _GetCaretPosX();
void _HideCompPage();
void _ClearCompPage();
void _MoveCompPage(int x, int y);
void _SetCompString(const std::wstring&);
void _SetCompCaretPosX(int);
void _SetCompMarkFrom(int);
void _SetCompMarkTo(int);

//candidate
void _ShowCandPageArray(const std::vector<std::wstring>& candidates);
void _SetCandString(const std::wstring& candidate);
void _EndCandPage();
int _GetCandValue();
HWND _CreateCandPage();
HWND _CreateCandPageWithHandle(HWND hwnd);
void _HideCandPage();
void _ShowCandPage();
void _MoveCandPage(int x,int y);
void _ClearCandPage();
void _ExpandCandi();

//notify
void _ShowNotifyPageArray(const std::vector<std::wstring>& candidates);
void _SetNotifyString(const std::wstring& candidate);
void _EndNotifyPage();
int _GetNotifyValue();
HWND _CreateNotifyPage();
HWND _CreateNotifyWithHandle(HWND hwnd);
void _HideNotifyPage();
void _ShowNotifyPage();
void _MoveNotifyPage(int x,int y);
void _ClearNotifyPage();
void _ExpandNotify();