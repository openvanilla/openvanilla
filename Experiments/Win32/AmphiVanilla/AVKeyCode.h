#ifndef AVKeyCode_h
#define AVKeyCode_h
#include "OpenVanilla.h"
#include <ctype.h>

class AVKeyCode : public OVKeyCode  {
public:
    AVKeyCode (int p=0)          { chr=toupper(p); shift=capslock=ctrl=alt=0; }
    virtual int code()              { return (shift||capslock||ctrl||alt) ? chr : tolower(chr); }
    virtual int isShift()           { return shift; }
    virtual int isCapslock()        { return capslock; }
    virtual int isCtrl()            { return ctrl; }
    virtual int isAlt()             { return alt; }
    
    virtual void setCode(int x)     { chr=toupper(x); }
    virtual void setShift(int x)    { shift=x; }
    virtual void setCapslock(int x) { capslock=x; }
    virtual void setCtrl(int x)     { ctrl=x; }
    virtual void setAlt(int x)      { alt=x; }
protected:
    int chr;
    int shift, capslock, ctrl, alt;
};
#endif // AVKeyCode_h
