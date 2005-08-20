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
		// load UI library
		char str[1024];
		LPTSTR decoded;
		InitLoader();
		for(int i = 0;; ++i) {
			if(ModuleName(i, str)) {
				dsvr.pushInputMethod(str);
				decoded = UTF16toWCHAR(str);
				UIPushInputMethod(decoded);
				free(decoded);
			} else {
				break;
			}
		}

		
		WNDCLASSEX wc;
		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_OVIME | CS_IME;
		wc.lpfnWndProc		= UIWndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 2 * sizeof(LONG);
		wc.hInstance		= (HINSTANCE)hModule;
		wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wc.hIcon			= NULL;
		wc.lpszMenuName		= (LPTSTR)NULL;
		wc.lpszClassName	= UICLASSNAME;
		wc.hbrBackground	= NULL;
		wc.hIconSm			= NULL;
		IMEUIRegisterClass( (HINSTANCE)hInst );
		dsvr.setHInst( (HINSTANCE)hInst );
		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

		break;
	case DLL_PROCESS_DETACH:
		// free UI library
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
		IMEUIUnRegisterClass( (HINSTANCE)hModule );
		ShutdownLoader();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
