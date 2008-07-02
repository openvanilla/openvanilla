//#define OV_DEBUG 

#pragma unmanaged

#include <stdio.h> 
#include "OVIMEUI.h"
#include "DotNETHeader.h"
#include <shobjidl.h>
#include <shlobj.h>
#include <atlbase.h>



// Status window 
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


void UICreateStatusWindow(HWND hUIWnd, int xPos, int yPos, bool isDocked)
{	
	
	
	if (!IsWindow(uiStatus.hWnd)) {
		murmur("\tUICreateStatusWindow, hUIWnd=%x @(%i, %i), isDocked:%d",hUIWnd, xPos, yPos, isDocked);
		RECT rec;

		uiStatus.pt.x = (xPos>0)?xPos : 0; 
		uiStatus.pt.y = (yPos>0)?yPos : 0;

		uiStatus.hWnd = _CreateStatusPage();

		murmur("\tStatusWindow created. hWnd=%x",uiStatus.hWnd);
	
		//設定C# Status 內 m_AppHWnd
		//_SetStatusAppHWnd(hUIWnd);

		//設定 module menu (init: current module)
		SendMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, 9);  // set all module names.
		//SendMessage(hUIWnd, WM_IME_NOTIFY, IMN_PRIVATE, 3);
		//UISetStatusModStr();

		//設定中英 
		//_SetStatusChiEng();

		//設定繁簡
		//_SetStatusSimpifiedOrTraditional(isTraditional);  		

		//設定 hIMEWnd (?)
		hIMEWnd = hUIWnd; //存到 hIMEWnd 之後會拿來判斷
		
		//移動到螢幕右下方
		if(isDocked)
		{
			murmur("\tDocked the status window..");
			UISetIsStatusDocked(true);  
		}
		else 
		{
			UISetIsStatusDocked(false); 
			if((xPos==0)||(yPos==0)) 
			{
				murmur("\tMove the status window to default location.");
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rec, 0);
				UIMoveStatusWindow(hUIWnd, rec.right - 200, rec.bottom - 50);
			}
			else
			{
				murmur("\tMove the status window to saved location, isDocked:%d.", UIGetIsStatusDocked());
				UIMoveStatusWindow(hUIWnd, xPos, yPos);
			}
		}
		
		
	}
	else
		murmur("Status window is already created.");
	
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

