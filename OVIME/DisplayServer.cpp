//#define OV_DEBUG 
#include "DisplayServer.h"
#include "OVIME.h"
#include "ImmController.h"
#include <windows.h>

DisplayServer::DisplayServer()
{
	murmur("dsvr initialized\n");
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

AVDisplayServer *DisplayServer::getStatusPos(LPPOINT lp_pt)
{
	
	if(UIGetStatusPos(lp_pt)==-1) {
		lp_pt->x=-1;
		lp_pt->y=-1;
	}

	return this;
	 
}
/*
AVDisplayServer *DisplayServer::setBufString(const char *str)
{
	UIHideNotifyWindow();	
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	ImmModel* model = ImmModel::open(m_hIMC);
	wchar_t* word = GETLPCOMPSTR(model->getCompStr());
	//wcscpy(word, wstr);
	wcscpy(model->getMyPrivate()->PreEditStr, wstr);
	MakeCompStr(model->getMyPrivate(), model->getCompStr(), 0);
	UISetCompStr(model->getMyPrivate()->PreEditStr);
	//要不要先檢查有沒有PreEditStr有沒有東西？


	ImmModel::close(); 
	murmur("\tCOMPOSITION GCS_COMPSTR");
	//MyGenerateMessage(m_hIMC, WM_IME_COMPOSITION, 0, GCS_COMPSTR);
	MyGenerateMessage( m_hIMC, WM_IME_COMPOSITION, 
				*(WORD*)word,
				(GCS_COMPSTR|GCS_COMPATTR
	//			|GCS_COMPREADSTR|GCS_COMPREADATTR
				|GCS_COMPCLAUSE|GCS_COMPREADCLAUSE
				|GCS_CURSORPOS
	//			|GCS_DELTASTART				 
				));
	//
	

	return this;
}
*/
AVDisplayServer *DisplayServer::setBufString(const char *str, int caretX, int markFrom, int markTo)
{
	hideNotify();

	ImmController* controller = ImmController::open();
	
	if(!controller->getCompStartedFlag()) {
		//controller->setCompStartedFlag(true);   
		murmur("\tDSVR: STARTCOMPOSITION");		
		MyGenerateMessage( m_hIMC, WM_IME_STARTCOMPOSITION,0,0);  
	}

	
	ImmModel* model = ImmModel::open(m_hIMC);

	
	wchar_t wstr[1024];
	
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
 	wcscpy(model->getMyPrivate()->PreEditStr, wstr);
	


	MakeCompStr(model->getMyPrivate(), model->getCompStr(), caretX, markFrom, markTo);
	wchar_t* word = GETLPCOMPSTR(model->getCompStr());


	
	murmur("\tDISPLAYSERVER::setbufString str:%s, len,%i, caretX:%i",str, wcslen(wstr), caretX);

	
	
	//要不要先檢查有沒有PreEditStr有沒有東西？
	UISetCompStr(model->getMyPrivate()->PreEditStr);
	if(caretX > -1 ) UISetCompCaretPosX(caretX); 
	else UISetCompCaretPosX((int)wcslen(wstr));
	
	//model->getCompStr()->dwCursorPos = caretX;

	/*
	
	if(!controller->getCompStartedFlag()) {     
		controller->setCompStartedFlag(true); 
		murmur("\tDSVR: STARTCOMPOSITION");
		murmur("\tdwCompResultStrlen:%d, dwCompStrLen:%d", model->getCompStr()->dwResultStrLen, model->getCompStr()->dwCompStrLen);
		MyGenerateMessage( m_hIMC, WM_IME_STARTCOMPOSITION,0,0); 
		ImmModel::close();
		return this;
	}
	*/
	ImmModel::close();
	
	
	murmur("\tCOMPOSITION GCS_COMPSTR"); 
	MyGenerateMessage( m_hIMC, WM_IME_COMPOSITION, 
				*(WORD*)word,
				(GCS_COMPSTR|GCS_COMPATTR
	//			|GCS_COMPREADSTR|GCS_COMPREADATTR
				|GCS_COMPCLAUSE//|GCS_COMPREADCLAUSE
				|GCS_CURSORPOS
				|GCS_DELTASTART				 
				));
	

	
	return this;
}

AVDisplayServer *DisplayServer::sendBuf(const char *str)
{
	wchar_t wstr[1024];
	MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);

	murmur("\tDISPLAYSERVER::sendbuf str:%s, len,%i",str, wcslen(wstr));
	ImmModel* model = ImmModel::open(m_hIMC); 
	wcscpy(model->getMyPrivate()->PreEditStr, L"");	
	MakeCompStr(model->getMyPrivate(), model->getCompStr());

	model->getCompStr()->dwResultStrLen = (int)wcslen(wstr);
	wchar_t* word = GETLPRESULTSTR(model->getCompStr());
	wcscpy(word, wstr);
	
	ImmModel::close();

	//James:把下兩行提前，看能否解決閃爍問題  
	showBuf(false);
	//再hide後再清掉，避免殘像發生 
	UIClearCompStr();//即時update C# comp string 同步資料
	//showCandi(false);

	murmur("\tCOMPOSITION GCS_RESULTSTR");

	MyGenerateMessage(
		m_hIMC,
		WM_IME_COMPOSITION,
		*(WORD*)word,
		GCS_COMPSTR|GCS_COMPATTR|
	//	GCS_COMPREADSTR|GCS_COMPREADATTR|
	//	GCS_COMPCLAUSE|GCS_COMPREADCLAUSE|
	//	GCS_CURSORPOS|GCS_DELTASTART|
		GCS_RESULTSTR|GCS_RESULTCLAUSE
	//  GCS_RESULTREADSTR|GCS_RESULTREADCLAUSE		
	);

	ImmController* controller = ImmController::open();
	controller->setCompStartedFlag(false);
	//controller->close();
	//controller = NULL;

	murmur("\tDSRV:ENDCOMPOSITION");
	MyGenerateMessage(m_hIMC, WM_IME_ENDCOMPOSITION, 0, 0);

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
	murmur("Notifypage:%s",wstr);
	UISetNotifyStr(wstr); 
	RefreshUI(m_hIMC);
	UIShowNotifyWindow();
	

	return this;
}

