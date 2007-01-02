#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#include <immdev.h>
#include "ExtraStructs.h"
#include "AVDisplayServer.h"

class DisplayServer : public AVDisplayServer
{
public:
	AVDisplayServer *lockIMC(HIMC h);
	AVDisplayServer *releaseIMC();
	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);	
	virtual DisplayServer *showStatus(bool t); //Jamed add
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);	
	virtual DisplayServer *moveBuf(int x,int y);
	virtual DisplayServer *moveCandi(int x,int y);
	virtual DisplayServer *SetCompStarted(bool t);
private:
	HIMC hIMC;
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	LPMYPRIVATE lpMyPrivate;
public:
	bool compStarted; //為了在Utils.cpp計算有無comp window的座標 暫改成public
};
#endif //DISPLAYSERVER_H

