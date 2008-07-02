#include <stdio.h>
#include "OVIMEUI.h"
#include "DotNETHeader.h"

void UICreateCandWindow(HWND hUIWnd)
{
	if (!IsWindow(uiCand.hWnd))
		uiCand.hWnd = _CreateCandiPageWithHandle(hUIWnd);		
	
	return;
}

int CompIndexToXPos( int i );

/*
BOOL GetCandPosFromCompWnd(LPSIZE lpsz)
{
	if (IsWindow(uiComp.hWnd))
	{
		RECT rc, screenrc;
		POINT pt;

		GetWindowRect(uiComp.hWnd, &rc);

		pt.x = rc.left;
		pt.y = rc.bottom + 2;

		pt.x += CompIndexToXPos( CompCursorPos );
		GetWindowRect(uiCand.hWnd, &rc);
		pt.x -= 16;
		if( pt.x < 0 )
			pt.x = 0;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);
		if( (pt.x + lpsz->cx) > screenrc.right)
			pt.x = screenrc.right - lpsz->cx - 5;//rc.left - lpsz->cx - 5;
		if( (pt.y + lpsz->cy) > screenrc.bottom)
			pt.y = screenrc.bottom - lpsz->cy;

		uiCand.pt.x = pt.x;
		uiCand.pt.y = pt.y;
		return TRUE;
	}
	return FALSE;
}
*/
void UISetCandStr(wchar_t* lpStr)
{

	wchar_t* lpCandStr;
	
	if(wcslen(lpStr))
	{	
		lpCandStr = wcsdup(lpStr);
		_SetCandiString(lpCandStr);
	}
	else
	{
		_ClearCandiPage();		
	}

}


void UIMoveCandWindow(int X, int Y, int compHeight)
{
	_MoveCandiPage(X, Y, compHeight);	
	
}


static bool stillrunning = false;
void UISelectNextCandItem(int i)
{
	if(!stillrunning){
		stillrunning=true;
		_SelectNextItem(i);
		stillrunning=false;
	}
}
int UIGetCandSelectedItemKey()
{
	return _GetSelectedItemKey();
}

void UIShowCandWindow()
{
	murmur("UIShowCandWindow");
	if (IsWindow(uiCand.hWnd)) {
		_ShowCandiPage();
	}
}

void UIHideCandWindow()
{
	murmur("UIHideCandWindow");
	if (IsWindow(uiCand.hWnd)) {
		_HideCandiPage();
	}
}

void UIExpandCandi()
{
	_ExpandCandi();
}
