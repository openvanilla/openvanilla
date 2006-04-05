#pragma once

#include <windows.h>

#include "CompWnd.h"
#include "CandWnd.h"
#include "StatusWnd.h"
#include "Tooltip.h"

class IMEUI
{
public:
	IMEUI(HWND hUIWnd);
	~IMEUI(void);

	CompWnd compWnd;
	CandWnd candWnd;
	StatusWnd statusWnd;
	Tooltip tooltip;
	HWND hwnd;
	POINT fixedCompWndPos;

	static BOOL IMEUI::registerUIClasses();
protected:
	static LRESULT CALLBACK wndProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );
	LRESULT onIMENotify( HIMC hIMC, WPARAM wp , LPARAM lp );
	LRESULT wndProc( UINT msg, WPARAM wp, LPARAM lp);

	void closeStatusWnd(void);
	void openStatusWnd(HIMC hIMC);
	void openCandWnd(void);
	void closeCandWnd(void);

public:
	static void unregisterUIClasses();
	static bool getWorkingArea(RECT* rc, HWND app_wnd);
	LRESULT onComposition(HIMC hIMC, WPARAM wp , LPARAM lp);
	POINT getCompWndPos(IMCLock& imc);
//	void setCompWndPos(IMCLock& imc);
	POINT getCandWndPos(IMCLock& imc);
	HIMC getIMC(){	return (HIMC)GetWindowLong( hwnd, IMMGWL_IMC );	}
protected:
	void showCandWnd(void);
public:
	void showUserNotify(IMCLock& imc);
};

