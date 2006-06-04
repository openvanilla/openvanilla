#include "AVLoader.h"
#include "AVKeyCode.h"
#include "AVDisplayServer.h"
#include "ConsoleDisplayServer.h"

int main(int argc, char *argv[])
{
	AVLoader *loader = AVLoader::getLoader();
	AVDisplayServer *dsvr = new ConsoleDisplayServer();
	loader->connectDisplayServer(dsvr);
	murmur("All inited");
	char c;
	while(c = getchar())
	{
		AVKeyCode kc(c);
		//kc.setCapslock(cmd.buf[2] == 1? 1:0);
		//kc.setShift(cmd.buf[3] == 1? 1:0);
		int rt = loader->keyEvent(0, kc);
	}
	AVLoader::shutdown();
	murmur("shutdown");
	delete dsvr;
	return 0;
}