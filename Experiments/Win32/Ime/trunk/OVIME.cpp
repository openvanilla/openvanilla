// OVIME.cpp : 定義 DLL 應用程式的進入點。
//
#include "OVIME.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		hInst = (HINSTANCE)hModule;
		hUIFont = CreateFont(16, 0, 0, 0, FW_NORMAL,
					FALSE, FALSE, FALSE, ANSI_CHARSET,
					OUT_TT_PRECIS, CLIP_TT_ALWAYS, ANTIALIASED_QUALITY,
					DEFAULT_PITCH, _T("新細明體"));
		IMERegisterClass( hInst );
		break;
	case DLL_PROCESS_DETACH:
		IMEUnRegisterClass( hInst );
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}

BOOL IMERegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wc;
	//
	// register class of UI window.
    //
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_OVIME | CS_IME;
    wc.lpfnWndProc		= UIWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 2 * sizeof(LONG);
	wc.hInstance		= hInstance;
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hIcon			= NULL;
	wc.lpszMenuName		= (LPTSTR)NULL;
	wc.lpszClassName	= UICLASSNAME;
	wc.hbrBackground	= NULL;
	wc.hIconSm			= NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;

	//
    // register class of composition window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_OVIME | CS_IME;
    wc.lpfnWndProc    = CompWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = UICHILDEXTRASIZE;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = UICOMPCLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;

	if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		return FALSE;

	//
    // register class of candadate window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_OVIME | CS_IME;
    wc.lpfnWndProc    = CandWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = UICHILDEXTRASIZE;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = UICANDCLASSNAME;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;

	return TRUE;
}

BOOL IMEUnRegisterClass( HINSTANCE hInstance )
{
	UnregisterClass(UICLASSNAME, hInstance);
	return TRUE;
}