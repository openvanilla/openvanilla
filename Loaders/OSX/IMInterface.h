// IMInterface.h

#ifndef __IMInterface_h
#define __IMInterface_h

#include "TSComposingBuffer.h"

struct IMContext
{
    TSComposingBuffer *buf;
    void *data;
    int sessionlock;
    
    IMContext(ComponentInstance i) : data(NULL), sessionlock(0)
        { buf=new TSComposingBuffer(i); }
    ~IMContext() { delete buf; }
};


int IMInitialize(MenuRef mnu);
int IMContextOpen(IMContext *c);
void IMContextClose(IMContext *c);
void IMContextActivate(IMContext *c);
void IMContextDeactivate(IMContext *c);
void IMContextFix(IMContext *c);
int IMContextEvent(IMContext *c, char charcode, unsigned int modifiers);
void IMMenuHandler(IMContext *c, unsigned int cmd);

#endif
