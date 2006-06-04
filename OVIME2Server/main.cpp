#include <windows.h>
#include <tchar.h>
#include "OVIME2Server.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	if( FindWindow( OVIME2ServerClassName, NULL) )
		return -1;
	OVIME2Server server;
	return ! server.run();
}



