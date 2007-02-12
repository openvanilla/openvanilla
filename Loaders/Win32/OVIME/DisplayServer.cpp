//#define OV_DEBUG 
#include "DisplayServer.h"
#include "OVIME.h"

DisplayServer* DisplayServer::m_self = NULL;
/*
AVDisplayServer *DisplayServer::lockIMC(HIMC h)
{
	hIMC = h;
	imm = ImmModel::open(hIMC);

	lpIMC = imm->getIMC();
	lpCompStr = imm->getCompStr();
	lpMyPrivate = imm->getMyPrivate();

	return this;
}

AVDisplayServer *DisplayServer::releaseIMC()
{
	ImmModel::close();

	lpMyPrivate = NULL;
	lpCompStr = NULL;
	lpIMC = NULL;
	hIMC = NULL;

	imm= NULL;

	return this;
}
*/

DisplayServer* DisplayServer::open()
{
	if(m_self == NULL)
		m_self = new DisplayServer();

	return m_self;
}

void DisplayServer::connectModel(HIMC hIMC)
{
	m_model = ImmModel::open(hIMC);
}

AVDisplayServer *DisplayServer::setBufString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(m_model->getMyPrivate()->PreEditStr, wstr);
	MakeCompStr(m_model->getMyPrivate(), m_model->getCompStr());

	murmur("\tAVDisplayServer *DisplayServer::setBufString(%s)",str);
	UISetCompStr(m_model->getMyPrivate()->PreEditStr);
	//要不要先檢查有沒有PreEditStr有沒有東西？

	return this;
}

AVDisplayServer *DisplayServer::setBufString(const char *str, int caretX)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(m_model->getMyPrivate()->PreEditStr, wstr);
	MakeCompStr(m_model->getMyPrivate(), m_model->getCompStr());

	murmur("\tAVDisplayServer *DisplayServer::setBufString(%s)",str);
	UISetCompStr(m_model->getMyPrivate()->PreEditStr);
	//要不要先檢查有沒有PreEditStr有沒有東西？

	m_model->getCompStr()->dwCursorPos = caretX;
	murmur("\tDisplayServer::setCursorPos-> %d", caretX);
	UISetCompCaretPosX(caretX);

	return this;
}

AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	
	m_model->getCompStr()->dwResultStrLen = (int)wcslen(wstr);

	wcscpy(GETLPRESULTSTR(m_model->getCompStr()), wstr);
	wcscpy(m_model->getMyPrivate()->PreEditStr, L"");
	
	MakeCompStr(m_model->getMyPrivate(), m_model->getCompStr());
	UIClearCompStr();//即時update C# comp string 同步資料
	
	//<comment author='b6s'>
	//Moves them to IME.cpp's ImeProcessKey,
	//since it casued a infinite-loop when "CTRL+\" occurred.
	//MyGenerateMessage(hIMC,
	//	WM_IME_COMPOSITION, 0, GCS_RESULTSTR);
	//MyGenerateMessage(hIMC,
	//	WM_IME_ENDCOMPOSITION, 0, 0);
	//</comment>

	return this;
}

AVDisplayServer *DisplayServer::setCandiString(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
	wcscpy(m_model->getMyPrivate()->CandStr, wstr);
	UpdateCandidate(m_model->getIMC(), wstr);
//	MyGenerateMessage(hIMC,
//			WM_IME_COMPOSITION, 0, GCS_COMPSTR);
	UISetCandStr(m_model->getMyPrivate()->CandStr);
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
		if(t &&
			m_model->getMyPrivate()->PreEditStr &&
			wcslen(m_model->getMyPrivate()->PreEditStr))
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
		if(t &&
			m_model->getMyPrivate()->CandStr &&
			wcslen(m_model->getMyPrivate()->CandStr))
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
