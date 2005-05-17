// CVKeyCode.h

#ifndef __CVKeyCode_h
#define __CVKeyCode_h

#include <Carbon/Carbon.h>
#include <OpenVanilla/OpenVanilla.h>

// the CVSmartMenu extension for CVKeyCode works like this, for example:
//     CVKeyCode k("k", "MOCS");
// means CMD+OPT+CTRL+SHIFT+K; m=CMD, o=OPT, c=CTRL, S=SHIFT (case insensitive)

class CVKeyCode : public OVKeyCode
{
public:
    CVKeyCode(char charcode, UInt32 modifiers);
    virtual int code()       { return c; }
    virtual int isShift();
    virtual int isCtrl();
    virtual int isAlt();
    virtual int isOpt()      { return isAlt(); }
    virtual int isCommand();
    virtual int isCapslock();
	
	// extensions for CVSmartMenu
	CVKeyCode(const char *charcode, const char *modifiers);
	CVKeyCode(NSString *s);
	virtual BOOL equalToKey(CVKeyCode *k, BOOL ignorecase=YES);
	virtual UInt8 convertToMenuModifier();
    
    // facility functions for OVPreference
    virtual NSArray *getKeyList();
    virtual NSString *getKeyCodeString();
    virtual NSString *getModifierString();
    virtual NSString *getModifierIconString();
protected:
	void init(const char *charcode, const char *modifiers);
    char c;
    UInt32 m;
};

#endif
