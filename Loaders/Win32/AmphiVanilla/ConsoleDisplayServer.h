#ifndef ConsoleDISPLAYSERVER_H
#define ConsoleDISPLAYSERVER_H

#include "AVDisplayServer.h"

class ConsoleDisplayServer : public AVDisplayServer
{
public:
	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
};
#endif //ConsoleDISPLAYSERVER_H
