#include "ConsoleDisplayServer.h"
#include <iostream>
using namespace std;

AVDisplayServer *ConsoleDisplayServer::setBufString(const char *str)
{
	cout << "setBufString " << str << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::sendBuf(const char *str)
{
	cout << "sendBuf " << str << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::setCandiString(const char *str)
{
	cout << "setCandiString " << str << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::showBuf(bool t)
{
	cout << "showBuf " << t << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::showCandi(bool t)
{
	cout << "showCandi " << t << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::showNotify(const char *str)
{
	cout << "showNotify " << str << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::hideNotify()
{
	cout << "hideNotify" << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::setCursorPos(int i)
{
	cout << "setCursorPos " << i << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::setMarkFrom(int i)
{
	cout << "setMarkFrom " << i << endl;
	return this;
}
AVDisplayServer *ConsoleDisplayServer::setMarkTo(int i)
{
	cout << "setMarkTo " << i << endl;
	return this;
}
