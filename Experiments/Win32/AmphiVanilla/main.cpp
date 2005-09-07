#include <iostream>
#include <cstring>
#include <windows.h>
#include <stdio.h>
#include <string>

#include "AVDisplayServer.h"
#include "DummyDisplayServer.h"

extern "C" {
extern void InitLoader();
extern void ShutdownLoader();
extern void ReloadConfig();
extern void SetDisplayServer(AVDisplayServer *);
extern int KeyEvent(int n, int c, char *s);
extern int ModuleName(int, char *);
}

int main(int argc, char *argv[])
{
	char str[1024];
	char c;
	AVDisplayServer *dsvr = new DummyDisplayServer();
	SetDisplayServer(dsvr);
	InitLoader();
	while( c = getchar())
	{
		if(c == EOF)
			break;
		if(c == '\n')
			continue;
		KeyEvent(0, c, str);
		printf("%c: %s\n", c, str);
	}
	ReloadConfig();
	getchar();
	ShutdownLoader();
	return 0;
}
