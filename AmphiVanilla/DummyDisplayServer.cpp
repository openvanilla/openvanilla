#define OV_DEBUG
#include "DummyDisplayServer.h"
#include <iostream>
using namespace std;

/*
AVDisplayServer *DummyDisplayServer::setBufString(const char *str)
{
	//cout << "setBufString " << str << endl;
	return this;
}
*/
AVDisplayServer *DummyDisplayServer::setBufString(const char *str,int caretX, int markFrom, int markTo)
{
	murmur("setbufstring:%s",str);
	//cout << "setBufString " << str << endl;
	return this;

}
AVDisplayServer *DummyDisplayServer::sendBuf(const char *str)
{
	murmur("sendbufstring:%s",str);
	//cout << "sendBufString " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setCandiString(const char *str)
{
	murmur("setcandistring:%s",str);
	//cout << "setCandiString " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::showBuf(bool t)
{
	//cout << "showBuf " << t << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::showCandi(bool t)
{
	//cout << "showCandi " << t << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::showNotify(const char *str)
{
	//cout << "showNotify " << str << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::hideNotify()
{
	//cout << "hideNotify" << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setCursorPos(int i)
{
	//cout << "setCursorPos " << i << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkFrom(int i)
{
	//cout << "setMarkFrom " << i << endl;
	return this;
}
AVDisplayServer *DummyDisplayServer::setMarkTo(int i)
{
	//cout << "setMarkTo " << i << endl;
	return this;
}



bool DummyDisplayServer::getisChineseFlag()
{
	//cout << "getisChineseFlag" << endl;
	return true;
}

bool DummyDisplayServer::getisFullShapeFlag()
{
	//cout << "getisFullFlag" << endl;
	return true;
}

int DummyDisplayServer::getCandSelectedItemKey()
{
	//cout << "getCandSelectedItemKey" << endl;
	return 0;
}

bool DummyDisplayServer::getStatusModuleChecked(int index)
{
	//cout << "getStatusModuleChecked " << index << endl;
	return true;
}

AVDisplayServer * DummyDisplayServer::setStatusModuleChecked(int index, bool b)
{
	//cout << "setStatusModuleChecked " << index <<":"<< b << endl;
	return this;
}
