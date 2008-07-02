//#define OV_DEBUG
// OVIME.cpp : 定義 DLL 應用程式的進入點。
//

 
#include "OVIME.h"
#include "OVUtility.h"

typedef void (CALLBACK *TypeImeGetUIClassname)(LPTSTR lpszImeUIClassName);
HINSTANCE hUIInst;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  dwReason, 
                       LPVOID lpReserved
					 )
{	
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
		hInst = (HINSTANCE)hModule;

		AVLoader* loader = AVLoader::open();
		loader->connectDisplayServer(dsvr);

		// load UI library

#ifdef WINCE
		hUIInst = LoadLibrary(_T("Msimeuic.dll"));
		
		TypeImeGetUIClassname ImeGetUIClassName;
		ImeGetUIClassName= (TypeImeGetUIClassname) GetProcAddress( hUIInst,_T("ImeGetUIClassName") );

		if(ImeGetUIClassName) {
			ImeGetUIClassName(UICLASSNAME);
		char buf[32];
		wcstombs(buf, UICLASSNAME,32);

		murmur("UIClassName:%s\n", buf);

		}
		else
			murmur("Bind UI dll failed\n");

#else
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

		//IMEUIRegisterClass( (HINSTANCE)hModule );


		if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
		  return FALSE;

#endif
		
		break;
		}
	case DLL_PROCESS_DETACH:
		// free UI library
		AVLoader::close();
#ifdef WINCE
		FreeLibrary(hUIInst);
#else
		UnregisterClass(UICLASSNAME, (HINSTANCE)hModule);
#endif
		//IMEUIUnRegisterClass( (HINSTANCE)hModule );
		//<comment author='b6s'>Test for a replacement of UI's DllMain()
		//UIDispose();
		//</comment>
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
    return TRUE;
}
