#ifndef DummyDISPLAYSERVER_H
#define DummyDISPLAYSERVER_H


#include "AVDisplayServer.h"
#include "OVUtility.h"

class DummyDisplayServer : public AVDisplayServer
{
public:
	virtual bool getisChineseFlag();
	virtual bool getisFullShapeFlag();
	virtual AVDisplayServer *setBufString(const char *str,int caretX=-1, int markFrom=-1, int markTo=-1);
	//virtual AVDisplayServer *setBufString(const char *str);
	virtual AVDisplayServer *sendBuf(const char *str);
	virtual AVDisplayServer *setCandiString(const char *str);
	virtual AVDisplayServer *showBuf(bool t);
	virtual AVDisplayServer *showCandi(bool t);
	virtual AVDisplayServer *showNotify(const char *str);
	virtual AVDisplayServer *hideNotify();
	virtual AVDisplayServer *setCursorPos(int i);
	virtual AVDisplayServer *setMarkFrom(int i);
	virtual AVDisplayServer *setMarkTo(int i);
	virtual int getCandSelectedItemKey();
	virtual bool getStatusModuleChecked(int index);
	virtual	AVDisplayServer * setStatusModuleChecked(int index, bool b);
};
#endif //DummyDISPLAYSERVER_H
