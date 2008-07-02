#include "OVIME.h"

BOOL CALLBACK ReloadIME(HWND hwnd, LPARAM lp)
{
	TCHAR tmp[9];
	GetClassName( hwnd, tmp, 8 );

	
	if( ! _tcscmp( tmp, UICLASSNAME ) ) {
		SendMessage( hwnd, WM_IME_RELOADCONFIG, 0, 0 );
	}
	return TRUE;
}