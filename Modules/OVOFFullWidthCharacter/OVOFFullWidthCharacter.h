// OVOFFullWidthCharacter.h

#ifndef __OVOFFullWidthCharacter_h
#define __OVOFFullWidthCharacter_h

#include <OpenVanilla/OpenVanilla.h>

class OVOFFullWidthCharacter : public OVOutputFilter
{
public:
    virtual const char *identifier() { return "OVOFFullWidthCharacter"; }
    virtual const char *localizedName(const char *locale);
    virtual const char *process (const char *src, OVService *srv);
};

#endif


