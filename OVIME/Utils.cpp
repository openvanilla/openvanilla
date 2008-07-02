//#define OV_DEBUG
#include "OVIME.h" 
#include <cstdio> 

void MakeCompStr(LPMYPRIVATE lpMyPrivate, LPCOMPOSITIONSTRING lpCompStr, int CaretX, int markFrom, int markTo)
{   
	
	wcscpy(GETLPCOMPSTR(lpCompStr), lpMyPrivate->PreEditStr);
		
	lpCompStr->dwCompStrLen = (DWORD)wcslen(GETLPCOMPSTR(lpCompStr));

#ifdef WINCE
	murmur("composition len:%d; cls len:%d", lpCompStr->dwCompStrLen, lpCompStr->dwCompClauseLen);
#else
	murmur("composition len:%d; cls len:%d", lpCompStr->dwCompStrLen, lpCompStr->dwCompClsLen);
#endif

	lpCompStr->dwDeltaStart =0; 

	if(CaretX >-1) lpCompStr->dwCursorPos = CaretX;
	else lpCompStr->dwCursorPos = lpCompStr->dwCompStrLen;

	lpCompStr->dwCompAttrLen = lpCompStr->dwCompStrLen;

	if(lpCompStr->dwCompAttrLen) {
		if(markFrom>-1 && markTo>-1) {
			memset((char*)GETLPCOMPATTR(lpCompStr), ATTR_CONVERTED, markFrom);
			memset((char*)(GETLPCOMPATTR(lpCompStr) + sizeof(BYTE)* markFrom), ATTR_TARGET_CONVERTED, markTo - markFrom);
			memset((char*)(GETLPCOMPATTR(lpCompStr) + sizeof(BYTE)* markTo), ATTR_CONVERTED, lpCompStr->dwCompAttrLen -markTo);
		}
		else {
			memset((char*)GETLPCOMPATTR(lpCompStr), ATTR_TARGET_CONVERTED, lpCompStr->dwCompAttrLen);
			
		}
	}
}

void InitCompStr(LPCOMPOSITIONSTRING lpCompStr)
{
	lpCompStr->dwSize = sizeof(MYCOMPSTR);
	lpCompStr->dwCursorPos = 0;
	lpCompStr->dwDeltaStart= 0;


	lpCompStr->dwCompStrOffset =
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szCompStr - (DWORD)lpCompStr;
	lpCompStr->dwCompStrLen = 0;
	*GETLPCOMPSTR(lpCompStr) = L'\0';

	lpCompStr->dwCompAttrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->compAttr - (DWORD)lpCompStr;
	lpCompStr->dwCompAttrLen = 0;
	//*GETLPCOMPATTR(lpCompStr) = L'\0';
#ifdef WINCE
	lpCompStr->dwCompClauseOffset = 
				(DWORD)((LPMYCOMPSTR)lpCompStr)->compClause - (DWORD)lpCompStr;
	lpCompStr->dwCompClauseLen = 0;
#else
	lpCompStr->dwCompClsOffset =
				(DWORD)((LPMYCOMPSTR)lpCompStr)->compClause - (DWORD)lpCompStr;
	lpCompStr->dwCompClsLen = 0;
#endif
	//*GETLPCOMPCLS(lpCompStr) = L'\0';


	lpCompStr->dwResultStrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szResultStr - (DWORD)lpCompStr;
	lpCompStr->dwResultStrLen = 0;
	*GETLPRESULTSTR(lpCompStr) = L'\0';

#ifdef WINCE
	lpCompStr->dwResultClauseOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->resultClause - (DWORD)lpCompStr;
	lpCompStr->dwResultClauseLen = 0;
#else
	lpCompStr->dwResultClsOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->resultClause - (DWORD)lpCompStr;
	lpCompStr->dwResultClsLen = 0;
#endif
	//*GETLPRESULTCLS(lpCompStr) = L'\0';


	lpCompStr->dwCompReadStrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szReadStr - (DWORD)lpCompStr;
	lpCompStr->dwCompReadStrLen = 0;
	*GETLPREADSTR(lpCompStr) = L'\0';

	lpCompStr->dwCompReadAttrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->readAttr - (DWORD)lpCompStr;
	lpCompStr->dwCompReadAttrLen = 0;
	//*GETLPREADATTR(lpCompStr) = L'\0';

#ifdef WINCE
	lpCompStr->dwCompReadClauseOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->readClause - (DWORD)lpCompStr;
	lpCompStr->dwCompReadClauseLen = 0;
#else
	lpCompStr->dwCompReadClsOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->readClause - (DWORD)lpCompStr;
	lpCompStr->dwCompReadClsLen = 0;
#endif
	//*GETLPREADCLS(lpCompStr) = L'\0';

	
	lpCompStr->dwResultReadStrOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->szResultReadStr - (DWORD)lpCompStr;
	lpCompStr->dwResultReadStrLen = 0;
	*GETLPRESULTREADSTR(lpCompStr) = L'\0';

#ifdef WINCE
	lpCompStr->dwResultClauseOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->resultReadClause - (DWORD)lpCompStr;
	lpCompStr->dwResultReadClauseLen = 0;
#else
	lpCompStr->dwResultClsOffset = 
		(DWORD)((LPMYCOMPSTR)lpCompStr)->resultReadClause - (DWORD)lpCompStr;
	lpCompStr->dwResultReadClsLen = 0;
#endif
	//*GETLPRESULTREADCLS(lpCompStr) = L'\0';

}

