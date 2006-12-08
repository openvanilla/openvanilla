#define OV_DEBUG

#include "OVIME.h"

LRESULT APIENTRY UIWndProc(HWND hWnd,
						   UINT msg,
						   WPARAM wParam,
						   LPARAM lParam)
{
	HIMC			hUICurIMC;
	LPINPUTCONTEXT	lpIMC;
	LONG			lRet = 0L;
	LPMYPRIVATE lpMyPrivate;

	hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);

	//
	// Even if there is no current UI. these messages should not be pass to 
	// DefWindowProc().
	//
	if(!hUICurIMC)
	{        
		switch (msg)
		{
		case WM_IME_STARTCOMPOSITION:
		case WM_IME_ENDCOMPOSITION:
		case WM_IME_COMPOSITION:
		case WM_IME_NOTIFY:
		case WM_IME_CONTROL:
		case WM_IME_COMPOSITIONFULL:
		case WM_IME_SELECT:
		case WM_IME_CHAR:
			return 0L;
		default:
			break;
		}
	}

	switch (msg)
	{

	case WM_MOUSEACTIVATE:
		murmur("WM_MOUSEACTIVATE");
		break;

	case WM_ACTIVATEAPP:
		murmur("WM_ACTIVATEAPP");
		break;

	case WM_CHILDACTIVATE:
		murmur("WM_CHILDACTIVATE");
		break;

	case WM_CREATE:  
		murmur("WM_CREATE");
		CompX = CompY = -1;
		UICreateCompWindow(hWnd);
		UICreateCandWindow(hWnd);
		UICreateNotifyWindow(hWnd);
		break;

	case WM_WINDOWPOSCHANGING: //The WM_WINDOWPOSCHANGING message is sent to a window whose size, position, or place in the Z order is about to change as a result of a call to the SetWindowPos function or another window-management function.
		murmur("WM_WINDOWPOSCHANGING");
		break;

	case WM_IME_SETCONTEXT:
		murmur("WM_IME_SETCONTEXT");
		murmur("%p",lParam);				
		if (wParam) //TRUE if the window is active, and FALSE otherwise.
		{
			murmur("wParm=%p",wParam);
			murmur("hUICurIMC:%p", hUICurIMC);
			if (hUICurIMC)  //hUICurIMC==0 表示出錯(?)
			{
				AdjustUIWindow(hWnd);				
			}
			else   // it is NULL input context. (?)
			{
				UIHideCandWindow();
				UIHideCompWindow();
			}
		}
		else //若把這個else拿掉 則切換視窗c# form 不會消失
		{
			murmur("wParam=NULL");
			isActive=false;
			UIHideCandWindow();
			UIHideCompWindow();
		}
		break;

	case WM_IME_STARTCOMPOSITION:
		murmur("WM_IME_STARTCOMPOSITION");
		/*
		//James test
		//在start時先move到正確的座標
		lpIMC = ImmLockIMC(hUICurIMC);
		POINT pt2;
		if(CompX < 0) 
		{
		//pt2.x = 15;
		//pt2.y = 15;
		pt2.x = 0;
		pt2.y = 0;			
		ClientToScreen(lpIMC->hWnd, &pt2);
		CompX = pt2.x;
		CompY = pt2.y;
		}
		//lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
		UIMoveCompWindow(hWnd, CompX, CompY, NULL);
		//*/
		break;

	case WM_IME_COMPOSITION:
		murmur("WM_IME_COMPOSITION");		
		//test
		//MyGenerateMessage(hWnd, WM_IME_NOTIFY, IMN_SETCANDIDATEPOS, 0);
		//lRet = NotifyHandle(hUICurIMC, hWnd, WM_IME_NOTIFY, IMN_SETCANDIDATEPOS, NULL);
		AdjustUIWindow(hWnd);
		ImmUnlockIMC(hUICurIMC); 
		break;

	case WM_IME_ENDCOMPOSITION:
		murmur("WM_IME_ENDCOMPOSITION");
		UIHideCompWindow();
		UIHideCandWindow();
		break;

	case WM_IME_COMPOSITIONFULL:
		murmur("WM_IME_COMPOSITIONFULL");
		break;

	case WM_IME_SELECT:
		murmur("WM_IME_SELECT");
		break;

	case WM_IME_CONTROL:
		murmur("WM_IME_CONTROL");
		lRet = ControlHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;


	case WM_IME_NOTIFY:
		murmur("WM_IME_NOTIFY: %p",wParam);
		lRet = NotifyHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
		murmur("WM_DESTROY");
		loader->shutdown();
		//loader->closeModule();
		break;

	case WM_IME_RELOADCONFIG:
		murmur("WM_IME_RELOADCONFIG");
		loader = AVLoader::getLoader();		
		loader->reloadConfig();
		
		break;

	default:
		murmur("no this message,%p",msg);
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return lRet;
}
void AdjustUIWindow(HWND hWnd)  //調整comp cand
{	
	
	HIMC			hUICurIMC;
	LPINPUTCONTEXT	lpIMC;
	LONG			lRet = 0L;
	LPMYPRIVATE lpMyPrivate;
	hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);
	lpIMC = ImmLockIMC(hUICurIMC);
	POINT pt;
	if(CompX < 0) 
	{
		pt.x = 15;
		pt.y = 15;
		ClientToScreen(lpIMC->hWnd, &pt);
		CompX = pt.x;
		CompY = pt.y;
	}
	lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);		
	if(lpMyPrivate->PreEditStr)//有comp指標 
	{						
		murmur("%s",lpMyPrivate->PreEditStr);
		if(wcslen(lpMyPrivate->PreEditStr)&& !isActive) //comp有字且還沒被active
		{
			murmur("COMPOSITION, move(%d, %d)", CompX, CompY);
			UIMoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
			UISetCompStr(lpMyPrivate->PreEditStr);
			UIShowCompWindow();
			isActive=true;
		}
		else if(wcslen(lpMyPrivate->PreEditStr)&& isActive)//comp有字但已經被active 
		{
			if(!wcslen(lpMyPrivate->CandStr) )//如果沒有candidate才要秀
			{
				murmur("COMPOSITION, move(%d, %d)", CompX, CompY);
				UIMoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
				UISetCompStr(lpMyPrivate->PreEditStr);
				UIShowCompWindow();
			}
			else
			{
				UISetCompStr(lpMyPrivate->PreEditStr);
			}
		}
		else{//comp沒字
			UIHideCompWindow();
			isActive=false;
		}

	}
	else //沒有comp指標 =>hide
	{
		UIHideCompWindow();
		isActive=false;
	}
	if(lpMyPrivate->CandStr)
	{
		if(wcslen(lpMyPrivate->CandStr))
		{
			UIMoveCandWindow(hWnd, CompX,CompY+30,NULL);
			UISetCandStr(lpMyPrivate->CandStr);
			//UIShowCandWindow();
		}
		else
			UIHideCandWindow();
	}
	else
	{
		UIHideCandWindow();
	}
	ImmUnlockIMCC(lpIMC->hPrivate);
	
	return;
}
