#ifndef DISPLAYSERVER_H
#define DISPLAYSERVER_H

#include <windows.h>
#include "imm.h"
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
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
private:
	HIMC hIMC;
	LPINPUTCONTEXT lpIMC;
	LPCOMPOSITIONSTRING lpCompStr;
	LPMYPRIVATE lpMyPrivate;
};
#endif //DISPLAYSERVER_H
