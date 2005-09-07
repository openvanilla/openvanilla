#ifndef AVKeyCode_h
#define AVKeyCode_h
#include "OpenVanilla.h"

class AVKeyCode : public OVKeyCode  {
public:
    AVKeyCode (int p=0)          { chr=p; shift=capslock=ctrl=alt=0; }
    virtual int code()              { return chr & 0x00FF; }
    virtual int isShift()           { return chr & 0x0100; }
    virtual int isCapslock()        { return chr & 0x0400; }
    virtual int isCtrl()            { return ctrl; }
    virtual int isAlt()             { return alt; }
    
    virtual void setCode(int x)     { chr=x; }
    virtual void setShift(int x)    { shift=x; }
    virtual void setCapslock(int x) { capslock=x; }
    virtual void setCtrl(int x)     { ctrl=x; }
    virtual void setAlt(int x)      { alt=x; }
protected:
    int chr;
    int shift, capslock, ctrl, alt;
};
#endif // AVKeyCode_h
