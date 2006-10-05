#include<string>
#pragma managed
#include<vector>
void _ShowCandPageArray(const std::vector<std::wstring>& candidates);
void _SetCandString(const std::wstring& candidate);
void _EndCandPage();
int _GetCandValue();
HWND _CreateCandPage();
void _HideCandPage();
void _ShowCandPage();
void _MoveCandPage(int x,int y);
void _ClearCandPage();
HWND _CreateCompPage();
void _ShowCompPage();
void _HideCompPage();
void _ClearCompPage();
void _MoveCompPage(int x, int y);
void _SetCompString(const std::wstring&);