void InitCandInfo(LPCANDIDATEINFO lpCandInfo)
{
	LPCANDIDATELIST lpCandList;
	
	int i;

	lpCandInfo->dwSize = sizeof(MYCAND);
	lpCandInfo->dwCount = 1;
	lpCandInfo->dwOffset[0] = 
			(DWORD)(&(((LPMYCAND)lpCandInfo)->cl)) - (DWORD)lpCandInfo;

	lpCandList = (LPCANDIDATELIST)((LPBYTE)lpCandInfo  + lpCandInfo->dwOffset[0]);
	for(i = 0; i < MAXCANDSTRNUM; i++)
	{
		lpCandList->dwOffset[i] = 
                   (DWORD)((LPMYCAND)lpCandInfo)->szCandStr[i] - (DWORD)lpCandList;
	}
	lpCandList->dwSize = sizeof(MYCAND) - sizeof(CANDIDATEINFO); 
	lpCandList->dwCount = 0L;
	lpCandList->dwSelection = 0L;
	lpCandList->dwPageStart = 0L;
	lpCandList->dwPageSize = 0L;
}

void ClearCandidate(LPCANDIDATEINFO lpCandInfo)
{
    ((LPMYCAND)lpCandInfo)->cl.dwCount =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwSelection =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageStart =0L;
    ((LPMYCAND)lpCandInfo)->cl.dwPageSize =0L;
}

void UpdateCandidate(LPINPUTCONTEXT lpIMC, const wchar_t* candis, DWORD dwPageSize)
{
	LPCANDIDATEINFO lpCandInfo;
	LPCANDIDATELIST lpCandList;
	if (lpCandInfo = (LPCANDIDATEINFO)ImmLockIMCC(lpIMC->hCandInfo))
	{
		wchar_t* lpStr;
		lpCandList = (LPCANDIDATELIST)((LPSTR)lpCandInfo
			+ lpCandInfo->dwOffset[0]);
				int rlen = (int)wcslen(candis);
		int n = 0;
		int ln = 0;
		for( int i = 1; i < rlen; i++ )
		{
			if(candis[i] == L' ' || i == rlen - 1)
			{				
				int dst = i - 1;
				if( i == rlen - 1)
					dst = i + 1;
				lpStr = GETLPCANDSTR(lpCandList, n);
				//memset(lpStr, 0, 32*sizeof(wchar_t));
				//wcsncpy(lpStr, candis+ln +2, dst - ln + 1 -2);
				wcsncpy(lpStr, candis+ln , dst - ln + 1 );
				//lpStr[dst - ln + 1 -2] =0;
				lpStr[dst - ln + 1] =0;
				ln = i + 1;
				n++;
			}
		}
		lpCandList->dwCount = n - 1;
		lpCandList->dwPageSize = dwPageSize;
		lpCandList->dwPageStart = 0;
		lpCandList->dwSelection = 0;

		ImmUnlockIMCC(lpIMC->hCandInfo);
	}
}

