// CVCandidate.h

#ifndef __CVCandidate_h
#define __CVCandidate_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include "CVInfoBox.h"

class CVCandidate : public OVCandidate  {
public:
    CVCandidate(CVInfoBox *ib);
    virtual OVCandidate* clear();
    virtual OVCandidate* append(const char *s);
    virtual OVCandidate* hide();
    virtual OVCandidate* show();
    virtual OVCandidate* update();
    virtual int onScreen();
    
    virtual OVCandidate* setPosition(Point p);
    virtual int height();
protected:
    CVInfoBox *infobox;
};


#endif
