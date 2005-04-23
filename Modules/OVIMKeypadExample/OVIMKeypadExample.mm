#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include <OpenVanilla/OVLibrary.h>
#include "KPController.h"

KPController *kpc;

class OVIMKeypadContext : public OVInputMethodContext {
public:
    virtual void start(OVBuffer* b, OVCandidate*, OVService*) {
        [kpc setBuffer:b];
        [[kpc window] orderFront:kpc];
    }
    virtual void end() {
        [kpc setBuffer:NULL];
        [[kpc window] orderOut:kpc];
    }
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*) {
        return 0;
    }
};

class OVIMKeypadExample : public OVInputMethod {
public:
    virtual const char *identifier() { return "OVIMKeyPadExample"; }
    virtual OVInputMethodContext *newContext() { return new OVIMKeypadContext; }
};

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;

}
extern "C" int OVInitializeLibrary(OVService*, const char*) { 
    NSApplicationLoad();
    kpc=[[KPController alloc] initWithWindowNibName:@"OVIMKeypadExample"];
    if (kpc) {
        NSLog(@"KPController loaded!");
    }
    else {
        NSLog(@"KPController loaded!");
    }
    [kpc window];
    [(NSPanel*)[kpc window] setFloatingPanel:YES];
    [(NSPanel*)[kpc window] setBecomesKeyOnlyIfNeeded:YES];

    return 1;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    return (idx==0) ? new OVIMKeypadExample : NULL;
}
