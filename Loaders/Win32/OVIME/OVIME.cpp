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
		// load UI library
		char str[1024];
		wchar_t wstr[1024];
		Watch watch;
		watch.start();
		loader = AVLoader::getLoader();
		watch.stop();
		murmur("%1.3f sec:\tAVLoader::getLoader()", watch.getSec());

		watch.start();
		loader->connectDisplayServer(dsvr);
		watch.stop();
		murmur("%1.3f sec:\tloader->connectDisplayServer(dsvr)", watch.getSec());

		watch.start();
		for(int i = 0;; ++i) {
			if(loader->moduleName(i, str)) {
				MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str)+1, wstr, 1024);
				UIPushInputMethod(wstr);
			} else {
				break;
			}
		}
		watch.stop();
		murmur("%1.3f sec:\tfor() { loader->moduleName(); UIPushInputMethod(); }",
			watch.getSec());
		
		watch.start();
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

		watch.stop();
		murmur("%1.3f sec:\t IMEUIRegisterClass", watch.getSec());

		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

		break;
	case DLL_PROCESS_DETACH:
		// free UI library
		AVLoader::shutdown();
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
		IMEUIUnRegisterClass( (HINSTANCE)hModule );
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
