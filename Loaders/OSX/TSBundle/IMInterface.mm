// OVLoader.mm: an implementation example of CarbonInputMethod's IM code

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "IMInterface.h"
#include "CVLoader.h"

// #define OVDEBUG
#include <OpenVanilla/OVUtility.h>

CVLoader *loader=NULL;

int IMInitialize(MenuRef mnu)
{
    if (loader) return 1;
    
    loader=new CVLoader;    
    if (!loader) return 0;
    return loader->init(mnu);
}

int IMContextOpen(IMContext *c)
{
    return (c->data=loader->newContext()) ? 1 : 0;
}

void IMContextClose(IMContext *c)
{
    if (c->data) delete (CVContext*)(c->data);
}

void IMContextActivate(IMContext *c)
{
    if (c->data) ((CVContext*)(c->data))->activate(c->buf);
}

void IMContextDeactivate(IMContext *c)
{
    if (c->data) ((CVContext*)(c->data))->deactivate();
}

void IMContextFix(IMContext *c)
{
    if (c->data) ((CVContext*)(c->data))->fix();
}

int IMContextEvent(IMContext *c, char charcode, unsigned int modifiers)
{
    if (c->data) return ((CVContext*)(c->data))->event(charcode, modifiers);
    return 0;
}

void IMMenuHandler(IMContext *c, unsigned int cmd)
{
    loader->menuHandler(cmd);
}

