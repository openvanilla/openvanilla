// OVLoader.mm: an implementation example of CarbonInputMethod's IM code

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "IMInterface.h"
#include "VXLoader.h"

#define OVDEBUG
#include <OpenVanilla/OVUtility.h>

VXLoader *loader=NULL;

int IMInitialize(MenuRef mnu)
{
    if (loader) return 1;
    
    loader=new VXLoader;    
    if (!loader) return 0;
    return loader->init(mnu);
}

int IMContextOpen(IMContext *c)
{
    return (c->data=loader->newContext()) ? 1 : 0;
}

void IMContextClose(IMContext *c)
{
    if (c->data) delete (VXContext*)(c->data);
}

void IMContextActivate(IMContext *c)
{
    if (c->data) 
    {
        loader->setActiveContext((VXContext*)(c->data));
        loader->syncConfig();
        ((VXContext*)(c->data))->activate(c->buf);
    }
}

void IMContextDeactivate(IMContext *c)
{
    loader->setActiveContext(NULL);
    if (c->data) ((VXContext*)(c->data))->deactivate(c->buf);
}

void IMContextFix(IMContext *c)
{
    if (c->data) ((VXContext*)(c->data))->fix(c->buf);
}

int IMContextEvent(IMContext *c, char charcode, unsigned int modifiers)
{
    if (c->data) return ((VXContext*)(c->data))->event(c->buf, charcode, modifiers);
    return 0;
}

void IMMenuHandler(IMContext *c, unsigned int cmd)
{
    loader->menuHandler(cmd);
}

