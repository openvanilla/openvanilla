#pragma once

#include <windows.h>

class IMEUI;
class IMEUILock
{
public:
	IMEUILock(HWND hUIWnd);
	~IMEUILock(void);
	IMEUI* getIMEUI();
private:
	HGLOBAL hIMEUI;
	IMEUI* ui;
	HWND hUIWnd;
public:
	IMEUI* createIMEUI(void);
};
