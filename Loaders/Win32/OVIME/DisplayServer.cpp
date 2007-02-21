//#define OV_DEBUG 
#include "DisplayServer.h"
#include "OVIME.h"
#include "ImmController.h"

DisplayServer::DisplayServer()
{
	hasCompStarted=false;
	isStatusEnabled=false;
	isCompEnabled=false;
	isCandiEnabled=false;	
}

DisplayServer::~DisplayServer()
{
	m_hIMC = NULL;
}

void DisplayServer::setHIMC(HIMC hIMC)
{
	m_hIMC = hIMC;
}

AVDisplayServer *DisplayServer::setBufString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);

	wcscpy(model->getMyPrivate()->PreEditStr, wstr);
	MakeCompStr(model->getMyPrivate(), model->getCompStr());
	UISetCompStr(model->getMyPrivate()->PreEditStr);
	//要不要先檢查有沒有PreEditStr有沒有東西？

	ImmModel::close();

	murmur("COMPOSITION GCS_COMPSTR");
	MyGenerateMessage(m_hIMC, WM_IME_COMPOSITION, 0, GCS_COMPSTR);

	return this;
}

AVDisplayServer *DisplayServer::setBufString(const char *str, int caretX)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);

	wcscpy(model->getMyPrivate()->PreEditStr, wstr);
	MakeCompStr(model->getMyPrivate(), model->getCompStr());
	UISetCompStr(model->getMyPrivate()->PreEditStr);
	//要不要先檢查有沒有PreEditStr有沒有東西？

	model->getCompStr()->dwCursorPos = caretX;
	UISetCompCaretPosX(caretX);

	ImmModel::close();

	murmur("COMPOSITION GCS_COMPSTR");
	MyGenerateMessage(m_hIMC, WM_IME_COMPOSITION, 0, GCS_COMPSTR);

	return this;
}

AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);

	model->getCompStr()->dwResultStrLen = (int)wcslen(wstr);
	wcscpy(GETLPRESULTSTR(model->getCompStr()), wstr);
	wcscpy(model->getMyPrivate()->PreEditStr, L"");
	
	MakeCompStr(model->getMyPrivate(), model->getCompStr());
	UIClearCompStr();//即時update C# comp string 同步資料

	ImmModel::close();

	murmur("COMPOSITION GCS_RESULTSTR");
	MyGenerateMessage(m_hIMC, WM_IME_COMPOSITION, 0, GCS_RESULTSTR);

	ImmController* controller = ImmController::open();
	controller->setCompStartedFlag(false);
	ImmController::close();
	controller = NULL;

	murmur("ENDCOMPOSITION");
	MyGenerateMessage(m_hIMC,	WM_IME_ENDCOMPOSITION, 0, 0);

	return this;
}

AVDisplayServer *DisplayServer::setCandiString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);

	wcscpy(model->getMyPrivate()->CandStr, wstr);
	UpdateCandidate(model->getIMC(), wstr);
//	MyGenerateMessage(hIMC,
//			WM_IME_COMPOSITION, 0, GCS_COMPSTR);
	UISetCandStr(model->getMyPrivate()->CandStr);

	ImmModel::close();

	return this;
}

AVDisplayServer *DisplayServer::showNotify(const char *str)
{
	wchar_t wstr[1024];
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
	UIMoveCompWindow(x,y);
	return this;
}

DisplayServer *DisplayServer::moveCandi(int x, int y)
{
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
		if(t)	UIShowStatusWindow();
		else	UIHideStatusWindow();
	}
	return this;
}

AVDisplayServer *DisplayServer::showBuf(bool t)
{
	if(dsvr->isCompEnabled)
	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->PreEditStr &&
			wcslen(model->getMyPrivate()->PreEditStr))
			UIShowCompWindow();
		else
			UIHideCompWindow();

		ImmModel::close();
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{
	if(dsvr->isCandiEnabled)
	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->CandStr &&
			wcslen(model->getMyPrivate()->CandStr))
			UIShowCandWindow();
		else
			UIHideCandWindow();

		ImmModel::close();
	}
	return this;
}

/*AVDisplayServer *DisplayServer::setCursorPos(int i) //搬到 setBufStr
{
	lpCompStr->dwCursorPos = i;
	murmur("\tDisplayServer::setCursorPos-> %d",i);
	//UISetCursorPos(lpCompStr->dwCursorPos);
	UISetCompCaretPosX(i);
	return this;
}*/

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
