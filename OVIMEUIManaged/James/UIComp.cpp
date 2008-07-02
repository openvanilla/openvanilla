//#define OV_DEBUG
#include <stdio.h>
#include "OVIMEUI.h"
#include "DotNETHeader.h"
#include <exception>

void UICreateCompWindow(HWND hUIWnd)
{
	//	murmur("%1.3f sec:\tC# comp window, create");
	if (!IsWindow(uiComp.hWnd))
	{
		uiComp.hWnd = _CreateCompPage();
		_SetCompAppHWnd(hUIWnd);
	}
}

void UIMoveCompWindow(
	int x, int y,
	int fontHeight, const wchar_t* fontName)
{
	_MoveCompPage(x, y, fontHeight, fontName);
}

void UISetCompStr(wchar_t* lpStr)
{
	//_ClearCompPage();  //James:®³±¼, no need

	if(wcslen(lpStr))
	{
		lpCompStr = wcsdup(lpStr);
		_SetCompString(lpCompStr);
	}
	/*else  //James:®³±¼, no need
	{
		_ClearCompPage();		
	}*/
}
void UIClearCompStr()
{
	_ClearCompPage();
}

void UISetCompCaretPosX(int xIndex)
{
	_SetCompCaretPosX(xIndex);
}

int CompIndexToXPos(int index)
{
	if( !lpCompStr || !*lpCompStr )
		return 1;
	HDC hdc = GetDC( uiComp.hWnd );
	HGDIOBJ oldFont = SelectObject(hdc, hUIFont);
	SIZE size;
	GetTextExtentPoint32( hdc, lpCompStr, index, &size);
	SelectObject(hdc, oldFont);
	ReleaseDC( uiComp.hWnd, hdc );
	return size.cx + 1;
}

void UIShowCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, show");
	if (IsWindow(uiComp.hWnd))
	{
		_ShowCompPage();
	}

}

void UIHideCompWindow()
{
	//	murmur("%1.3f sec:\tC# comp window, hide");
	if (IsWindow(uiComp.hWnd))
	{
		_HideCompPage();		
	}
}

void UISetMarkFrom(int i)
{
	CompSelStart = i;
	_SetCompMarkFrom(CompSelStart);
}

void UISetMarkTo(int i)
{
	CompSelEnd = i;
	_SetCompMarkTo(CompSelEnd);
}

int UIGetHeight()
{
	return _GetHeight();
}

int UIGetCaretPosX()
{
	return _GetCaretPosX();
}
