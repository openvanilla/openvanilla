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
	ImmUnlockIMCC(lpIMC->hPrivate);
	ImmUnlockIMCC(lpIMC->hCompStr);
	ImmUnlockIMC(hIMC);
	return this;
}
AVDisplayServer *DisplayServer::setBufString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(lpMyPrivate->PreEditStr, wstr);
	MakeCompStr(lpMyPrivate, lpCompStr);
	murmur(" ---> DisplayServer.cpp ");
	UISetCompStr(lpMyPrivate->PreEditStr); 	//要不要先檢查有沒有PreEditStr有沒有東西？

	return this;
}
AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(GETLPRESULTSTR(lpCompStr), wstr);
	lpCompStr->dwResultStrLen = (int)wcslen(wstr);
	wcscpy(lpMyPrivate->PreEditStr, L"");
	
	MakeCompStr(lpMyPrivate, lpCompStr);
	UIClearCompStr();//即時update C# comp string 同步資料

	MyGenerateMessage(hIMC,
		WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
	MyGenerateMessage(hIMC,
		WM_IME_ENDCOMPOSITION, 0, 0);	
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
	murmur("AVDisplayServer *DisplayServer::showNotify");
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	UIShowNotifyWindow(wstr);
	return this;
}
AVDisplayServer *DisplayServer::hideNotify()
{
	UIHideNotifyWindow();	
	return this;
}


DisplayServer *DisplayServer::moveBuf(int x, int y)
{
	murmur("AVDisplayServer *DisplayServer::moveBuf(%d,%d)",x,y);
	UIMoveCompWindow(x,y);
	return this;
}

DisplayServer *DisplayServer::moveCandi(int x, int y)
{
	murmur("AVDisplayServer *DisplayServer::moveCandi(%d,%d)",x,y);
	UIMoveCandWindow(x,y);
	return this;
}

DisplayServer *DisplayServer::showStatus(bool t)
{
	murmur("AVDisplayServer *DisplayServer::showStatus");
	if(t)
		UIShowStatusWindow();
	else
		UIHideStatusWindow();
	return this;
}

AVDisplayServer *DisplayServer::showBuf(bool t)
{		
	if(t && wcslen(lpMyPrivate->PreEditStr))
	{
		murmur("AVDisplayServer *DisplayServer::showBuf");
		UIShowCompWindow();
	}
	else
	{
		murmur("AVDisplayServer *DisplayServer::hideBuf");
		UIHideCompWindow();
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{	
	//james test
	if(t && wcslen(lpMyPrivate->CandStr))
	{
		murmur("AVDisplayServer *DisplayServer::showCandi");
		UIShowCandWindow();
	}
	else
	{
		murmur("AVDisplayServer *DisplayServer::hideCandi");
		UIHideCandWindow();
	}
	return this;
}
AVDisplayServer *DisplayServer::setCursorPos(int i)
{
	lpCompStr->dwCursorPos = i;
	murmur("DisplayServer::setCursorPos-> %d",i);
	UISetCursorPos(lpCompStr->dwCursorPos);
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
