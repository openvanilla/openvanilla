// VXKeyCode.h

#ifndef __VXKeyCode_h
#define __VXKeyCode_h

#include <Carbon/Carbon.h>
#include "OpenVanilla.h"

class VXKeyCode : public OVKeyCode
{
public:
    virtual int code()
        { return charcode; }
    virtual int isShift()
        { return modifiers & (shiftKey | rightShiftKey); }
    virtual int isCapslock()
        { return modifiers & alphaLock; }
    virtual int isCtrl()
        { return modifiers & (controlKey | rightControlKey); }
    virtual int isAlt()
        { return isopt(); }
    virtual int isOpt()
        { return modifiers & (optionKey | rightOptionKey); }
    virtual int isCommand()
        { return modifiers & cmdKey; }
    virtual void set(unsigned char c, UInt32 k, UInt32 m)
        { charcode=c; keycode=k; modifiers=m; }

protected:
    unsigned char charcode;
    UInt32 keycode;
    UInt32 modifiers;
};

#endif