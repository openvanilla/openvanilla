#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#include <immdev.h>
#include "ExtraStructs.h"
#include "AVDisplayServer.h"

#include "ImmModel.h"

class DisplayServer : public AVDisplayServer
{
public:
	DisplayServer();
	~DisplayServer();

	void setHIMC(HIMC hIMC);

	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *setBufString(const char *str,int caretX);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *showCandi(bool t);
	//virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);

	virtual DisplayServer *showStatus(bool t);
	virtual DisplayServer *moveBuf(int x,int y);
	virtual DisplayServer *moveCandi(int x, int y, int compHeight);

	virtual DisplayServer *SetStatusEnabled(bool t);
	virtual DisplayServer *SetCompEnabled(bool t);
	virtual DisplayServer *SetCandiEnabled(bool t);

	virtual DisplayServer *SetCompStarted(bool t);

public:	//James: 為了在Utils.cpp, IME.cpp 使用，暫改成public
	bool hasCompStarted; 
	bool isStatusEnabled; 
	bool isCompEnabled; 
	bool isCandiEnabled;

private:
	HIMC m_hIMC;
};
#endif //DISPLAYSERVER_H

