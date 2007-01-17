#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#include <immdev.h>
#include "ExtraStructs.h"
#include "AVDisplayServer.h"

class DisplayServer : public AVDisplayServer
{
public:
	DisplayServer()
	{
		hIMC=NULL;
		lpIMC=NULL;
		lpCompStr=NULL;
		lpMyPrivate=NULL;		
		hasCompStarted=false;
		isStatusEnabled=false;
		isCompEnabled=false;
		isCandiEnabled=false;
	}
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

	virtual DisplayServer *SetStatusEnabled(bool t);	
	virtual DisplayServer *SetCompEnabled(bool t);	
	virtual DisplayServer *SetCandiEnabled(bool t);	

public:	//James: 為了在Utils.cpp, IME.cpp 使用，暫改成public
	HIMC hIMC;	
	LPCOMPOSITIONSTRING lpCompStr;
	LPINPUTCONTEXT lpIMC;
	LPMYPRIVATE lpMyPrivate;	
	bool hasCompStarted; 
	bool isStatusEnabled; 
	bool isCompEnabled; 
	bool isCandiEnabled; 
};
#endif //DISPLAYSERVER_H

