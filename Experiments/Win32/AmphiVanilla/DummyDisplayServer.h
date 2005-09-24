#ifndef DummyDISPLAYSERVER_H
#define DummyDISPLAYSERVER_H

#include "AVDisplayServer.h"
#include "NetIO.h"

class DummyDisplayServer : public AVDisplayServer
{
public:
	DummyDisplayServer(UDPSocket *sock);
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
private:
	UDPSocket *sock;
};
#endif //DummyDISPLAYSERVER_H