#ifndef WINCE

void RefreshUI(HIMC hUICurIMC)  //½Õ¾ãcomp cand
{	

	static RECT* screenRC = NULL;

	int NotifyX; int NotifyY;

	if(!screenRC)
	{
		screenRC = new RECT;
		SystemParametersInfo(SPI_GETWORKAREA, 0, screenRC, 0);
		murmur("Screen RECT: (%d,%d)",screenRC->right, screenRC->bottom);
	}


	murmur("RefreshUI");
	murmur("\tisCompEnable:%i ; isCandienabled:%i", dsvr->isCompEnabled, dsvr->isCandiEnabled);

	//HIMC hUICurIMC = (HIMC)GetWindowLong(hWnd, IMMGWL_IMC);
	ImmModel* model = ImmModel::open(hUICurIMC);

	//dsvr->setHIMC(hUICurIMC);
	
	LPINPUTCONTEXT lpIMC = model->getIMC(); 
	//lpIMC = ImmLockIMC(hUICurIMC);

	LOGFONT* lfptr = (LOGFONT*)(&lpIMC->lfFont);
	LOGFONT lfClone;
	memcpy(&lfClone, lfptr, sizeof(lfClone));
	int UIHeight=lfClone.lfHeight; 
	
	POINT ptSrc;
	ptSrc = lpIMC->cfCompForm.ptCurrentPos;

	murmur("\tcomp dwstyle = 0x%x.",lpIMC->cfCompForm.dwStyle);
	murmur("\tCompForm.ptCurrentPos(%i, %i).",ptSrc.x, ptSrc.y); 
	ClientToScreen(lpIMC->hWnd, &ptSrc);  
	switch( lpIMC->cfCompForm.dwStyle )                 
		{
		 case CFS_POINT: 
         case CFS_RECT:
			break;
		 case CFS_DEFAULT:
         default:
			 if( GetCaretPos(&ptSrc)) {
				
				ClientToScreen(lpIMC->hWnd, &ptSrc);
				murmur("\tCaretpos(%i, %i), lfHeight: %i, lfWidth: %i.",ptSrc.x, ptSrc.y,  lfClone.lfHeight, lfClone.lfWidth);
				if(lfClone.lfHeight >0)		ptSrc.y += lfClone.lfHeight;
		
			}
			else{  
				RECT rec; 
				SystemParametersInfo(SPI_GETWORKAREA, 0, &rec, 0);
				ptSrc.x = rec.left + 100; 
				ptSrc.y = rec.bottom - 100; 
				murmur("\tCaret at default location(%i, %i).",ptSrc.x, ptSrc.y); 
			}
		}
		
	//lpIMC->cfCompForm.ptCurrentPos = 	ptSrc;
	//ClientToScreen(lpIMC->hWnd, &ptSrc);
	if( ! (lpIMC->fdwInit & INIT_COMPFORM) )
			lpIMC->fdwInit |= INIT_COMPFORM;
	
	// Set notify location if candi is not enabled.
	NotifyX = ptSrc.x -UIGetNotifyWidth();
	if( NotifyX < 0) 	NotifyX = 0;
	if( NotifyX + UIGetNotifyWidth() > screenRC->right) NotifyX = screenRC->right - UIGetNotifyWidth();

	NotifyY = ptSrc.y + abs(UIHeight)* 3/ 2;
	
	
	if (dsvr->isCompEnabled )//|| (!dsvr->isCompEnabled&&!dsvr->isCandiEnabled))
	{
		// Don't move buff if the location is the same as the last position. Solve the blinking issue.
		murmur("\t comp at (%i,%i)", ptSrc.x, ptSrc.y);
		if(CompX == ptSrc.x&&CompY==ptSrc.y) murmur("\tcomp nomove.");
		else { 

		CompX = ptSrc.x;
		CompY = ptSrc.y;
		
		murmur(
			"\tmoveBuf(%i, %i, %i) and cursor at %i", 
			CompX, CompY, lfClone.lfHeight, model->getCompStr()->dwCursorPos);
		dsvr->moveBuf(
			CompX, CompY  ,
			UIHeight, lfClone.lfFaceName);
		}
	}
	if (dsvr->isCandiEnabled)// || (!dsvr->isCompEnabled&&!dsvr->isCandiEnabled)) 
	{ 
		ptSrc.x = ptSrc.x + UIGetCaretPosX();
		
		  
		if(!dsvr->isCompEnabled) {
			ptSrc = lpIMC->cfCandForm[0].ptCurrentPos;
			ClientToScreen(lpIMC->hWnd, &ptSrc);
			
			murmur("\tcfCandForm[0].ptCurrentPos(%i, %i).",ptSrc.x, ptSrc.y);
			//ptSrc.y += lfClone.lfHeight;
		}
		else
			UIHeight = UIGetHeight(); 
		//char chs[1024];
		//sprintf(chs,"candi.dwstyle%x, font height:%i, UIHeight%i..",lpIMC->cfCandForm[0].dwStyle, lfClone.lfHeight, UIHeight);
		//dsvr->showNotify(chs);
			
	
		murmur("\t UIHeight: %i, UIGetCaretPoxX:%i", UIHeight,UIGetCaretPosX());
		murmur("\tcandi dwstyle = 0x%x.",lpIMC->cfCandForm[0].dwStyle);
	
		switch( lpIMC->cfCandForm[0].dwStyle )                
		{
		case CFS_CANDIDATEPOS: 
			if(lfClone.lfHeight >= 0) ptSrc.y = ptSrc.y - UIHeight;//- lfClone.lfHeight IE here.     
            break;  
		case CFS_EXCLUDE:{  
	
			RECT rc = lpIMC->cfCandForm[0].rcArea;
			murmur("\tcandi excluded rect (R%i, B%i, L%i, T%i)",rc.right, rc.bottom, rc.left, rc.top);
			UIHeight = rc.bottom - rc.top ;
			POINT pt;
			pt.y= rc.top;
			pt.x= rc.left;
			ClientToScreen(lpIMC->hWnd, &pt);
			murmur("\texcluded rect T:%i, L:%i", pt.y, pt.x);
			if(lfClone.lfHeight >= 0) ptSrc.y = ptSrc.y - UIHeight +lfClone.lfHeight; 
			break;
			 }
		case CFS_DEFAULT:  
		default:
			if(lfClone.lfHeight >= 0)	ptSrc.y -=  UIHeight;//lfClone.lfHeight;
			break; // use comp position		 
		}
		

		murmur("\tcandi position(%i, %i).",ptSrc.x, ptSrc.y); 
		murmur("\tcandi size(%i, %i).", UIGetCandWidth(),UIGetCandHeight());
		CandX= ptSrc.x;
		CandY= ptSrc.y;


		
		if(CandX+UIGetCandWidth() > screenRC->right) 
					CandX = screenRC->right - UIGetCandWidth();

		NotifyX = CandX -UIGetNotifyWidth();// fixme-notify and candi window will overlapped becasue of offset_x on candi(for alignement right edge when scorolling).
		if(NotifyX <0)	
			NotifyX = CandX + UIGetCandWidth();
	
		
		NotifyY = CandY+ UIHeight*3/2;
		if(CandY+UIGetCandHeight() > screenRC->bottom)
		{
			CandY = CandY - 2*UIHeight - UIGetCandHeight();
			NotifyY = NotifyY - 2*UIHeight - UIGetNotifyHeight();
		}

		murmur("moveCandi(%i, %i, %i)", 
				CandX, CandY , UIHeight);
		dsvr->moveCandi(CandX, CandY, UIHeight);
		

	}
	murmur("movenotify(%i, %i)", NotifyX, NotifyY);
				
		dsvr->moveNotify(NotifyX, NotifyY);


	ImmModel::close();
}
#endif