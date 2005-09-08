#include "AVBuffer.h"

AVBuffer::AVBuffer(AVDisplayServer* svr) : dsvr(svr) {};

OVBuffer* AVBuffer::clear()
{
	bufstr = "";
	return this;
}
OVBuffer* AVBuffer::append(const char *s)
{
	bufstr += s;
	return this;
}
OVBuffer* AVBuffer::send()
{
	dsvr->sendBuf(bufstr.c_str());
	return this;
}
OVBuffer* AVBuffer::update()
{
	dsvr->setBufString(bufstr.c_str());
	return this;
}
OVBuffer* AVBuffer::update(int cursorPos, int markFrom=-1, int markTo=-1)
{
	dsvr->setCursorPos(cursorPos);
	dsvr->setMarkFrom(markFrom);
	dsvr->setMarkTo(markTo);
	return update();
}
int AVBuffer::isEmpty()
{
	if (!bufstr.length()) return 1; return 0;
}
