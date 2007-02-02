#ifndef AVDISPLAYSERVER_H
#define AVDISPLAYSERVER_H

#include "OpenVanilla.h"

class AVDisplayServer : public OVBase
{
public:
	virtual AVDisplayServer *setBufString(const char *str) { return this; }
	virtual AVDisplayServer *setBufString(const char *str,int caretX) { return this; }
	virtual AVDisplayServer *sendBuf(const char *str) { return this; }
	virtual AVDisplayServer *setCandiString(const char *str) { return this; }
	virtual AVDisplayServer *updateBuf() { return this; }
	virtual AVDisplayServer *showBuf(bool t) { return this; }	
	virtual AVDisplayServer *showCandi(bool t) { return this; }
	virtual AVDisplayServer *showNotify(const char *str) { return this; }
	virtual AVDisplayServer *hideNotify() { return this; }
	virtual AVDisplayServer *setCursorPos(int i) { return this; }
	virtual AVDisplayServer *setMarkFrom(int i) { return this; }
	virtual AVDisplayServer *setMarkTo(int i) { return this; }
};
#endif //AVDISPLAYSERVER_H

