#include "DummyDisplayServer.h"
#include <iostream>
using namespace std;

AVDisplayServer *DummyDisplayServer::setBufString(const char *str)
{
	cout << "setBufString " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::sendBuf(const char *str)
{
	cout << "sendBufString " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setCandiString(const char *str)
{
	cout << "setCandiString " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::showBuf(bool t)
{
	cout << "showBuf " << t << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::showCandi(bool t)
{
	cout << "showCandi " << t << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setCursorPos(int i)
{
	cout << "setCursorPos " << i << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkFrom(int i)
{
	cout << "setMarkFrom " << i << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkTo(int i)
{
	cout << "setMarkTo " << i << endl;
	return this;
}
