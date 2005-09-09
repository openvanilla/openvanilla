#include <stdio.h>

#include "AVLoader.h"
#include "AVKeyCode.h"
#include "AVDisplayServer.h"
#include "DummyDisplayServer.h"

int main(int argc, char *argv[])
{
	char str[1024];
	char c;
	AVLoader *loader = AVLoader::getLoader();
	AVLoader::shutdown();
	loader = AVLoader::getLoader();
	AVDisplayServer *dsvr = new DummyDisplayServer();
	loader->connectDisplayServer(dsvr);
	murmur("All inited");
	while( c = getchar())
	{
		if(c == EOF)
			break;
		if(c == '\n')
			continue;
		AVKeyCode kc(c);
		kc.setCapslock(1);
		loader->keyEvent(0, kc);
	}
	loader->reloadConfig();
	AVLoader::shutdown();
	murmur("shutdown");
	delete dsvr;
	return 0;
}
