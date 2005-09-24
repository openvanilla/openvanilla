#include "DummyDisplayServer.h"
#include <iostream>
using namespace std;

DummyDisplayServer::DummyDisplayServer(UDPSocket *s) : sock(s) {};

AVDisplayServer *DummyDisplayServer::setBufString(const char *str)
{
	//cout << "setBufString " << str << endl;
	sock->send(SETBUFSTRING, (char*)str);
	return this;
}
AVDisplayServer *DummyDisplayServer::sendBuf(const char *str)
{
	//cout << "sendBuf " << str << endl;
	sock->send(SENDBUF, (char*)str);
	return this;
}
AVDisplayServer *DummyDisplayServer::setCandiString(const char *str)
{
	//cout << "setCandiString " << str << endl;
	sock->send(SETCANDISTRING, (char*)str);
	return this;
}
AVDisplayServer *DummyDisplayServer::showBuf(bool t)
{
	//cout << "showBuf " << t << endl;
	sock->send(0, "showBuf");
	t ? sock->send(0, "true") : sock->send(0, "false");
	return this;
}
AVDisplayServer *DummyDisplayServer::showCandi(bool t)
{
	//cout << "showCandi " << t << endl;
	t ? sock->send(SHOWCANDI, "true") : sock->send(SHOWCANDI, "false");
	return this;
}
AVDisplayServer *DummyDisplayServer::showNotify(const char *str)
{
	//cout << "showNotify " << str << endl;
	sock->send(SHOWNOTIFY, (char*)str);
	return this;
}
AVDisplayServer *DummyDisplayServer::hideNotify()
{
	//cout << "hideNotify" << endl;
	sock->send(HIDENOTIFY, "");
	return this;
}
AVDisplayServer *DummyDisplayServer::setCursorPos(int i)
{
	//cout << "setCursorPos " << i << endl;
	char buf[100];
	sprintf(buf, "%d", i);
	sock->send(SETCURSORPOS, buf);
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkFrom(int i)
{
	//cout << "setMarkFrom " << i << endl;
	char buf[100];
	sprintf(buf, "%d", i);
	sock->send(SETMARKFROM, buf);
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkTo(int i)
{
	//cout << "setMarkTo " << i << endl;
	char buf[100];
	sprintf(buf, "%d", i);
	sock->send(SETMARKTO, buf);
	return this;
}
