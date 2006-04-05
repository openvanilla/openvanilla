#pragma once

#include "IMEChildWnd.h"
#include "IMCLock.h"

#include <string>

using namespace std;

class CompWnd : public IMEChildWnd
{
public:
	CompWnd(void);
	~CompWnd(void);
	static BOOL registerClass(void);
protected:
	static LRESULT wndProc( HWND hwnd , UINT msg, WPARAM wp , LPARAM lp);
	int indexToXPos( wstring compStr, int idx);
	void onPaint(IMCLock& imc, PAINTSTRUCT& ps);
	HFONT font;
	bool coloredCompCursor;
public:
	void setFont(LOGFONT* lf);
	HFONT getFont(){	return font;	}
	void getSize(int* w, int* h);
	wstring getDisplayedCompStr(IMCLock& imc);
	int getDisplayedCursorPos(IMCLock& imc);
	wstring getDisplayedCompStr(){	return getDisplayedCompStr(IMCLock(getIMC()));	}
	int getDisplayedCursorPos(){	return getDisplayedCursorPos(IMCLock(getIMC()));	}
public:
	bool create(HWND imeUIWnd);
	void getRelativeCandPos(IMCLock& imc, POINT* pt);
	void getCandPos(IMCLock& imc, POINT* pt);
	short* getIntervalAry( IMCLock& imc );
};
