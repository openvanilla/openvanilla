//#define OV_DEBUG

#include "OVIME.h" 



static bool isPrivateEightExecuting = false;
static bool inprivate2=false;
static bool inprivate5=false;






LRESULT NotifyHandle(HIMC hUICurIMC,
				  HWND hWnd,
				  UINT message,
				  WPARAM wParam,
				  LPARAM lParam)
{
	LRESULT lRet = 0L;
	//<comment author='b6s'>Unused codes
	//LPINPUTCONTEXT lpIMC;  
	//LOGFONT* lfptr; 
	//LOGFONT lf2;
	//</comment>
	
	if(!hUICurIMC) return lRet;

	dsvr->setHIMC(hUICurIMC);
	AVLoader* loader = AVLoader::open();
	loader->connectDisplayServer(dsvr);

	switch (wParam)
	{
	case IMN_OPENSTATUSWINDOW:
		{
		murmur("IMN_OPENSTATUSWINDOW");
		murmur("\thwnd=%x", hWnd);		 

		loader->setGlobalConfig("StatusBar");
		UICreateStatusWindow(hWnd, 
			atoi(loader->getGlobalConfigKey("StatusPosX")),
			atoi(loader->getGlobalConfigKey("StatusPosY")),
			!!atoi(loader->getGlobalConfigKey("IsDocked"))
			);
		
		dsvr->showStatus(true);	
		

		break;
		}
	case IMN_SETOPENSTATUS: // the same as IMN_OPENSTATUSWINDOW
		{
		murmur("IMN_SETOPENSTATUS");
		loader->setGlobalConfig("StatusBar");
		UICreateStatusWindow(hWnd, 
			atoi(loader->getGlobalConfigKey("StatusPosX")), 
			atoi(loader->getGlobalConfigKey("StatusPosY")),
			!!atoi(loader->getGlobalConfigKey("IsDocked"))
			);
		
		dsvr->showStatus(true);	 
		
		
		

		break;
		}
	case IMN_CLOSESTATUSWINDOW:
		{
		murmur("IMN_CLOSESTATUSWINDOW");
		dsvr->showStatus(false);	
		
		

		//<comment author='b6s'>
		// When attached app lost focus, only this message occurred,
		// so it must also hide following windows.
		//dsvr->showBuf(false);	
		//dsvr->showCandi(false);								

		//</comment>
		break;
		}
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
		isChinese = (conv & IME_CMODE_NATIVE) == 0 ? false : true;
		isFullShape = (conv & IME_CMODE_FULLSHAPE) == 0 ? false : true;
		//murmur("\t");
		murmur("\tIsChinese:%i; IsFullShape:%i", isChinese, isFullShape);
		break;  

	case IMN_SETSENTENCEMODE:
		murmur("IMN_SETSENTENCEMODE");
		break;

	case IMN_SETCANDIDATEPOS: // For the Search Text Field of Firefox
		{
		murmur("IMN_SETCANDIDATEPOS"); 
		//dsvr->SetCompEnabled(true);  
		dsvr->SetCandiEnabled(true);
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
		//dsvr->moveCandi(CandX,CandY);
		//not finished!!
		break;
		}
 
	case IMN_SETCOMPOSITIONFONT: 
		{
		murmur("IMN_SETCOMPOSITIONFONT");
		dsvr->SetCompEnabled(true); 
		//dsvr->SetCandiEnabled(true);
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
		//dsvr->showStatus(true);
		//<comment author='b6s'>
		// It is weird but... when the attached app got focus back,
		// this message occurred.
		//</comment>
	
		break;
		}
	case IMN_SETCOMPOSITIONWINDOW:
		{
		/*	Sent immediately before the IME generates 
			the composition string as a result of a keystroke.
			This message is a notification to an IME window to 
			open its composition window. An application should process
			this message if it displays composition characters itself. */
		murmur("IMN_SETCOMPOSITIONWINDOW");
		dsvr->SetCompEnabled(true);
		//dsvr->SetCandiEnabled(true);
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
		murmur("\tRefreshUI()"); 
		RefreshUI(hUICurIMC);  
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
		murmur("lParam=%i", lParam);
		switch(lParam) 
		{
		case 0:
			break;
		case 1: //Change UI Half/Full from key: shift+space
			{
			murmur("\tChange UI Half/Full.");
			

			UIChangeHalfFull(); 
			 
			break;
			}

		case 2:  //Change UI CHI/ENG
		{
			if(!inprivate2){
				inprivate2=true;
			murmur("\tChange UI CHI/ENG.");
			
			ImmModel* model = ImmModel::open(hUICurIMC); // force commit before changing 
			if(wcslen(GETLPCOMPSTR(model->getCompStr())) )
				loader->unloadCurrentModule();
			model->close();
			
			UIChangeChiEng();  

		    //通知 windows API 輸入法狀態改變
			//HIMC imc = ImmGetContext( hWnd );
			if( hUICurIMC )
			{
				isChinese=!isChinese;
				DWORD conv, sentence;
				ImmGetConversionStatus( hUICurIMC, &conv, &sentence);
				if( isChinese ){
					conv |= IME_CMODE_NATIVE;
				}
				else{
					conv &= ~IME_CMODE_NATIVE;									
				}
				ImmSetConversionStatus( hUICurIMC, conv, sentence);
			}				
			
			inprivate2=false;
			}
			break;
		}
		case 3: //Switch modules by Mouse
			{
			murmur("\tChange Modules by Mouse");
			/* close module and set new IC */

			loader->unloadCurrentModule();
			int moduleId = UIModuleChange();
			UISetStatusModStrCurrent(moduleId);
			loader->syncConfigMenu(moduleId);
			//loader->initContext(moduleId);
			break;
			}
		case 4: // Change UI Traditional/Simplified Chinese
			murmur("\tChange UI Traditional/Simplified Chinese");			
			UIChangeSimpifiedOrTraditional(hWnd); 
			break;
		case 5: { //Change BoPoMoFo keyboard layout by ctrl +'='
			/* close module and set new IC */	
			if(!inprivate5)
			{
				inprivate5 = true;
				int moduleId = loader->getSwitchedBoPoMoFoLayoutModIndex(UICurrentInputMethod());
				murmur("\tChange BoPoMoFo keyboard layout by ctrl+=. New moduleId:%d", moduleId);
				if(moduleId != UICurrentInputMethod()) {
					loader->unloadCurrentModule();
					UISetStatusModStrCurrent(moduleId);
					loader->syncConfigMenu( UIModuleChange());
					
					
				//loader->initContext(moduleId);	
				}
				inprivate5 =false;
			}
			break;
			}
		case 6: // Toggle Small/Large Candidate window. (ctrl+alt+k)	
			{
			murmur("\tToggle Small/Large Candidate window.");
			murmur("\tTest Notify window");
			char *str="Hide Deskband";			
			
			
			dsvr->showNotify(str); //會 new 一個 NotifyWindow ->real form		
			//UIExpandCandi();
			UIShowDeskBand(false);
			break;
			}
		case 7: // Test Notify window. (ctrl+alt+L)
			{
			murmur("\tTest show deskband");
			char *str="Show Deskband";			
			static bool t=true;
			
			if( UIShowDeskBand(t)) t=!t;
			

			//UICreateNotifyWindow(hWnd); //new IMENotifyWindow -> fake form
			dsvr->showNotify(str); //會 new 一個 NotifyWindow ->real form		
			
			
			break;
			}
		case 8: //Change Modules by ctrl +'\'
			{
			/* close module and poll current IC */
				//<comment author='b6s'>
				// A stupid but necessary check to prevent infinite loop. 
				if(!isPrivateEightExecuting) {
					isPrivateEightExecuting = true;
					loader->unloadCurrentModule();
					int moduleId = UIModuleRotate();					
					murmur("Ctrl+ \\; Switch IM to module:%d", moduleId);
					UISetStatusModStrCurrent(moduleId );
					loader->syncConfigMenu(moduleId);
					//loader->initContext(moduleId);
					isPrivateEightExecuting = false; 
				}
				//</comment>
			break;
			}
		case 9: //Set all module names
			{			

			int modAmount = loader->getInputMethodCount() +loader->getKeypreprocessorCount() + loader->getOutputFilterCount();
			murmur("Loading module names to status bar:%d modules", modAmount);
			const char** modNameList = new const char* [modAmount];

			loader->getAllModuleNames(modNameList);
			
			for(int i=0;i<modAmount; i++)
				murmur("module %d: %s", i, modNameList[i]);

			UISetkpCount(loader->getKeypreprocessorCount());
			UISetofCount(loader->getOutputFilterCount());
			UISetimCount(loader->getInputMethodCount());

			UISetStatusModStrMenuAll(modAmount, modNameList);
			UISetStatusModStrCurrent(UICurrentInputMethod());
			loader->syncConfigMenu(UICurrentInputMethod() );//UIModuleChange());  // update current IC.
			

			//dsvr->showStatus(true);
			
			
			break;
			}
		case 10: // Call OVpreference and send reload to all OV instances after process ended.
			{
			murmur("Shell execute OVpreference if exist");
								
			TCHAR dir[MAX_PATH], file[MAX_PATH];
			
			mbstowcs(dir,loader->getBaseDirectory(),MAX_PATH);

			_tcscpy(file,dir);
			_tcscat(file,_T("OVPreferences.exe"));
				
			char buff[MAX_PATH];
			wcstombs(buff,file, MAX_PATH);
			murmur("OVpreference:%s",buff);

				//if(PathFileExists(buf)){ 
			if(!_waccess(file,0)){
				SHELLEXECUTEINFO ShExecInfo = {0};
				ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
				ShExecInfo.hwnd = NULL;
				ShExecInfo.lpVerb = NULL;
				ShExecInfo.lpFile = file;		
				ShExecInfo.lpParameters = _T("");	
				ShExecInfo.lpDirectory = dir;
				ShExecInfo.nShow = SW_SHOWNORMAL;
				ShExecInfo.hInstApp = NULL;	
				ShellExecuteEx(&ShExecInfo);
				WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

				murmur("send WM_IME_RELOADCONFIG to all windows. My Hwnd:%x", hWnd);
				EnumChildWindows( GetDesktopWindow(), ReloadIME, NULL);

			}

			break;

			}
		case 11: // Status bar loaction updated.
			{
			POINT pt;

			loader->setGlobalConfig("StatusBar");
			dsvr->getStatusPos(&pt);
			if((pt.x!=-1)&&(pt.y!=-1)) {
				murmur("Status location changed notification from UI: Save new position : (%i, %i).", pt.x, pt.y);
				char tmp[32]; 
				sprintf(tmp, "%d", pt.x);
				loader->setGlobalConfigKey("StatusPosX",tmp);
				sprintf(tmp, "%d", pt.y);
				loader->setGlobalConfigKey("StatusPosY",tmp);
				//sprintf(tmp, "%d", dsvr->GetisStatusDocked());
				//loader->setGlobalConfigKey("IsDocked",tmp);
				
			}	  	
			
			break;
			}
		case 12: // Select Candi item with mouse.
			{
			AVKeyCode k;
			k.setCode(UIGetCandSelectedItemKey());
			
			loader->keyEvent(UICurrentInputMethod(), k);

			break;
			}
		case 13: // KP OF checked status updated.
			{
			murmur("Sync KP OF checked status to config.");
			loader->syncMenuConfig(UICurrentInputMethod());

			murmur("send WM_IME_RELOADCONFIG to all windows. My Hwnd:%x", hWnd);
			EnumChildWindows( GetDesktopWindow(), ReloadIME, NULL);
			break;
			}
		case 14: // Status docking status updated
			{
			murmur("Status docking status updated");
			dsvr->showStatus(false);
			if(dsvr->GetisStatusDocked())
			{
				dsvr->SetisStatusDocked(false);
			}
			else
			{
				dsvr->SetisStatusDocked(true);
				
			}
			dsvr->showStatus(true);
			

		
			POINT pt;
			char tmp[32]; 
			loader->setGlobalConfig("StatusBar");
			if(dsvr->GetisStatusDocked())
			{
				
				loader->setGlobalConfigKey("IsDocked","1"); 
		
			}
			else
			{
				dsvr->getStatusPos(&pt);
				if((pt.x!=-1)&&(pt.y!=-1)) {
				murmur("Save current Status bar location (%i, %i), and isDocking:%i."
					, pt.x, pt.y, dsvr->GetisStatusDocked());
				sprintf(tmp, "%d", pt.x);
				loader->setGlobalConfigKey("StatusPosX",tmp);
				sprintf(tmp, "%d", pt.y);
				loader->setGlobalConfigKey("StatusPosY",tmp);
				}
				loader->setGlobalConfigKey("IsDocked","0");
			
			}
			
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

	return lRet;
}

