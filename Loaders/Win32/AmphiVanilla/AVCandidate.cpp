#include "AVCandidate.h"

AVCandidate::AVCandidate() : candiOnscreen(0) {}
AVCandidate::AVCandidate(AVDisplayServer* svr) : dsvr(svr), candiOnscreen(0) {}

void AVCandidate::setDisplayServer(AVDisplayServer *svr)
{
	dsvr = svr;
}
OVCandidate* AVCandidate::clear() {
	candistr = "";
	return this;
}
OVCandidate* AVCandidate::append(const char *s) {
	candistr += s;
	return this;
}
OVCandidate* AVCandidate::hide() {
	if (candiOnscreen) { candiOnscreen=0; dsvr->showCandi(false);}
	return this;
}
OVCandidate* AVCandidate::show() {
	if (!candiOnscreen) { candiOnscreen=1; dsvr->showCandi(true);}
	return this;
}
OVCandidate* AVCandidate::update() {
	dsvr->setCandiString(candistr.c_str());
	return this; 
}
int AVCandidate::onScreen() {
	return candiOnscreen;
}
