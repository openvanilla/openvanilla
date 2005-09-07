#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include "AVDisplayServer.h"
#include <windows.h>

class DisplayServer : public AVDisplayServer
{
public:
	AVDisplayServer *setHIMC(HIMC h);
	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
private:
	HIMC hIMC;
};
#endif //DISPLAYSERVER_H
