#include "AVDisplayServer.h"

OVDisplayServer *MyOVDisplayServer::setBufString(const string str) { bufstr = str; return this;}

const string MyOVDisplayServer::getBufString() { return bufstr; }

OVDisplayServer *MyOVDisplayServer::setCandiString(const string str) { candistr = str; return this; }

const string MyOVDisplayServer::getCandiString() { return candistr; }

OVDisplayServer *MyOVDisplayServer::regDisplayComponent(OVDisplayComponent* u) { UIObservers.push_back(u); return this;}

OVDisplayServer *MyOVDisplayServer::setBufPos(int x, int y) { bufX = x; bufY = y; return this; }
int MyOVDisplayServer::getBufX() { return bufX; }
int MyOVDisplayServer::getBufY() { return bufY; }
OVDisplayServer *MyOVDisplayServer::setCandiPos(int x, int y) { candiX = x; candiY = y; return this; }
int MyOVDisplayServer::getCandiX() { return candiX; }
int MyOVDisplayServer::getCandiY() { return candiY; }
OVDisplayServer *MyOVDisplayServer::setStatusPos(int x, int y) { statusX = x; statusY = y; return this; }
int MyOVDisplayServer::getStatusX() { return statusX; }
int MyOVDisplayServer::getStatusY() { return statusY; }
OVDisplayServer *MyOVDisplayServer::showBuf(bool t) { showbuf = t; return this; }
bool MyOVDisplayServer::isBufShow() { return showbuf; }
OVDisplayServer *MyOVDisplayServer::showCandi(bool t) { showcandi = t; return this; }
bool MyOVDisplayServer::isCandiShow() { return showcandi; }
OVDisplayServer *MyOVDisplayServer::showStatus(bool t) { showstatus = t; return this; }
bool MyOVDisplayServer::isStatusShow() { return showstatus; }
OVDisplayServer *MyOVDisplayServer::setCursorPos(int i) { cursorPos = i; return this; }
int MyOVDisplayServer::getCursorPos() { return cursorPos; }
OVDisplayServer *MyOVDisplayServer::setMarkFrom(int i) { markfrom = i; return this; }
int MyOVDisplayServer::getMarkFrom() { return markfrom; }
OVDisplayServer *MyOVDisplayServer::setMarkTo(int i) { markto = i; return this; }
int MyOVDisplayServer::getMarkTo() { return markto; }
OVDisplayServer *MyOVDisplayServer::setHInst(HINSTANCE h) { hInst = h; return this; }
HINSTANCE MyOVDisplayServer::getHInst() { return hInst; }
OVDisplayServer *MyOVDisplayServer::setHWnd(HWND h) { hWnd = h; return this; }
HWND MyOVDisplayServer::getHWnd() { return hWnd; }

OVDisplayServer *MyOVDisplayServer::pushInputMethod(const char *str) { IC.push_back(str); return this; }
OVDisplayServer *MyOVDisplayServer::setInputMethod(int i) { CurrentIC = i; return this; }
int MyOVDisplayServer::getInputMethod() { return CurrentIC; }
const string MyOVDisplayServer::getInputMethodName(int i) { return IC.at(i); }

template<class TYPE> struct update : public std::unary_function<TYPE, void> {
	void operator() (TYPE& x) { static_cast<OVDisplayComponent*>(x)->update(); }
};

OVDisplayServer *MyOVDisplayServer::removeDisplayComponent(OVDisplayComponent* u)
	{
		vector<OVDisplayComponent*>::iterator t;
		t = find(UIObservers.begin(), UIObservers.end(), u);
		if(*t) UIObservers.erase(t);
		return this;
	}
OVDisplayServer *MyOVDisplayServer::notify() { for_each(UIObservers.begin(), UIObservers.end(), update<OVDisplayComponent*>()); return this;}
