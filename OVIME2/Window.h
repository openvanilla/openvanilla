// Window.h: interface for the Window class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDOW_H__86D89A4E_5040_4FF8_B991_0C7D6502119D__INCLUDED_)
#define AFX_WINDOW_H__86D89A4E_5040_4FF8_B991_0C7D6502119D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class Window  
{
public:
	Window();
	virtual ~Window();

	HWND getHwnd(){	return hwnd;	}
	void destroy(void);
	BOOL isVisible(){ return IsWindowVisible(hwnd); }
	BOOL isWindow(){ return IsWindow(hwnd); }
	void show()
	{
		if( hwnd )
			ShowWindow(hwnd, SW_SHOWNA);
	}
	void hide(){ ShowWindow(hwnd, SW_HIDE); }
	void refresh()	{	InvalidateRect( hwnd, NULL, FALSE );	}

protected:
	HWND hwnd;

};

#endif // !defined(AFX_WINDOW_H__86D89A4E_5040_4FF8_B991_0C7D6502119D__INCLUDED_)
