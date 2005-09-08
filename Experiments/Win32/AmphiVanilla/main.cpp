#include <stdio.h>

#include "AVLoader.h"
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
		loader->keyEvent(0, c);
	}
	loader->reloadConfig();
	AVLoader::shutdown();
	murmur("shutdown");
	delete dsvr;
	return 0;
}
