#define OV_DEBUG 
#include "DisplayServer.h"
#include "OVIME.h"

AVDisplayServer *DisplayServer::lockIMC(HIMC h)
{
	hIMC = h;
	lpIMC = ImmLockIMC(hIMC);
	lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
	lpMyPrivate = (LPMYPRIVATE)ImmLockIMCC(lpIMC->hPrivate);
	return this;
}
AVDisplayServer *DisplayServer::releaseIMC()
{
	if(lpMyPrivate)
	{
		ImmUnlockIMCC(lpIMC->hPrivate);
		lpMyPrivate=NULL;
	}
	if(lpCompStr)
	{
		ImmUnlockIMCC(lpIMC->hCompStr);
		lpCompStr=NULL;
	}
	if(hIMC)
	{
		ImmUnlockIMC(hIMC);
		hIMC=NULL;
	}
	return this;
}
AVDisplayServer *DisplayServer::setBufString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(lpMyPrivate->PreEditStr, wstr);
	MakeCompStr(lpMyPrivate, lpCompStr);
	murmur("\tAVDisplayServer *DisplayServer::setBufString(%s)",str);
	UISetCompStr(lpMyPrivate->PreEditStr); 	//要不要先檢查有沒有PreEditStr有沒有東西？

	return this;
}
AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);	
	
	lpCompStr->dwResultStrLen = (int)wcslen(wstr);

	wcscpy(GETLPRESULTSTR(lpCompStr), wstr);	
	wcscpy(lpMyPrivate->PreEditStr, L"");
	
	MakeCompStr(lpMyPrivate, lpCompStr);
	UIClearCompStr();//即時update C# comp string 同步資料
		
	MyGenerateMessage(hIMC,
		WM_IME_COMPOSITION, 0, GCS_RESULTSTR);  //?
	MyGenerateMessage(hIMC,
		WM_IME_ENDCOMPOSITION, 0, 0);	 //?
	return this;
}
AVDisplayServer *DisplayServer::setCandiString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(lpMyPrivate->CandStr, wstr);
	UpdateCandidate(lpIMC, wstr);
//	MyGenerateMessage(hIMC,
//			WM_IME_COMPOSITION, 0, GCS_COMPSTR);
	UISetCandStr(lpMyPrivate->CandStr);
	return this;
}
AVDisplayServer *DisplayServer::showNotify(const char *str)
{
	murmur("\tAVDisplayServer *DisplayServer::showNotify");
	wchar_t wstr[1024];
	murmur("notify str=%s",str);
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	UISetNotifyStr(wstr);	
	//UIShowNotifyWindow();
	return this;
}
AVDisplayServer *DisplayServer::hideNotify()
{
	UIHideNotifyWindow();	
	return this;
}


DisplayServer *DisplayServer::moveBuf(int x, int y)
{
	murmur("\tAVDisplayServer *DisplayServer::moveBuf(%d,%d)",x,y);
	UIMoveCompWindow(x,y);
	return this;
}

DisplayServer *DisplayServer::moveCandi(int x, int y)
{
	murmur("\tAVDisplayServer *DisplayServer::moveCandi(%d,%d)",x,y);
	UIMoveCandWindow(x,y);
	return this;
}

DisplayServer *DisplayServer::SetCompStarted(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompStarted=true");
	else
		murmur("\t*DisplayServer::SetCompStarted=false");
	
	hasCompStarted=t;
	return this;
}

DisplayServer *DisplayServer::SetStatusEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetStatusEnabled=true");
	else
		murmur("\t*DisplayServer::SetStatusEnabled=false");
	
	isStatusEnabled=t;
	return this;
}

DisplayServer *DisplayServer::SetCompEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompEnabled=true");
	else
		murmur("\t*DisplayServer::SetCompEnabled=false");
	
	isCompEnabled=t;
	return this;
}

DisplayServer *DisplayServer::SetCandiEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCandiEnabled=true");
	else
		murmur("\t*DisplayServer::SetCandiEnabled=false");
	
	isCandiEnabled=t;
	return this;
}

DisplayServer *DisplayServer::showStatus(bool t)
{	
	if(dsvr->isStatusEnabled)
	{
		if(t)
		{
			murmur("\tAVDisplayServer *DisplayServer::showStatus");
			UIShowStatusWindow();
		}
		else
		{
			murmur("\tAVDisplayServer *DisplayServer::hideStatus");
			UIHideStatusWindow();
		}
	}
	return this;
}

AVDisplayServer *DisplayServer::showBuf(bool t)
{		
	if(dsvr->isCompEnabled)
	{
		if(t && lpMyPrivate->PreEditStr && wcslen(lpMyPrivate->PreEditStr))	
		{
			murmur("\tAVDisplayServer *DisplayServer::showBuf");		
			UIShowCompWindow();
		}
		else
		{
			murmur("\tAVDisplayServer *DisplayServer::hideBuf");
			UIHideCompWindow();
		}
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{		
	if(dsvr->isCandiEnabled)
	{
		if(t &&  lpMyPrivate->CandStr && wcslen(lpMyPrivate->CandStr))	
		{
			UIShowCandWindow();
			murmur("\tAVDisplayServer *DisplayServer::showCandi");		
		}
		else
		{		
			UIHideCandWindow();
			murmur("\tAVDisplayServer *DisplayServer::hideCandi");
		}
	}
	return this;
}
AVDisplayServer *DisplayServer::setCursorPos(int i)
{
	lpCompStr->dwCursorPos = i;
	murmur("\tDisplayServer::setCursorPos-> %d",i);
	//UISetCursorPos(lpCompStr->dwCursorPos);
	UISetCompCaretPosX(i);
	return this;
}
AVDisplayServer *DisplayServer::setMarkFrom(int i)
{
	UISetMarkFrom(i);
	return this;
}
AVDisplayServer *DisplayServer::setMarkTo(int i)
{
	UISetMarkTo(i);
	return this;
}
