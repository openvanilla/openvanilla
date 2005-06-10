// TSB.mm

// If we follow the ATSC protocol well, we can be 100% sure that 
// (1) our component will only be initialized once
// (2) when initialization fails, no other functions will ever be called
// (3) if TSBContextOpen fails, no other functions will ever be called

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "TSBundle.h"
#include "IMInterface.h"

// #define DEBUG

#ifdef DEBUG
    #define MURMUR(x)   fprintf(stderr, x); fprintf(stderr, "\n");
#else
    #define MURMUR(x)
#endif

// global data (active context and menu reference)
static TSBDataPtr tsbActiveContext=NULL;

extern "C" ComponentResult TSBInitialize(MenuRef mnu)
{
    MURMUR("TSBInitialize");

	if (!NSApplicationLoad()) 
	{
        MURMUR("TSBInitialize: NSApplicationLoad() failed");
        return -1;
	}
	
    // this is sine-qua-non for every Cocoa bundle... as we will 
    // (and cannot) release this pool, we won't have assign anything to it
	[NSAutoreleasePool new];
    
    // initilize the real IM code
    return IMInitialize(mnu) ? noErr : -1;
}

extern "C" ComponentResult TSBContextOpen(ComponentInstance inst, 
    TSBDataPtr* savePtr)
{
    MURMUR("TSBContextOpen");
    IMContext *cntx=new IMContext(inst);
    if (!(*savePtr=cntx) || !cntx->buf) return -1;
    if (!IMContextOpen(cntx)) return -1;
    return noErr;
}

// as we have been said, the functions below will never be called if
// there's anything wrong in the init/context opening process, 
// so we won't do any check
extern "C" ComponentResult TSBContextClose(TSBDataPtr ptr)
{
    MURMUR("TSBContextClose");
    IMContextClose((IMContext*)ptr);
    return noErr;
}

extern "C" ComponentResult TSBContextActivate(TSBDataPtr ptr)
{
    MURMUR("TSBContextActivate");
    tsbActiveContext=ptr;
    IMContextActivate((IMContext*)ptr);
    return noErr;
}

extern "C" ComponentResult TSBContextDeactivate(TSBDataPtr ptr)
{
    MURMUR("TSBContextDeactivate");
    IMContextDeactivate((IMContext*)ptr);
    tsbActiveContext=NULL;
    return noErr;
}

extern "C" ComponentResult TSBContextFix(TSBDataPtr ptr)
{
    MURMUR("TSBContextFix");
    IMContext *c=(IMContext*)ptr;
    if (!c->sessionlock) IMContextFix(c);
    return noErr;
}

extern "C" ComponentResult TSBContextEvent(TSBDataPtr ptr, EventRef evnt)
{
    MURMUR("TSBContextEvent");
    UInt32 eventkind=GetEventKind(evnt);

    // we only take kEventRawKeyDown and kEventRawKeyRepat
    if (GetEventClass(evnt)!=kEventClassKeyboard ||
        !(eventkind==kEventRawKeyDown || eventkind==kEventRawKeyRepeat))
        return FALSE;

    // extract charcode (ASCII) and modifiers
    UInt32 modifiers=0;
    char charcode=0;
    GetEventParameter(evnt, kEventParamKeyMacCharCodes, typeChar, nil,
        sizeof(charcode), nil, &charcode);
    GetEventParameter(evnt, kEventParamKeyModifiers, typeUInt32, nil,
        sizeof(modifiers), nil, &modifiers);
    // fprintf(stderr, "charcode=%d, modifiers=%d\n", charcode, modifiers);

    IMContext *c=(IMContext*)ptr;
    c->sessionlock=1;
    int r=IMContextEvent((IMContext*)ptr, charcode, modifiers);
    c->sessionlock=0;    

    return r;
}

extern "C" ComponentResult TSBMenuHandler(UInt32 cmd)
{
    MURMUR("TSBMenuHandler")
    if (!tsbActiveContext) return eventNotHandledErr;
    IMMenuHandler((IMContext*)tsbActiveContext, cmd);
    return noErr;
}
