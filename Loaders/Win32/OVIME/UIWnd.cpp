//#define OV_DEBUG

#include "OVIME.h"

LRESULT APIENTRY UIWndProc(HWND hWnd,
						   UINT msg,
						   WPARAM wParam,
						   LPARAM lParam)
{
	LRESULT lRet = 0L;
	//LPINPUTCONTEXT	lpIMC;
	//LPMYPRIVATE lpMyPrivate;
	HIMC hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);
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

	bool doDefWndProc = false;

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
	{
		murmur("WM_CREATE");

		dsvr->setHIMC(hUICurIMC);
		AVLoader* loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		CompX = CompY = -1;
		//UICreateStatusWindow(hWnd);
		UICreateCompWindow(hWnd);
		UICreateCandWindow(hWnd);
		UICreateNotifyWindow(hWnd);
		break;
	}
	case WM_WINDOWPOSCHANGING: 
		//The WM_WINDOWPOSCHANGING message is sent to 
		//a window whose size, position, or place in the 
		//Z order is about to change as a result of a call
		//to the SetWindowPos function or another window-management function.
		murmur("WM_WINDOWPOSCHANGING");
		break;

	case WM_IME_SETCONTEXT:  //switch between windows
		murmur("WM_IME_SETCONTEXT");	
		switch(lParam)
		{
			case ISC_SHOWUICANDIDATEWINDOW :
				murmur("\tISC_SHOWUICANDIDATEWINDOW ");
				break;
			case ISC_SHOWUICOMPOSITIONWINDOW :
				murmur("\tISC_SHOWUICOMPOSITIONWINDOW  ");
				break;
			case ISC_SHOWUIGUIDELINE :
				murmur("\tISC_SHOWUIGUIDELINE ");
				break;
			case ISC_SHOWUIALLCANDIDATEWINDOW :
				murmur("\tISC_SHOWUIALLCANDIDATEWINDOW");
				break;
			case ISC_SHOWUIALL :
				murmur("\tISC_SHOWUIALL");
				break;
			default:
				murmur("\tdefault");
				break;		
		}
		//CompX=-1;
		//RefreshUI(hWnd);			
		
		if (wParam) //switch in
		{		
			murmur("\tsetcontext to hwnd:%x",hWnd);			
			if (hUICurIMC)  //hUICurIMC==0 表示出錯(?)
			{
				dsvr->setHIMC(hUICurIMC);
				AVLoader* loader = AVLoader::open();
				loader->connectDisplayServer(dsvr);

				RefreshUI(hWnd); //多視窗 program 切換子視窗要重設 ic position
				murmur("\thUICurIMC==true");
				if(lParam & ISC_SHOWUICOMPOSITIONWINDOW) // not sure
				{	
					dsvr->showBuf(true);
				}
				//dsvr->showCandi(true); //註解掉因為切開就應該 clear cand了
			}
			else   // it is NULL input context. (?)
			{				
				murmur("\thUICurIMC==false, hide all");
				dsvr->showBuf(false);
			}
		}
		else //switch out
		{
			murmur("\tswitch out, hide all");
			dsvr->showBuf(false);
			dsvr->showCandi(false);

			//dsvr->releaseIMC();  //?
		}
		break;

	case WM_IME_STARTCOMPOSITION:
		murmur("WM_IME_STARTCOMPOSITION");
		dsvr->SetCompEnabled(true);
		dsvr->SetCandiEnabled(true);									
		RefreshUI(hWnd); //James comment : 是否move到正確的座標?
		break;

	case WM_IME_COMPOSITION:
		murmur("WM_IME_COMPOSITION");		
		RefreshUI(hWnd);
		//ImmUnlockIMC(hUICurIMC); 
		break;

	case WM_IME_ENDCOMPOSITION:
		murmur("WM_IME_ENDCOMPOSITION");
		//dsvr->SetCompEnabled(false);
		//dsvr->showBuf(false);
		//dsvr->showCandi(false);	
		break;

	case WM_IME_COMPOSITIONFULL:
		murmur("WM_IME_COMPOSITIONFULL");
		break;

	case WM_IME_SELECT:
		//Sent to an application when the operating system 
		//     is about to change the current IME
		//dsvr->SetCompStarted(false);
		if(wParam) //the indicated IME is selected
		{
			murmur("WM_IME_SELECT: selected.");
		}
		else
		{
			murmur("WM_IME_SELECT: specified IME is no longer selected.");
		}		
		break;

	case WM_IME_CONTROL:
		murmur("WM_IME_CONTROL");
		lRet = ControlHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;

	case WM_IME_NOTIFY:
		murmur("WM_IME_NOTIFY");
		lRet = NotifyHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;

	case WM_DESTROY:
	{
		murmur("WM_DESTROY");
		
		dsvr->showStatus(false);
		dsvr->showBuf(false);
		dsvr->showCandi(false);
		
		AVLoader* loader = AVLoader::open();
		loader->unloadCurrentModule();	
		AVLoader::close();

		break;
	}
	case WM_IME_RELOADCONFIG:
	{
		murmur("WM_IME_RELOADCONFIG");

		AVLoader* loader = AVLoader::open();
		loader->unloadCurrentModule();
		AVLoader::close();

		dsvr->setHIMC(hUICurIMC);
		loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		//SendMessage(hWnd, WM_IME_NOTIFY, IMN_PRIVATE, 9L);
		int modAmount = loader->getInputMethodCount();
		const char** modNameList = new const char* [modAmount];
		loader->getAllModuleNames(modNameList);
		UISetStatusModStrMenuAll(modAmount, modNameList);

		break;
	}
	case WM_NCCREATE:
		murmur("WM_NCCREATE");
		if(lParam) doDefWndProc = true;
		break;

	case WM_NCDESTROY:   
		murmur("WM_NCDESTROY");
		break;
		
	case WM_NCCALCSIZE:   		
		// message is sent when the size and position of a 
		//window's client area must be calculated.
		//By processing this message, an application can 
		//control the content of the window's client area 
		//when the size or position of the window changes.
		murmur("WM_NCCALCSIZE");
		break;

	case WM_DEVICECHANGE:   
		//notifies an application of a change to the 
		//hardware configuration of a device or the computer.
		murmur("WM_DEVICECHANGE: wparam=%p",wParam);
		break;

	case WM_WINDOWPOSCHANGED:   
		murmur("WM_WINDOWPOSCHANGED");
		break;

	case WM_WININICHANGE:   
		murmur("WM_WININICHANGE");
		break;

	case WM_MOVE:
		murmur("WM_MOVE");
		break;

	case WM_SIZE:
		murmur("WM_SIZE");
		break;

	case WM_GETTEXT:
		murmur("WM_GETTEXT");
		break;

	default:
		murmur("Uncatched message,%p",msg);
		doDefWndProc = true;
		break;
	}

	if(doDefWndProc) lRet = DefWindowProc(hWnd, msg, wParam, lParam);

	return lRet;
}
