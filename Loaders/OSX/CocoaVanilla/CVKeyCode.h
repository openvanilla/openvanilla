// CVKeyCode.h

#ifndef __CVKeyCode_h
#define __CVKeyCode_h

#include <OpenVanilla/OpenVanilla.h>

class CVKeyCode : public OVKeyCode
{
public:
    CVKeyCode(char charcode, UInt32 modifiers) : c(charcode), m(modifiers) {}
    virtual int code()       { return c; }
    virtual int isShift()    { return m & (shiftKey | rightShiftKey); }
    virtual int isCtrl()     { return m & (controlKey | rightControlKey); }
    virtual int isAlt()      { return m & (optionKey | rightOptionKey); }
    virtual int isOpt()      { return isAlt(); }
    virtual int isCommand()  { return m & cmdKey; }
    virtual int isCapslock() { return m&(alphaLock|kEventKeyModifierNumLockMask); }
protected:
    char c;
    UInt32 m;
};

#endif
