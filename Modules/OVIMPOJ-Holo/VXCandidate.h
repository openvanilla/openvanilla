// VXCandidate.h

#ifndef __VXCandidate_h
#define __VXCandidate_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include "OVDeprecated.h"

class VXCandidate // : public OVObject
{
public:
    VXCandidate()
    {
        onduty=0;
    }
    virtual ~VXCandidate() {}
    
    void prepare(NSArray *l, char *skey, OVTextBar *textbar);
    int onDuty() { return onduty; }
    int onePage() { return (count <= perpage); }
    void cancel() { onduty=0; }
    void update(OVTextBar *textbar);
    VXCandidate* pageUp();
    VXCandidate* pageDown();
    NSString* select(char c);
    void backOnDuty() { onduty=1; }
    
protected:
    int onduty;
    char selkey[32];
    int count;
    int perpage;
    int pos;
    NSArray *list;
};

#endif
