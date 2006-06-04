#pragma once
#include "window.h"

class Tooltip :
	public Window
{
public:
	Tooltip(void);
	virtual ~Tooltip(void);
	static BOOL registerClass(void);
	static void unregisterClass();
protected:
	LRESULT wndProc(UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
public:
	void onPaint(PAINTSTRUCT& ps);
	void showTip(int x, int y, LPCWSTR tip_text, DWORD duration = 0);
protected:
	UINT timerID;
	LPWSTR text;
public:
	BOOL create(void);
	void hideTip(void);
};
