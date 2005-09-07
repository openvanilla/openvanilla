#ifndef DummyDISPLAYSERVER_H
#define DummyDISPLAYSERVER_H

#include "AVDisplayServer.h"

class DummyDisplayServer : public AVDisplayServer
{
public:
	virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
};
#endif //DummyDISPLAYSERVER_H
