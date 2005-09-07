#ifndef AVBuffer_h
#define AVBuffer_h
#include "OpenVanilla.h"
#include "AVDisplayServer.h"
#include <string>
#include <vector>
using namespace std;

class AVBuffer : public OVBuffer {
public:
    AVBuffer(AVDisplayServer* svr) : dsvr(svr) {};
    virtual OVBuffer* clear() { 
        bufstr="";
        return this;
    }
    virtual OVBuffer* append(const char *s) {
        bufstr+=s;
        return this;
    }
    virtual OVBuffer* send() { 
	if(bufstr!="") {
		dsvr->sendBuf(bufstr.c_str());
		bufstr="";
	}
        return this;
    }
    virtual OVBuffer* update() { 
	if(bufstr!="") {
		dsvr->setBufString(bufstr.c_str());
	}
        return this;
    }
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1) {
	    dsvr->setCursorPos(cursorPos);
	    dsvr->setMarkFrom(markFrom);
	    dsvr->setMarkTo(markTo);
	    return update();
    }
    virtual int isEmpty() {
        if (!bufstr.length()) return 1; return 0;
    }

private:
    string bufstr;
    AVDisplayServer *dsvr;
};
#endif // AVBuffer_h
