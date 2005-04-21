// TSComposingBuffer.h

#ifndef __TSComposingBuffer_h
#define __TSComposingBuffer_h

class TSComposingBuffer
{
public:
    TSComposingBuffer(ComponentInstance i);
    ~TSComposingBuffer();
    TSComposingBuffer* clear();
    TSComposingBuffer* send();
    TSComposingBuffer* update(Boolean send=FALSE, int cursor=-1,
        int markFrom=-1, int markTo=-1);
    TSComposingBuffer* append(NSString* s);
    Boolean isEmpty();
    
    Point getAppCursorPosition();
    NSMutableString* getContent();
    
protected:
    int realPos(int p);         // convert codepoint pos to real UniChar position
    ComponentInstance inst;
    
    int lastupdate;
    NSMutableString* str;
};

#endif
