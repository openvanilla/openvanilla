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
		loader=AVLoader::getLoader();
		loader->connectDisplayServer(dsvr);
		CompX = CompY = -1;
//		UISCompWindow(hWnd);
		UICreateCompWindow(hWnd);
		UICreateCandWindow(hWnd);
		UICreateNotifyWindow(hWnd);
		break;

	case WM_WINDOWPOSCHANGING: //The WM_WINDOWPOSCHANGING message is sent to a window whose size, position, or place in the Z order is about to change as a result of a call to the SetWindowPos function or another window-management function.
		murmur("WM_WINDOWPOSCHANGING");
		break;

	case WM_IME_SETCONTEXT:  //switch between windows
		murmur("WM_IME_SETCONTEXT");	
		switch(lParam)
		{
			case ISC_SHOWUICANDIDATEWINDOW :
				murmur("ISC_SHOWUICANDIDATEWINDOW ");
				break;
			case ISC_SHOWUICOMPOSITIONWINDOW :
				murmur("ISC_SHOWUICOMPOSITIONWINDOW  ");
				break;
			case ISC_SHOWUIGUIDELINE :
				murmur("ISC_SHOWUIGUIDELINE ");
				break;
			case ISC_SHOWUIALLCANDIDATEWINDOW :
				murmur("ISC_SHOWUIALLCANDIDATEWINDOW");
				break;
			case ISC_SHOWUIALL :
				murmur("ISC_SHOWUIALL");
				break;
			default:
				murmur("default");
				break;		
		}
		//CompX=-1;
		//RefreshUI(hWnd);	
		if (wParam) //switch in
		{		
			murmur("setcontext to hwnd:%x",hWnd);			
			if (hUICurIMC)  //hUICurIMC==0 表示出錯(?)
			{
				murmur("if(hUICurIMC)=true, show all");
				dsvr->lockIMC(hUICurIMC);
				dsvr->showBuf(true);
				dsvr->showCandi(true);				
			}
			else   // it is NULL input context. (?)
			{				
				murmur("if(hUICurIMC)=false, hide all");
				dsvr->showBuf(false);
				dsvr->showCandi(false);		
			}
		}
		else //switch out
		{
			murmur("switch out, hide all");
			dsvr->showBuf(false);
			dsvr->showCandi(false);
		}
		break;

	case WM_IME_STARTCOMPOSITION:
		murmur("WM_IME_STARTCOMPOSITION");
		dsvr->showStatus(true);
		//dsvr->showBuf(true);
		RefreshUI(hWnd);
		//James comment
		//Can we在start時先move到正確的座標?		
		break;

	case WM_IME_COMPOSITION:
		murmur("WM_IME_COMPOSITION");		
		RefreshUI(hWnd);
		//ImmUnlockIMC(hUICurIMC); 
		break;

	case WM_IME_ENDCOMPOSITION:
		murmur("WM_IME_ENDCOMPOSITION");
		dsvr->showBuf(false);
		dsvr->showCandi(false);		
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
		dsvr->showStatus(false);
		dsvr->showBuf(false);
		dsvr->showCandi(false);
		dsvr->releaseIMC();

		loader->closeModule(); //also send buf to app
		
		break;

	case WM_IME_RELOADCONFIG:
		murmur("WM_IME_RELOADCONFIG");
		loader = AVLoader::getLoader();
		loader->shutdown();		

		loader = AVLoader::getLoader();
		//loader->connectDisplayServer(dsvr);
		loader->reloadConfig();
		
		break;

	default:
		murmur("no this message,%p",msg);
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return lRet;
}

