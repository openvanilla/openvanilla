#ifndef AVDISPLAYSERVER_H
#define AVDISPLAYSERVER_H

#include "OpenVanilla.h"
#include <windows.h>

class AVDisplayServer : public OVBase
{
public:
	virtual bool getisChineseFlag()=0;
	virtual bool getisFullShapeFlag()=0;
	virtual AVDisplayServer *getStatusPos(LPPOINT) {return this;}
	virtual AVDisplayServer *setStatusPos(int, int) {return this;}
	//virtual AVDisplayServer *setBufString(const char *str) { return this; }
	virtual AVDisplayServer *setBufString(const char *str,int caretX=-1, int markFrom=-1, int markTo=-1) { return this; }
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
	virtual AVDisplayServer *selectNextCandItem(int i) { return this; }
	virtual int getCandSelectedItemKey()=0;
	virtual bool getStatusModuleChecked(int index)=0;
	virtual	AVDisplayServer *setStatusModuleChecked(int index, bool b){ return this; }
	virtual AVDisplayServer *setNotifyGlobalEnabled(bool b){ return this; }
	virtual AVDisplayServer *setNotifyColor(int forecolor, int backcolor){ return this; }
	virtual AVDisplayServer *setNotifyFont(const wchar_t* ff, int fontsize){ return this; }
	virtual AVDisplayServer *setNotifyOpacity(int o){ return this; }
	virtual AVDisplayServer *setCandiColor(int forecolor, int backcolor, int titlecolor, int titlebackcolor){ return this; }
	virtual AVDisplayServer *setCandiFont(const wchar_t* ff, int fontsize){ return this; }
	virtual AVDisplayServer *setCandiOpacity(int o){ return this; }


	
};
#endif //AVDISPLAYSERVER_H

