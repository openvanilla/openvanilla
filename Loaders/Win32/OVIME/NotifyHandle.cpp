#define OV_DEBUG

#include "OVIME.h"

LONG NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LONG lRet = 0L;
	LPINPUTCONTEXT lpIMC;
	static BOOL first = false;
	LOGFONT* lfptr;
	LOGFONT lf2;
	

	if (!(lpIMC = ImmLockIMC(hUICurIMC)))
		return 0L;

	switch (wParam)
	{
	case IMN_OPENSTATUSWINDOW:
		murmur("IMN_OPENSTATUSWINDOW");
		murmur("\thwnd=%x", hWnd);		
		dsvr->SetStatusEnabled(true);
		UICreateStatusWindow(hWnd);								
		dsvr->showStatus(true);		
		break;

	case IMN_SETOPENSTATUS: // the same as IMN_OPENSTATUSWINDOW
		murmur("IMN_SETOPENSTATUS");
		UICreateStatusWindow(hWnd);
		dsvr->showStatus(true);		
		break;

	case IMN_CLOSESTATUSWINDOW:
		murmur("IMN_CLOSESTATUSWINDOW");
		dsvr->showStatus(false);			
		//<comment author='b6s'>
		// When attached app lost focus, only this message occurred,
		// so it must also hide following windows.
		//dsvr->showBuf(false);	
		//dsvr->showCandi(false);								

		//</comment>
		break;

	case IMN_OPENCANDIDATE:
		murmur("IMN_OPENCANDIDATE");
		dsvr->SetCandiEnabled(true);
		dsvr->showCandi(true);		
		break;

	case IMN_CLOSECANDIDATE:
		murmur("IMN_CLOSECANDIDATE");
		dsvr->showCandi(false);		
		break;

	case IMN_CHANGECANDIDATE:
		dsvr->SetCandiEnabled(true);
		murmur("IMN_CHANGECANDIDATE");
		break;

	case IMN_SETCONVERSIONMODE:
		murmur("IMN_SETCONVERSIONMODE");
		DWORD conv, sentence;
		ImmGetConversionStatus( ImmGetContext(hWnd), &conv, &sentence);
		isChinese = (conv & IME_CMODE_NATIVE);
		isFullShape = (conv & IME_CMODE_FULLSHAPE);
		break;

	case IMN_SETSENTENCEMODE:
		murmur("IMN_SETSENTENCEMODE");
		break;

	case IMN_SETCANDIDATEPOS: // For the Search Text Field of Firefox
		{
		murmur("IMN_SETCANDIDATEPOS");
		dsvr->SetCandiEnabled(true);
		RefreshUI(hWnd);
		/*POINT ptSrc;
		SIZE szOffset;
		HDC hDC;
		ptSrc = lpIMC->cfCandForm[0].ptCurrentPos;
		ClientToScreen(lpIMC->hWnd, &ptSrc);
		hDC = GetDC(lpIMC->hWnd);
		GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
		ReleaseDC(lpIMC->hWnd,hDC);
		LPMYPRIVATE lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
		CompX=ptSrc.x ;//+szOffset.cx;
		CompY=ptSrc.y + szOffset.cy;			
		RefreshUI(hWnd);*/
		//dsvr->moveCandi(CandX,CandY);
		//not finished!!
		break;
		}

	case IMN_SETCOMPOSITIONFONT:
		murmur("IMN_SETCOMPOSITIONFONT");
		dsvr->SetCompEnabled(true);
		//<comment author='b6s'>
		// It is weird but... when the attached app got focus back,
		// this message occurred.
		//</comment>	
		RefreshUI(hWnd);
		break;

	case IMN_SETCOMPOSITIONWINDOW:   
		{			
		/*	Sent immediately before the IME generates 
			the composition string as a result of a keystroke.
			This message is a notification to an IME window to 
			open its composition window. An application should process
			this message if it displays composition characters itself. */
		murmur("IMN_SETCOMPOSITIONWINDOW");
		dsvr->SetCompEnabled(true);
		/*POINT ptSrc;
		SIZE szOffset;
		HDC hDC;
		TEXTMETRIC tm;
		int localDPIY; //for device dpiY
		ptSrc = lpIMC->cfCompForm.ptCurrentPos;
		ClientToScreen(lpIMC->hWnd, &ptSrc);
		hDC = GetDC(lpIMC->hWnd);
		murmur("\thWnd->%x", lpIMC->hWnd);
		GetTextExtentPoint(hDC, _T("A"), 1, &szOffset);
		GetTextMetrics(hDC, &tm);
		localDPIY = GetDeviceCaps(hDC, LOGPIXELSY);
		ReleaseDC(lpIMC->hWnd,hDC);
		
		lfptr = (LOGFONT*)(&lpIMC->lfFont);
		memcpy( &lf2, lfptr, sizeof( lf2) );					
		CompX = ptSrc.x ;
		CompY = ptSrc.y + abs(lf2.lfHeight)*localDPIY/tm.tmDigitizedAspectY;*/					
		RefreshUI(hWnd);
		break;
		}
	case IMN_GUIDELINE:
		murmur("IMN_GUIDELINE");
		break;

	case IMN_SETSTATUSWINDOWPOS:
		murmur("IMN_SETSTATUSWINDOWPOS");
		//		MoveStatusWindow(hWnd, );
		break;

	case IMN_PRIVATE:
		{
		murmur("IMN_PRIVATE");
		murmur("\thwnd=%x", hWnd);
		switch(lParam) 
		{
		case 0: //Change Modules by ctrl +'\'
			UIChangeModule(hWnd);
			break;
		case 1: //Change UI Half/Full
			UIChangeHalfFull(hWnd); 
			break;
		case 2:  //Change UI CHI/ENG
			murmur("\tChange UI CHI/ENG.");
			UIChangeChiEng(hWnd);  
			break;
		case 3: //Change Modules by Mouse
			murmur("\tChange Modules by Mouse");
			UIChangeModuleByMouse(hWnd); 
			break;
		case 4: // Change UI Traditional/Simplified Chinese
			murmur("\tChange UI Traditional/Simplified Chinese");			
			UIChangeSimpifiedOrTraditional(hWnd); 
			break;
		case 5: //Change BoPoMoFo keyboard layout by ctrl +'='
			UIChangeBoPoMoFoLayout(hWnd);
			break;
		case 6: // Toggle Small/Large Candidate window. (ctrl+alt+g)			
			murmur("\tToggle Small/Large Candidate window.");
			UIExpandCandi();
			break;
		case 7: // Test Notify window. (ctrl+alt+L)
			{
			murmur("\tTest Notify window");		
			char *str="GG notify message";			
			//UICreateNotifyWindow(hWnd); //new IMENotifyWindow -> fake form
			dsvr->showNotify(str); //·| new ¤@­Ó NotifyWindow ->real form			
			//UIShowNotifyWindow(); //show our IMENotifyWindow ->fake form
			break;
			}
		default:
			murmur("\tUknown IMN_PRIVATE");
			break;
		}
		break;
		}
	default:
		murmur("WM_IME_NOTIFY: no this wParam");
		break;
	}
	ImmUnlockIMC(hUICurIMC);

	return lRet;
}
