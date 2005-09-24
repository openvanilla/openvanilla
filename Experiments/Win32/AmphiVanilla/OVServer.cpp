#include "AVLoader.h"
#include "AVKeyCode.h"
#include "AVDisplayServer.h"
#include "DummyDisplayServer.h"
#include "NetIO.h"

int main(int argc, char *argv[])
{
	cmd_in cmd;
	char c;
	UDPSocket sock;
	sock.listen("127.0.0.1", 5100);

	AVLoader *loader = AVLoader::getLoader();
	AVDisplayServer *dsvr = new DummyDisplayServer(&sock);
	loader->connectDisplayServer(dsvr);
	murmur("All inited");
	while(sock.recv(&cmd))
	{
		switch(cmd.cmd) {
			case KEYCODE:
			{
				AVKeyCode kc(cmd.buf[1]);
				kc.setCapslock(cmd.buf[2] == 1? 1:0);
				kc.setShift(cmd.buf[3] == 1? 1:0);
				int rt = loader->keyEvent(cmd.buf[0]-1, kc);
				rt ? sock.send(PROCESS, "") : sock.send(PASS, "");
				sock.send(END, "");
			}
				break;
			case GETNAME:
			{
				loader->moduleName(cmd.buf[0], cmd.buf);
				sock.send(RETURN, cmd.buf);
			}
			break;
			case RELOADCONFIG:
				AVLoader::shutdown();
				loader = AVLoader::getLoader();
				loader->connectDisplayServer(dsvr);
				break;
		}
	}
	AVLoader::shutdown();
	murmur("shutdown");
	delete dsvr;
	return 0;
}
