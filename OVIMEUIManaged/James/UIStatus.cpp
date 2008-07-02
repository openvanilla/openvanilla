//#define OV_DEBUG
#include <stdio.h> 
#include "OVIMEUI.h"
#include "DotNETHeader.h"

void UISetkpCount(int count)
{
	_setStatuskpCount(count);
}

void UISetofCount(int count)
{
	_setStatusofCount(count);
}
void UISetimCount(int count)
{

	_setStatusimCount(count);
}


void UICreateStatusWindow(HWND hUIWnd, int xPos, int yPos)
{	
	murmur("\tUICreateStatusWindow, hUIWnd=%x, %i, %i",hUIWnd, xPos, yPos);

	RECT rec;
	if (!IsWindow(uiStatus.hWnd)) {
		uiStatus.hWnd = _CreateStatusPage();
	
		//設定C# Status 內 m_AppHWnd
		_SetStatusAppHWnd(hUIWnd);

		//設定 module menu (init: current module)
		SendMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, 9);  // set all module names.
		//SendMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, 3);
		//UISetStatusModStr();

		//設定中英 
		//_SetStatusChiEng();

		//設定繁簡
		_SetStatusSimpifiedOrTraditional(isTraditional);  		

		//設定 hIMEWnd (?)
		hIMEWnd = hUIWnd; //存到 hIMEWnd 之後會拿來判斷
		
		//移動到螢幕右下方
		
		if((xPos==0)||(yPos==0)) {
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rec, 0);
			UIMoveStatusWindow(hUIWnd, rec.right - 200, rec.bottom - 50);
		}
		else
			UIMoveStatusWindow(hUIWnd, xPos, yPos);
	}
	//設定C# Status 內 m_AppHWnd 
	_SetStatusAppHWnd(hUIWnd); 	
}

const wchar_t* convertUtf8NameToUcs2(const char* name)
{
	wchar_t *modCurrentName;
	wchar_t modNameUCS2[1024];
	MultiByteToWideChar(
		CP_UTF8, 0, name, (int)strlen(name)+1, modNameUCS2, 1024);
	modCurrentName = modNameUCS2;
	return wcsdup(modCurrentName);
}

void UISetStatusModStrMenuEach(const char* newName)
{
	_SetStatusMenuModString(convertUtf8NameToUcs2(newName));
}

//set Status window module list
void UISetStatusModStrMenuAll(int modAmount, const char* modNames[])
{
	UIClearStatusMenuModString();	
	for(int i = 0; i < modAmount; i++){
		murmur("\t set module %i name:%s",i , modNames[i]);
		UISetStatusModStrMenuEach(modNames[i]);  
	}
}
bool UIGetModuleChecked(int index)
{
	return _GetItemChecked(index);
}

void UISetModuleChecked(int index, bool b)
{
	_SetItemChecked(index,b);
}



void UISetStatusModStrCurrent(int index)
{
	_SetStatusModString(index);
}

void UIClearStatusMenuModString()
{
	_ClearStatusMenuModString();
}

void UIMoveStatusWindow(HWND hUIWnd, int X, int Y)
{   
	
	/*if (!IsWindow(uiStatus.hWnd))
		UICreateStatusWindow(hUIWnd);*/

	if (IsWindow(uiStatus.hWnd))
	{
		POINT pt;
		RECT screenrc;
		SIZE sz;

		sz.cx = 0;
		sz.cy = 0;
		uiStatus.pt.x = (X>0)?X : 0; 
		uiStatus.pt.y = (Y>0)?Y : 0;
		sz.cx = uiStatus.sz.cx;
		sz.cy = uiStatus.sz.cy;
		pt.x = uiStatus.pt.x;
		pt.y = uiStatus.pt.y;
		
		MONITORINFO mi;
		mi.cbSize=sizeof(MONITORINFO); 
		
		GetMonitorInfo(MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST), &mi);
		if( (pt.x + sz.cx) > mi.rcMonitor.right )
			pt.x = mi.rcMonitor.right - 200;
			if( (pt.y + sz.cy) > mi.rcMonitor.bottom )
			pt.y = mi.rcMonitor.bottom - 50;		

/*		SystemParametersInfo(SPI_GETWORKAREA, 0, &screenrc, 0);

		if( (pt.x + sz.cx) > screenrc.right )
			pt.x = screenrc.right - 200;
		if( (pt.y + sz.cy) > screenrc.bottom )
			pt.y = screenrc.bottom - 50;		
*/
		_MoveStatusPage(pt.x, pt.y);
	}
	
}

void UIShowStatusWindow()
{
	//if (IsWindow(uiStatus.hWnd))
	{	
		_ShowStatusPage(); //show		
	}
}

void UIHideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
		//ShowWindow(uiStatus.hWnd, SW_HIDE);
		_HideStatusPage();
}
int UIGetStatusPos(LPPOINT lp_pt)
{
	if (IsWindow(uiStatus.hWnd)) {
	LPRECT lp_rect;
	lp_rect = new RECT;
	GetWindowRect(uiStatus.hWnd,lp_rect);
	lp_pt->x = lp_rect->left;
	lp_pt->y = lp_rect->top;
	murmur("status window at (%i, %i).", lp_pt->x, lp_pt->y);
	return 1;
	} else return -1;
	
	
}

int CompIndexToXPos(int index);


void UISetCursorPos(int i)
{
	CompCursorPos = i;
}

#if 0
void UISetMarkFrom(int i)
{
//	murmur("-----> UISetMarkFrom, status window");
	CompSelStart = i;
	murmur( "---> UISetMarkFrom Start %d", CompSelStart);
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mf=%d"),  i);
	TextOut( tmpdc, 0, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
}

void UISetMarkTo(int i)
{
//	murmur("-----> UISetMarkTo, status window");
/*	HDC tmpdc = GetDC(NULL);
	TCHAR debug_info[100];
	_stprintf(debug_info, _T("mt=%d"),  i);
	TextOut( tmpdc, 40, 0, debug_info, lstrlen(debug_info));
	ReleaseDC( NULL, tmpdc );
*/
	CompSelEnd = i;
	murmur( "---> UISetMarkFrom End %d", CompSelEnd);
}
#endif

int UIModuleChange()
{
	CompCursorPos=0;  //James test
	
	CurrentIC = _GetStatusSelectedModuleIndex();
	//UISetStatusModStrCurrent(CurrentIC);
	_ShowStatusPage();

	return CurrentIC;
}

int UIModuleRotate()
{
	_RotateStatusSelectedModule();
	return UIModuleChange();
}

void UIChangeHalfFull()
{
	_SetStatusHalfFull(); 
	UIShowStatusWindow();
}

void UIChangeChiEng()
{
	//SendMessage( hToolbar, TB_CHANGEBITMAP, ID_CHI_ENG, MAKELPARAM(isChinese ? 2 : 3, 0));
		
	//設定中英
	//isChinese = !isChinese;
	_SetStatusChiEng();  

	//refresh
	UIShowStatusWindow();
}

void UIChangeSimpifiedOrTraditional(HWND hWnd)
{	
	//設定繁簡
	isTraditional = !isTraditional;		
	_SetStatusSimpifiedOrTraditional(isTraditional); 

	//refresh
	UIShowStatusWindow();

	/*以下是模組要做的事情*/

}

void UIChangeBoPoMoFoLayout(int index)
{
	if(index == -1) return;

	CompCursorPos=0;  //James test

	CurrentIC = index;
	UISetStatusModStrCurrent(CurrentIC);
	_ShowStatusPage();
}

int UICurrentInputMethod()
{
	return CurrentIC;
}
