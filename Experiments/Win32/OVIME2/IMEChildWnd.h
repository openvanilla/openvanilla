#pragma once

#include <windows.h>
#include "imm.h"
#include "window.h"

class IMEChildWnd : virtual public Window
{
public:
	IMEChildWnd();
	virtual ~IMEChildWnd(void);

	virtual void getSize(int* w, int* h){	*w=0; *h=0;	}
	HIMC getIMC(){	return (HIMC)GetWindowLong( GetParent(hwnd), IMMGWL_IMC );	}
	static HIMC getIMC(HWND hwnd){	return (HIMC)GetWindowLong( GetParent(hwnd), IMMGWL_IMC );	}
	void move(int x, int y);
//	static IMEChildWnd* getAssocWndObj(HWND hwnd);
protected:
	POINTS oldPos;
//	void assocWndObj(void);
protected:
	void onLButtonDown(WPARAM wp, LPARAM lp);
	void onLButtonUp(WPARAM wp, LPARAM lp);
	void onMouseMove(WPARAM wp, LPARAM lp);
public:
	virtual bool create(HWND imeUIWnd) = 0;
};