AVDisplayServer *DisplayServer::hideNotify()
{

	UIHideNotifyWindow();	
	return this;
}

AVDisplayServer *DisplayServer::moveNotify(int x, int y)
{
	UIMoveNotifyWindow(x, y);
	return this;
}
AVDisplayServer *DisplayServer::moveBuf(
	int x, int y, int fontHeight, const wchar_t* fontName)
{
	murmur("\t move comp to (%d,%d)",x,y);
	UIMoveCompWindow(x, y, fontHeight, fontName);
	return this;
}

AVDisplayServer *DisplayServer::moveCandi(int x, int y, int compHeight)
{
	murmur("\t move candi to (%d,%d)",x,y);
	UIMoveCandWindow(x, y, compHeight);
	return this;
}

AVDisplayServer *DisplayServer::SetCompStarted(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompStarted=true");
	else
		murmur("\t*DisplayServer::SetCompStarted=false");
	
	hasCompStarted=t;
	return this;
}

AVDisplayServer *DisplayServer::SetStatusEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetStatusEnabled=true");
	else
		murmur("\t*DisplayServer::SetStatusEnabled=false");
	
	isStatusEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCompEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCompEnabled=true");
	else
		murmur("\t*DisplayServer::SetCompEnabled=false");
	
	isCompEnabled=t;
	return this;
}

AVDisplayServer *DisplayServer::SetCandiEnabled(bool t)
{
	if(t)
		murmur("\t*DisplayServer::SetCandiEnabled=true");
	else
		murmur("\t*DisplayServer::SetCandiEnabled=false");
	
	isCandiEnabled=t;
	return this; 
}

AVDisplayServer *DisplayServer::showStatus(bool t)
{	
	murmur("dsvr: show status window: %d", t);
	if(dsvr->isStatusEnabled && !t) 
	{
		UIHideStatusWindow();
		dsvr->SetStatusEnabled(false);
	}
	if(!dsvr->isStatusEnabled && t)
	{
		UIShowStatusWindow();
		dsvr->SetStatusEnabled(true);
	}
	
	return this;
}

bool DisplayServer::GetisStatusDocked()
{
	return UIGetIsStatusDocked();
}
void DisplayServer::SetisStatusDocked(bool t)
{
	UISetIsStatusDocked(t);
	
}


AVDisplayServer *DisplayServer::showBuf(bool t)
{
	if(dsvr->isCompEnabled)
	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->PreEditStr &&
			wcslen(model->getMyPrivate()->PreEditStr))
		{
			murmur("\t show comp");
			UIShowCompWindow();
		}
		else
		{
			murmur("\t hide comp");
			UIHideCompWindow();
		}

		ImmModel::close();
	}
	return this;
}

AVDisplayServer *DisplayServer::showCandi(bool t)
{
	if(!dsvr->isCandiEnabled) 
		dsvr->SetCandiEnabled(true);
		
//	{
		ImmModel* model = ImmModel::open(m_hIMC);

		if(t &&
			model->getMyPrivate()->CandStr &&
			wcslen(model->getMyPrivate()->CandStr))
		{
			murmur("\t show candi"); 
			RefreshUI(m_hIMC);
			UIShowCandWindow();
		}
		else
		{
			murmur("\t hide candi");
			UIHideCandWindow();
		}

		ImmModel::close();
//	}
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

AVDisplayServer *DisplayServer::selectNextCandItem(int i)
{
	UISelectNextCandItem(i);
	return this;
}


int DisplayServer::getCandSelectedItemKey()
{
	return UIGetCandSelectedItemKey();
	
}

bool DisplayServer::getStatusModuleChecked(int index)
{
	return UIGetModuleChecked(index);
}

AVDisplayServer *DisplayServer::setStatusModuleChecked(int index, bool b)
{
	UISetModuleChecked(index, b);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyGlobalEnabled(bool b)
{
	UISetNotifyEnabled(b);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyColor(int forecolor, int backcolor)
{
	murmur("dsvr::setNotifyColor, %d, %d", forecolor, backcolor);
	//UISetNotifyColor(forecolor, backcolor);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyFont(const wchar_t* ff, int fontsize)
{
	UISetNotifyFont(ff, fontsize);
	return this;
}

AVDisplayServer *DisplayServer::setCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor)
{
	UISetCandiColor(forecolor, backcolor, titlecolor, titlebackcolor);
	return this;
}

AVDisplayServer *DisplayServer::setCandiFont(const wchar_t* ff, int fontsize)
{
	UISetCandiFont(ff, fontsize);
	return this;
}


AVDisplayServer *DisplayServer::setCandiOpacity(int o)
{
	UISetCandiOpacity(o);
	return this;
}

AVDisplayServer *DisplayServer::setNotifyOpacity(int o)
{
	UISetNotifyOpacity(o);
	return this;
}
