#include "OVIME.h"
//#include "NetIO.h"

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
		DebugLog("UIMSG");
	case WM_CREATE:
		CompX = CompY = -1;
		UICreateCompWindow(hWnd);
		UICreateCandWindow(hWnd);
		UICreateNotifyWindow(hWnd);
		break;
		
	case WM_IME_SETCONTEXT:
		if (wParam)
		{
			if (hUICurIMC)
			{
				lpIMC = ImmLockIMC(hUICurIMC);
				if (lpIMC)
				{
					lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
					UIMoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
					UIMoveCandWindow(hWnd, -1, -1, lpMyPrivate->CandStr);
					ImmUnlockIMCC(lpIMC->hPrivate);
				}
				else
				{
					UIHideCandWindow();
					UIHideCompWindow();
				}
				ImmUnlockIMC(hUICurIMC);
			}
			else   // it is NULL input context.
			{
				UIHideCandWindow();
				UIHideCompWindow();
			}
		}
		break;
		
	case WM_IME_STARTCOMPOSITION:
		break;
		
	case WM_IME_COMPOSITION:
		lpIMC = ImmLockIMC(hUICurIMC);
		POINT pt;
		if(CompX < 0) {
			pt.x = 15;
			pt.y = 15;
			ClientToScreen(lpIMC->hWnd, &pt);
			CompX = pt.x;
			CompY = pt.y;
		}
		lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
		UIMoveCompWindow(hWnd, CompX, CompY, lpMyPrivate->PreEditStr);
		UIMoveCandWindow(hWnd, -1, -1, lpMyPrivate->CandStr);
		ImmUnlockIMCC(lpIMC->hPrivate);
		ImmUnlockIMC(hUICurIMC);
		break;
		
	case WM_IME_ENDCOMPOSITION:
		UIHideCompWindow();
		UIHideCandWindow();
		break;
		
	case WM_IME_COMPOSITIONFULL:
		break;
		
	case WM_IME_SELECT:
		break;
		
	case WM_IME_CONTROL:
		DebugLog("WM_IME_CONTROL");
		lRet = ControlHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;
		
		
	case WM_IME_NOTIFY:
		DebugLog("WM_IME_NOTIFY");
		lRet = NotifyHandle(hUICurIMC, hWnd, msg, wParam, lParam);
		break;
		
	case WM_DESTROY:
		break;

	case WM_IME_RELOADCONFIG:
		loader = AVLoader::getLoader();
		loader->reloadConfig();
		{
			/*
			UDPSocket sock;
			//sock.send("127.0.0.1", 5100, RELOADCONFIG, "");
			UIClearInputMethodList();
			char pos[2];
			wchar_t wstr[1024];
			cmd_in rtn;
			for(int i = 0;; ++i) {
				pos[0] = i;
				sock.send("127.0.0.1", 5100, GETNAME, pos);
				sock.recv(&rtn);
				if(rtn.buf[0]) {
					MultiByteToWideChar(CP_UTF8, 0, rtn.buf, strlen(rtn.buf)+1, wstr, 1024);
					UIPushInputMethod(wstr);
				} else {
					break;
				}
			}
			*/
			//UIPushInputMethod(L"Test");
		}
		break;

	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
    }
    return lRet;
}
