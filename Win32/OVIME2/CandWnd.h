#pragma once

#include "IMEChildWnd.h"

class IMCLock;
class CandWnd : public IMEChildWnd
{
public:
	CandWnd(void);
	~CandWnd(void);

	static BOOL registerClass(void);
	void setFont( HFONT f){	font = f;	}
protected:
	static LRESULT wndProc( HWND hwnd , UINT msg, WPARAM wp , LPARAM lp);
	void onPaint(HIMC hIMC, PAINTSTRUCT& ps);
public:
	void getSize(int* w, int* h);
	void updateSize(void);
	bool create(HWND imeUIWnd);
    void UpdateFont();
	void setCandPerRow(int n){	candPerRow = n;	}

    HFONT font;
    int   font_size;
	int   candPerRow;
};
