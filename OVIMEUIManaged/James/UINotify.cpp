//#define OV_DEBUG
#include <stdio.h>
#include "OVIMEUI.h"
#include "DotNETHeader.h"
void UICreateNotifyWindow(HWND hUIWnd)
{
	
	if (!IsWindow(uiNotify.hWnd))
	{
		uiNotify.hWnd=_CreateNotifyPage();

	}
	return;
}

/*
BOOL GetNotifyPosFromCompWnd(LPSIZE lpsz)
{
	if (IsWindow(uiComp.hWnd))
	{
		RECT rc, screenrc;
		POINT pt;

		pt.x = uiComp.pt.x;
		pt.y = uiComp.pt.y;

		GetWindowRect(uiNotify.hWnd, &rc);
		if( pt.x < 0 )
			pt.x = 0;

		SystemParametersInfo(SPI_GETWORKAREA,
				0,
				&screenrc,
				0);
		if( (pt.x + lpsz->cx) > screenrc.right)
			pt.x = rc.left - lpsz->cx - 5;
		if( (pt.y + lpsz->cy) > screenrc.bottom)
			pt.y = screenrc.bottom - lpsz->cy;

		uiNotify.pt.x = pt.x;
		uiNotify.pt.y = pt.y;
		return TRUE;
	}
	else
	{
		uiNotify.pt.x = uiComp.pt.x;
		uiNotify.pt.y = uiComp.pt.y;
	}
	return FALSE;
}
*/

void UIShowNotifyWindow()
{
	
	_ShowNotifyPage();
	

}

void UIHideNotifyWindow()
{
	//if (IsWindow(uiNotify.hWnd))
		//ShowWindow(uiNotify.hWnd, SW_HIDE);
		_ClearNotifyPage(); 
		_HideNotifyPage();
} 

void UIMoveNotifyWindow(int X, int Y)
{ 
	_MoveNotifyPage(X, Y);
	
}

void UISetNotifyStr(wchar_t* lpStr)
{
	free(lpNotifyStr);
	lpNotifyStr = wcsdup(lpStr);
	_SetNotifyString(lpNotifyStr);
}