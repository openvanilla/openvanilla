#ifndef AVCandidate_h
#define AVCandidate_h
#include "OpenVanilla.h"
#include "AVDisplayServer.h"

class AVCandidate : public OVCandidate  {
public:
    AVCandidate(AVDisplayServer* svr) : dsvr(svr), onscreen(0) {}
    virtual OVCandidate* clear() {
        candistr="";
        return this;
    }
    virtual OVCandidate* append(const char *s) {
        candistr+=s;
        return this;
    }
    virtual OVCandidate* hide() {
        if (onscreen) { onscreen=0; dsvr->showCandi(false);}
        return this;
    }
    virtual OVCandidate* show() {
        if (!onscreen) { onscreen=1; dsvr->showCandi(true);}
        return this;
    }
    virtual OVCandidate* update() {
	dsvr->setCandiString(candistr.c_str());
        return this; 
    }
    virtual int onScreen() {
        return onscreen;
    }

protected:
    string candistr;
    int onscreen;
    AVDisplayServer *dsvr;
};
#endif // AVCandidate_h
