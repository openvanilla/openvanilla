#include "AVCandidate.h"

AVCandidate::AVCandidate(AVDisplayServer* svr) : dsvr(svr), onscreen(0) {}

OVCandidate* AVCandidate::clear() {
	candistr = "";
	return this;
}
OVCandidate* AVCandidate::append(const char *s) {
	candistr += s;
	return this;
}
OVCandidate* AVCandidate::hide() {
	if (onscreen) { onscreen=0; dsvr->showCandi(false);}
	return this;
}
OVCandidate* AVCandidate::show() {
	if (!onscreen) { onscreen=1; dsvr->showCandi(true);}
	return this;
}
OVCandidate* AVCandidate::update() {
	dsvr->setCandiString(candistr.c_str());
	return this; 
}
int AVCandidate::onScreen() {
	return onscreen;
}
