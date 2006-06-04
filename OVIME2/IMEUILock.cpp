#include ".\imeuilock.h"
#include "IMEUI.h"

IMEUILock::IMEUILock(HWND hIMEUIWnd) : ui(NULL), hUIWnd(hIMEUIWnd)
{
	hIMEUI = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
}

IMEUILock::~IMEUILock(void)
{
	if( ui )
		GlobalUnlock(hIMEUI);
}

IMEUI* IMEUILock::getIMEUI()
{
	if( ui )
		return ui;
	ui = (IMEUI*)GlobalLock(hIMEUI);
	return ui;
}



IMEUI* IMEUILock::createIMEUI(void)
{
	hIMEUI = GlobalAlloc(GHND, sizeof(IMEUI));
	SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)hIMEUI );
	ui = (IMEUI*)GlobalLock(hIMEUI);
	if( ui )
		return new(ui) IMEUI(hUIWnd);	// placement new
	return NULL;
}