void UIDockStatusWindow(int X, int Y)
{   
	if (IsWindow(uiStatus.hWnd))
	{
		_DockStatusPage(X, Y);
	}
	
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
//static bool m_docked=true;
void UIShowStatusWindow()
{
	
	if (IsWindow(uiStatus.hWnd))
	{	
		murmur("\tShow status window. isDocked:%d", UIGetIsStatusDocked());
		
		if(UIGetIsStatusDocked()) 
		{
			murmur("\tAdd deskband...");
			UIShowDeskBand(true);// 		bandadded=true;
		}
		else
			murmur("\tStatus is not doced");
		
		_ShowStatusPage(); //show		 
	}
} 


void UIHideStatusWindow()
{
	if (IsWindow(uiStatus.hWnd))
	{
		murmur("\tHide status window...");
		//ShowWindow(uiStatus.hWnd, SW_HIDE);  
		
		if(UIGetIsStatusDocked())
		{
			murmur("\tRemove deskband..."); 
			
			UIShowDeskBand(false);// bandadded = false;
		}
		
	
		_HideStatusPage();
	}
	
}

bool UIGetIsStatusDocked()
{
	return _GetIsStatusDocked();
}


void UISetIsStatusDocked(bool t)
{
	_SetIsStatusDocked(t);
	if(!t)
		UIMoveStatusWindow(NULL, uiStatus.pt.x, uiStatus.pt.y);
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
	//UIShowStatusWindow();
}

void UIChangeChiEng()
{
		
	//設定中英
	//isChinese = !isChinese;
	_SetStatusChiEng();  

	//refresh
	//UIShowStatusWindow();
}

void UIChangeSimpifiedOrTraditional(HWND hWnd)
{	
	//設定繁簡
	isTraditional = !isTraditional;		
	_SetStatusSimpifiedOrTraditional(isTraditional); 

	//refresh
	//UIShowStatusWindow();

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



// Candi window

void UISetCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	_SetCandiColor( forecolor, backcolor, titlecolor,titlebackcolor);
}

void UISetCandiFont(const wchar_t* ff, int fontsize)
{
	_SetCandiFont(ff, fontsize);
}

void UISetCandiOpacity(int o)
{
	_SetCandiOpacity(o);
}


int UIGetCandHeight()
{
	return _GetCandHeight();
}

int UIGetCandWidth()
{
	return _GetCandWidth();
}


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

// Composition window
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
	//_ClearCompPage();  //James:拿掉, no need

	if(wcslen(lpStr))
	{
		lpCompStr = wcsdup(lpStr);
		_SetCompString(lpCompStr);
	}
	/*else  //James:拿掉, no need
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


//Notify window

void UISetNotifyColor(int forecolor, int backcolor)
{
	_SetNotifyColor( forecolor, backcolor);
}

void UISetNotifyFont(const wchar_t* ff, int fontsize)
{
	_SetNotifyFont(ff, fontsize);
}

void UISetNotifyEnabled(bool b)
{
	_SetNotifyEnabled(b);
}

void UISetNotifyOpacity(int o)
{
	_SetNotifyOpacity(o);
}

int UIGetNotifyHeight()
{
	return _GetNotifyHeight();

}


int UIGetNotifyWidth()
{
	return _GetNotifyWidth();

}

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


	
    CComPtr <IBandSite>   spBandSite=NULL;  
	//CComPtr <IUnknown>    spBand=NULL;  
	CComPtr <IDeskBand>    spBand=NULL;  

bool UIShowDeskBand(bool t)
{
	CLSID CLSID_OVDeskBand ={0x6d5d9154, 0xb557, 0x4509, {0x82, 0x9e, 0x94, 0xb8, 0xbf, 0x96, 0x2e, 0x5b}};
	static HRESULT  bandID=0;
	bool ret=false;
	int i =0;
	
	
	murmur("hIMEWND:%x, AncestorWND:%x, TopWindow:%x, ForegroundWindow:%x, ShellWindow:%x "
				,hIMEWnd, GetAncestor(hIMEWnd,GA_ROOTOWNER), GetTopWindow(NULL), GetForegroundWindow(), GetShellWindow());
	
	

	//CLSID CLSIDDeskBand = {0x46b3d3ef, 0x71a, 0x4b7e, {0x8a, 0xa2, 0xe5, 0x60, 0x81, 0xd, 0xab, 0x35}};
	
	// only for vista
	/*
	ITrayDeskBand * spTrayDeskBand=0; 
	if(SUCCEEDED(CoCreateInstance(CLSID_TrayDeskBand, NULL, CLSCTX_LOCAL_SERVER,
									IID_ITrayDeskBand, (void**)&spTrayDeskBand)))
	{
		if(t) spTrayDeskBand->ShowDeskBand(CLSID_OVDeskBand);
		else spTrayDeskBand->HideDeskBand(CLSID_OVDeskBand);
	}
	*/
	CComPtr <IUnknown>    spBandService;  
	if(!spBandSite) 
	{
		if(SUCCEEDED(spBandService.CoCreateInstance(CLSID_TrayBandSiteService,   NULL)))
		{
			//HWND aWnd = GetAncestor(hIMEWnd, GA_ROOTOWNER);
			//if(aWnd) BringWindowToTop(aWnd); 
			murmur("Create IBandsite interface succeed"); 
			spBandService->QueryInterface(&spBandSite);
		}
		else 
		{
			murmur("Create IBandsite interface failed");
			
			spBand = NULL;
			spBandSite = NULL;
			//WaitForSingleObject(NULL, 3000);
			//SendMessage(hIMEWnd, WM_IME_NOTIFY, IMN_OPENSTATUSWINDOW, 0);
		
		}
	}
	
	
	
	if(spBandSite && !spBand ) 
	{
		if(SUCCEEDED(CoCreateInstance(CLSID_OVDeskBand,   
				NULL,   CLSCTX_INPROC,   IID_IUnknown,   (void**)&spBand)))
		{
	
	//	CComPtr<IObjectWithSite>   spSite;  
	//    if(SUCCEEDED(spBand->QueryInterface(&spSite))   )   
	//	{
	//		spSite->SetSite(spBandSite);  
	//	}
	//	else spBand = NULL;
		}
		else spBand = NULL;
		
	}
	
    //if(SUCCEEDED(spBandService.CoCreateInstance(CLSID_TrayBandSiteService,   NULL))   
	// 				  &&   SUCCEEDED(spBandService->QueryInterface(&spBandSite))   )  
	if( spBand && spBandSite)
    {  			
        	if(t) 
			{
			if(!((int)bandID >0))
			  {
				bandID = spBandSite->AddBand(spBand);  
				murmur("\tBandID:%d",bandID);
				if( (int)bandID >0 ) 
				{
					murmur("Create deskband succeeded."); 

					//spBand  =   NULL;  
					//spBandSite   =   NULL;   
					ret = true;  
				}
				else
				{
					murmur("Create deskband failed."); 
					spBand->ShowDW(0);  
					spBand  =   NULL; 
					spBandSite   =   NULL;   
					bandID =0; 
				}
			  }
			else
			{
				murmur("\tBandID:%d",bandID);
				murmur("Deskband created."); 
			}


				
			}
			else if((int)bandID >0)
			{
				
				
				//HWND aWnd = GetAncestor(hIMEWnd, GA_ROOTOWNER);
				//if(aWnd) BringWindowToTop(aWnd);
			    if(SUCCEEDED( spBandSite->RemoveBand(bandID))) 
				{
					murmur("\tBandID:%d",bandID);
					murmur("Remove deskband succeeded.");  
					
					spBand  =   NULL; 
					spBandSite   =   NULL;   
					bandID =0;
					
					//CoUninitialize();
					ret = true;
				}
				else
				{ 
					murmur("Remove deskband failed.");
					//spBand->CloseDW(0); 
					//spBand=NULL;
					//spBandSite = NULL;   
					//bandID =0;
					
					//HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,_T("OVE")); 
					//WaitForSingleObject(hEvent, 500);
					 
					//SendMessage(hIMEWnd, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, 0);
				}
				
			}
			else
			{
					murmur("Deskband was not created.");
					//spBand->CloseDW(0); 
					spBand=NULL;
					spBandSite = NULL;   
				
			}			  
            
                    
         
    }              
	

	//}
	//while(!ret && i++<3);
	murmur("::hIMEWND:%x, AncestorWND:%x, TopWindow:%x, ForegroundWindow:%x, ShellWindow:%x "
				,hIMEWnd, GetAncestor(hIMEWnd,GA_ROOTOWNER), GetTopWindow(NULL), GetForegroundWindow(), GetShellWindow());

    
	return ret;
	
}