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
    virtual int isshift()
        { return modifiers & (controlKey | rightControlKey); }
    virtual int iscapslock()
        { return modifiers & alphaLock; }
    virtual int isctrl()
        { return mofidifers & (controlKey | rightControlKey); }
    virtual int isalt()
        { return isopt(); }
    virtual int isopt()
        { return modifiers & (optionKey | rightOptionKey); }
    virtual int iscommand()
        { return modifiers & cmdKey; }
    virtual void set(unsigned char c, UInt32 k, UInt32 m)
        { charcode=c; keycode=k; modifiers=m; }

protected:
    unsigned char charcode;
    UInt32 keycode;
    UInt32 modifiers;
};

#endif