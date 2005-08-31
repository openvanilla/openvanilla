#ifndef OVDISPLAYSERVER_H
#define OVDISPLAYSERVER_H

#include <windows.h>

#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVDisplayComponent.h"

#include <string>
#include <algorithm>
#include <functional>
#include <vector>

using std::string;
using std::vector;

#ifndef OV_DEBUG
#define OV_DEBUG
#endif

class OVDisplayServer : public OVBase
{
public:
	virtual OVDisplayServer *setBufString(const char *str) { return this; }
	virtual const string getBufString() { return ""; }
	virtual OVDisplayServer *setCandiString(const char *str) { return this; }
	virtual const string getCandiString() { return ""; }
	virtual OVDisplayServer *setBufPos(int x, int y) { return this; }
	virtual int getBufX() { return 0; }
	virtual int getBufY() { return 0; }
	virtual OVDisplayServer *setCandiPos(int x, int y) { return this; }
	virtual int getCandiX() { return 0; }
	virtual int getCandiY() { return 0; }
	virtual OVDisplayServer *setStatusPos(int x, int y) { return this; }
	virtual int getStatusX() { return 0; }
	virtual int getStatusY() { return 0; }
	virtual OVDisplayServer *showBuf(bool t) { return this; }
	virtual bool isBufShow() { return true; }
	virtual OVDisplayServer *showCandi(bool t) { return this; }
	virtual bool isCandiShow() { return true; }
	virtual OVDisplayServer *showStatus(bool t) { return this; }
	virtual bool isStatusShow() { return true; }
	virtual OVDisplayServer *setCursorPos(int i) { return this; }
	virtual int getCursorPos() { return 0; }
	virtual OVDisplayServer *setMarkFrom(int i) { return this; }
	virtual int getMarkFrom() { return 0; }
	virtual OVDisplayServer *setMarkTo(int i) { return this; }
	virtual int getMarkTo() { return 0; }
	virtual OVDisplayServer *pushInputMethod(const char *str) { return this; }
	virtual OVDisplayServer *setInputMethod(int i) { return this; }
	virtual int getInputMethod() { return 0; }
	virtual const string getInputMethodName(int i) { return ""; }
	virtual OVDisplayServer *setHInst(HINSTANCE h) { return this; }
	virtual HINSTANCE getHInst() { return 0; }
	virtual OVDisplayServer *setHWnd(HWND h) { return this; }
	virtual HWND getHWnd() { return 0; }

	virtual OVDisplayServer *regDisplayComponent(OVDisplayComponent* u) { return this; }
	virtual OVDisplayServer *removeDisplayComponent(OVDisplayComponent* u) { return this; }
	virtual OVDisplayServer *notify() { return this; }

};

class MyOVDisplayServer : public OVDisplayServer
{
public:
	virtual OVDisplayServer *setBufString(const string str);
	virtual const string getBufString();
	virtual OVDisplayServer *setCandiString(const string str);
	virtual const string getCandiString();
	virtual OVDisplayServer *setBufPos(int x, int y);
	virtual int getBufX();
	virtual int getBufY();
	virtual OVDisplayServer *setCandiPos(int x, int y);
	virtual int getCandiX();
	virtual int getCandiY();
	virtual OVDisplayServer *setStatusPos(int x, int y);
	virtual int getStatusX();
	virtual int getStatusY();
	virtual OVDisplayServer *showBuf(bool t);
	virtual bool isBufShow();
	virtual OVDisplayServer *showCandi(bool t);
	virtual bool isCandiShow();
	virtual OVDisplayServer *showStatus(bool t);
	virtual bool isStatusShow();
	virtual OVDisplayServer *setCursorPos(int i);
	virtual int getCursorPos();
	virtual OVDisplayServer *setMarkFrom(int i);
	virtual int getMarkFrom();
	virtual OVDisplayServer *setMarkTo(int i);
	virtual int getMarkTo();
	virtual OVDisplayServer *pushInputMethod(const char *str);
	virtual OVDisplayServer *setInputMethod(int i);
	virtual int getInputMethod();
	virtual const string getInputMethodName(int i);
	virtual OVDisplayServer *setHInst(HINSTANCE h);
	virtual HINSTANCE getHInst();
	virtual OVDisplayServer *setHWnd(HWND h);
	virtual HWND getHWnd();

	virtual OVDisplayServer *regDisplayComponent(OVDisplayComponent* u);
	virtual OVDisplayServer *removeDisplayComponent(OVDisplayComponent* u);
	virtual OVDisplayServer *notify();
protected:
	string bufstr;
	string candistr;
	bool showbuf;
	bool showcandi;
	bool showstatus;
	int bufX;
	int bufY;
	int candiX;
	int candiY;
	int statusX;
	int statusY;
	int cursorPos;
	int markfrom;
	int markto;
	// InputContext
	vector<string> IC;
	int CurrentIC;
	
	HINSTANCE hInst;
	HWND hWnd;

	vector<OVDisplayComponent*> UIObservers;
};

#endif //OVDISPLAYSERVER_H