#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#ifdef WINCE
#include <imm.h>
#else
#include <immdev.h>
#endif

#include "ExtraStructs.h"
#include "AVDisplayServer.h"

#include "ImmModel.h"

extern bool isChinese;
extern bool isFullShape;

class DisplayServer : public AVDisplayServer
{
public:
	DisplayServer();
	~DisplayServer();

	void setHIMC(HIMC hIMC);
	virtual AVDisplayServer * getStatusPos(LPPOINT);
	bool getisChineseFlag() {return isChinese;}
	bool getisFullShapeFlag() {return isFullShape;}

	//virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *setBufString(const char *str,int caretX=0, int markFrom=-1, int markTo=-1); 
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *moveNotify(int x, int y);
	virtual AVDisplayServer *showCandi(bool t);
	//virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
	virtual AVDisplayServer *selectNextCandItem(int i);
	virtual int				getCandSelectedItemKey();
	virtual bool			getStatusModuleChecked(int index);
	virtual	AVDisplayServer * setStatusModuleChecked(int index, bool b);


	virtual AVDisplayServer *showStatus(bool t);
	virtual AVDisplayServer *moveBuf(
		int x, int y, int fontHeight, const wchar_t* fontName);
	virtual AVDisplayServer *moveCandi(int x, int y, int compHeight);

	virtual AVDisplayServer *SetStatusEnabled(bool t);
	virtual AVDisplayServer *SetCompEnabled(bool t);
	virtual AVDisplayServer *SetCandiEnabled(bool t);

	virtual AVDisplayServer *SetCompStarted(bool t);

	virtual bool			GetisStatusDocked();
	virtual void			SetisStatusDocked(bool t);

	virtual AVDisplayServer *setNotifyGlobalEnabled(bool b);
	virtual AVDisplayServer *setNotifyColor(int forecolor, int backcolor);
	virtual AVDisplayServer *setNotifyFont(const wchar_t* ff, int fontsize);
	virtual AVDisplayServer *setNotifyOpacity(int o);
	virtual AVDisplayServer *setCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor);
	virtual AVDisplayServer *setCandiFont(const wchar_t* ff, int fontsize);
	virtual AVDisplayServer *setCandiOpacity(int o);


public:	//James: 為了在Utils.cpp, IME.cpp 使用，暫改成public
	bool hasCompStarted; 
	bool isStatusEnabled; 
	bool isCompEnabled; 
	bool isCandiEnabled;
	

private:
	HIMC m_hIMC;
};
#endif //DISPLAYSERVER_H

