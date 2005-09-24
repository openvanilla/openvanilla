// OVIME.cpp : 定義 DLL 應用程式的進入點。
//
#include "OVIME.h"
#include "NetIO.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{
	UDPSocket sock;
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		// load UI library
		/*
		char str[1024];
		wchar_t wstr[1024];
		loader = AVLoader::getLoader();
		loader->connectDisplayServer(dsvr);
		for(int i = 0;; ++i) {
			if(loader->moduleName(i, str)) {
				MultiByteToWideChar(CP_UTF8, 0, str, strlen(str)+1, wstr, 1024);
				UIPushInputMethod(wstr);
			} else {
				break;
			}
		}
		*/
		char pos[2];
		wchar_t wstr[1024];
		cmd_in rtn;
		for(int i = 0;; ++i) {
			pos[0] = i;
			sock.send("127.0.0.1", 5100, GETNAME, pos);
			sock.recv(&rtn);
			if(rtn.buf[0]) {
				MultiByteToWideChar(CP_UTF8, 0, rtn.buf, strlen(rtn.buf)+1, wstr, 1024);
				UIPushInputMethod(wstr);
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
		IMEUIRegisterClass( (HINSTANCE)hModule );
		hInst = (HINSTANCE)hModule;
		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

		break;
	case DLL_PROCESS_DETACH:
		// free UI library
		//AVLoader::shutdown();
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
		IMEUIUnRegisterClass( (HINSTANCE)hModule );
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
