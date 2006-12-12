#include<string>
#pragma managed
#include<vector>
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
HWND _CreateCompPage();
void _ShowCompPage();
int _GetHeight();
void _HideCompPage();
void _ClearCompPage();
void _MoveCompPage(int x, int y);
void _SetCompString(const std::wstring&);
HWND _CreateStatusPage();
void _ShowStatusPage();
void _MoveStatusPage(int, int);
void _HideStatusPage();
void _SetStatusChiEng(bool);
void _SetStatusModString(const std::wstring&);
void _SetStatusMenuModString(const std::wstring&);
void _ClearStatusMenuModString();
void _SetStatusAppHWnd(HWND hwnd);
int _GetStatusSelectedModuleIndex();
void _SetCompCaretPosX(int);
void _SetCompMarkFrom(int);
void _SetCompMarkTo(int);
void _SetUserDir();
