// CVBuffer.h: CocoaVanilla implementation of OVBuffer

#ifndef __CVBuffer_h
#define __CVBuffer_h

#include <OpenVanilla/OpenVanilla.h>
#include "TSComposingBuffer.h"

class CVBuffer : public OVBuffer {
public:
    CVBuffer(TSComposingBuffer *cb, NSArray *ofa, OVService *s);
    virtual OVBuffer* clear();
    virtual OVBuffer* append(const char *s);
    virtual OVBuffer* send();
    virtual OVBuffer* update();
    virtual OVBuffer* update(int cursorPos, int markFrom=-1, int markTo=-1);
    virtual int isEmpty();
    virtual Point getAppCursorPosition();
    
    virtual CVBuffer* setComposingBuffer(TSComposingBuffer *cb);
protected:
    TSComposingBuffer *cbuf;
    NSArray *ofarray;
    OVService *srv;
};

#endif
