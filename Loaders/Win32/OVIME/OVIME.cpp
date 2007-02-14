// OVIME.cpp : 定義 DLL 應用程式的進入點。
//
//#define OV_DEBUG

#include "OVIME.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{	
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
		// load UI library
		AVLoader* loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		//<comment author='b6s'>Test for a replacement of UI's DllMain()
		UIConstruct();
		//</comment>

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
		IMEUIRegisterClass( (HINSTANCE)hModule );
		hInst = (HINSTANCE)hModule;

		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

		break;
		}
	case DLL_PROCESS_DETACH:
		// free UI library
		AVLoader::close();
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
		IMEUIUnRegisterClass( (HINSTANCE)hModule );
		//<comment author='b6s'>Test for a replacement of UI's DllMain()
		UIDispose();
		//</comment>
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
