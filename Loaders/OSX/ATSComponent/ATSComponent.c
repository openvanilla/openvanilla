// ATSComponent.c: Apple Text Service Component for OpenVanilla
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <Carbon/Carbon.h>
#include "ATSComponent.h"
#include "TSBundle.h"

// #define DEBUG

#ifdef DEBUG
    #define MURMUR(x)       fprintf (stderr, "%s\n", x);
    #define TSMSG(cnd, x)   if (cnd) fprintf (stderr, "%s\n", x); \
                            else fprintf (stderr, "%s: (empty context)\n", x);
#else
    #define MURMUR(x)
    #define TSMSG(cnd, x)
#endif

// ACRT stands for "Apple component return type"
#define ACRT    pascal ComponentResult      // no extern "C" here

// main entry point
ACRT ATSCDispatch(ComponentParameters *param, Handle hndl);

// we implement these in this module
ACRT ATSCCanDo(SInt16 selector);
ACRT ATSCGetVersion();
ACRT ATSCGetScriptLangSupport(Handle hndl, ScriptLanguageSupportHandle *outhndl);

// component entry points
ACRT ATSCOpenComponent(ComponentInstance inst);
ACRT ATSCCloseComponent(Handle hndl, ComponentInstance inst);

// text service component entry points
ACRT ATSCInitiateTextService(Handle hndl);
ACRT ATSCTerminateTextService(Handle hndl);
ACRT ATSCActivateTextService(Handle hndl);
ACRT ATSCDeactivateTextService(Handle hndl);
ACRT ATSCTextServiceEvent(Handle hndl, EventRef evnt);
ACRT ATSCGetTextServiceMenu(Handle hndl, MenuHandle *mnu);
ACRT ATSCFixTextService(Handle hndl);
ACRT ATSCHidePaletteWindows(Handle hndl);

// initializer (pencil menu, bundle loading, call TSB init)
ACRT ATSCInitialize(ComponentInstance inst);

// menu handler
pascal OSStatus ATSCMenuHandler(EventHandlerCallRef callref,
    EventRef evnt, void *userdata);

// global variables
int atscInitialized=0;
CFBundleRef tsbRef=NULL;
MenuRef atscMenu=nil;

// loaded function pointers
_tsbinit_t *tsbInitialize=NULL;
_tsbopen_t *tsbContextOpen=NULL;
_tsbfunc_t *tsbContextClose=NULL, 
    *tsbContextActivate=NULL,
    *tsbContextDeactivate=NULL,
    *tsbContextFix=NULL;
_tsbevent_t *tsbContextEvent=NULL;
_tsbmenu_t *tsbMenuHandler=NULL;

// initialization, this is called by ATSCOpenComponent()
ACRT ATSCInitialization(ComponentInstance inst)
{
    MURMUR("ATSCInitialize: text service component initialization");
    atscInitialized=1;

    MURMUR("ATSCInitialize: getting pencil menu");
        
    // open resource fork
    short rfref=OpenComponentResFile((Component)inst);
    if (rfref==-1)
    {
        MURMUR("ATSCInitialize: resource not found, init failed");
        return resNotFound;
    }
        
    atscMenu=GetMenu(ATSCPENCILMENUID);
    CloseComponentResFile(rfref);
    if (!atscMenu)
    {
        MURMUR("ATSCInitialize: pencil menu not found, init failed");
        return resNotFound;
    }
        
    MURMUR("ATSCInitialize: setting menu handler");
    EventTypeSpec s;
    ComponentResult result;
    s.eventClass=kEventClassCommand;
    s.eventKind=kEventProcessCommand;
    if ((result=InstallMenuEventHandler(atscMenu, 
        NewEventHandlerUPP(ATSCMenuHandler), 1, &s, nil, nil)) != noErr)
    {
        MURMUR("ATSCInitialize: cannot setup menu handler, init failed");
        return result;
    }
               
    MURMUR("ATSCInitialize: loading text service bundle: " TSBUNDLEFILENAME);    
    CFURLRef url=CFURLCreateWithFileSystemPath(NULL, CFSTR(TSBUNDLEFILENAME), 
        kCFURLPOSIXPathStyle, true);
    tsbRef=CFBundleCreate(NULL, url);
    CFRelease(url);

    if (!tsbRef)
    {
        MURMUR("ATSCInitialize: failed loading the bundle");
        return noErr;
    }
        
    #define GETPOINTER(x) CFBundleGetFunctionPointerForName(tsbRef, CFSTR(x))
    if (
        !(tsbInitialize=(_tsbinit_t*)GETPOINTER("TSBInitialize")) ||
        !(tsbContextOpen=(_tsbopen_t*)GETPOINTER("TSBContextOpen")) ||
        !(tsbContextClose=(_tsbfunc_t*)GETPOINTER("TSBContextClose")) ||
        !(tsbContextActivate=(_tsbfunc_t*)GETPOINTER("TSBContextActivate")) ||
        !(tsbContextDeactivate=(_tsbfunc_t*)GETPOINTER("TSBContextDeactivate")) ||
        !(tsbContextFix=(_tsbfunc_t*)GETPOINTER("TSBContextFix")) ||
        !(tsbContextEvent=(_tsbevent_t*)GETPOINTER("TSBContextEvent")) ||
        !(tsbMenuHandler=(_tsbmenu_t*)GETPOINTER("TSBMenuHandler"))
        )
    {
        MURMUR("ATSCInitialize: failed loading functions, tsbRef reverted");
        tsbRef=NULL;
        return noErr;
    }
        
    // load the dynamically loadable bundle
    if ((result=tsbInitialize(atscMenu)) != noErr)
    {
        MURMUR("ATSCInitialize: TSBInitialize() failed, tsbRef reverted");
        tsbRef=NULL;
        return result;
    }        
    
    return noErr; 
}

// creates a new session context, before that we initialize the component
// if it's not yet done
ACRT ATSCOpenComponent(ComponentInstance inst)
{
    ComponentResult result=noErr;
    TSBDataPtr ptr=NULL;

    // if this is the first stance of ATSC, initialize the global state:
    //     (1) load text service menu ("the pencil menu"),
    //     (2) install menu event handler
    //     (3) initialize global (inter-session) variables
    if (!atscInitialized) 
        if ((result=ATSCInitialization(inst)) != noErr) return result;
       
    MURMUR("ATSCOpenComponent: creating a new session content");

    if (tsbRef)
    {
        // initialize the new session
        if ((result=tsbContextOpen(inst, (TSBDataPtr*)&ptr)) != noErr)
            return result;            
    }
    else
    {
        MURMUR("ATSCOpenComponent: no bundle loaded, empty context created");
    }
        
    SetComponentInstanceStorage(inst, (Handle)ptr);     // save the allocated data
    return noErr;
}

// close the given instance. since we don't do reference counting, we won't
// know if that's the last closed instance (henceforth meaning the end
// of the text service component--but in fact we won't know anyway, as
// the text service seems to never terminate under OS X, so we leave the
// clear up thing to the operating system
ACRT ATSCCloseComponent(Handle hndl, ComponentInstance inst)
{
    TSMSG(hndl && tsbRef, "ATSCCloseComponent");
    if (inst==nil) return paramErr;
    if (hndl && tsbRef) 
    {
        MURMUR("ATSCCloseComponent: closing context");
        tsbContextClose((TSBDataPtr)hndl);
    }
    
    SetComponentInstanceStorage(inst, nil);     // remove instance storage pointer
    return noErr;
}

ACRT ATSCActivateTextService(Handle hndl)
{
    TSMSG(tsbRef && hndl, "ATSCActivateTextService");
    return (tsbRef && hndl) ? tsbContextActivate((TSBDataPtr)hndl) : noErr;
}

ACRT ATSCDeactivateTextService(Handle hndl)
{
    TSMSG(tsbRef && hndl, "ATSCDeactivateTextService");
    return (tsbRef && hndl) ? tsbContextDeactivate((TSBDataPtr)hndl) : noErr;
}

ACRT ATSCTextServiceEvent(Handle hndl, EventRef evnt)
{
    TSMSG(tsbRef && hndl, "ATSCTextServiceEvent");
    return (tsbRef && hndl) ? tsbContextEvent((TSBDataPtr)hndl, evnt) : noErr;
}

ACRT ATSCFixTextService(Handle hndl)
{
    TSMSG(tsbRef && hndl, "ATSCFixTextService");
    return (tsbRef && hndl) ? tsbContextFix((TSBDataPtr)hndl) : noErr;
}

pascal OSStatus ATSCMenuHandler(EventHandlerCallRef callref, EventRef evnt, 
    void *userdata)
{
    HICommand command;
    TSMSG(tsbRef, "ATSCMenuHandler");
 	if (GetEventParameter(evnt, kEventParamDirectObject, typeHICommand, nil,
        sizeof(command), nil, &command)!=noErr) return eventNotHandledErr;
    return tsbRef ? tsbMenuHandler(command.commandID) : eventNotHandledErr;        
}

// --------------------------------------------------------------------------
// text service functions implemented by  this module
// --------------------------------------------------------------------------

// returns our "pencil" menu
ACRT ATSCGetTextServiceMenu(Handle hndl, MenuHandle *mnu)
{
    TSMSG(tsbRef && hndl, "ATSCGetTextServiceMenu");
    *mnu=atscMenu;
    return noErr;
}

ACRT ATSCGetVersion()
{
    TSMSG(tsbRef, "ATSCGetVersion");
    return ATSCVERSION;
}

// returns true if what indicates in the selector is what we can do
ACRT ATSCCanDo(SInt16 selector)
{
    TSMSG(tsbRef, "ATSCCando");
    switch(selector)
    {
        case kComponentOpenSelect:
        case kComponentCloseSelect:
        case kComponentCanDoSelect:
        case kComponentVersionSelect:
        case kCMGetScriptLangSupport:
        case kCMInitiateTextService:
        case kCMTerminateTextService:
        case kCMActivateTextService:
        case kCMDeactivateTextService:
        case kCMTextServiceEvent:
        case kCMGetTextServiceMenu:
        case kCMFixTextService:
        case kCMHidePaletteWindows:
            return true;
    }
    return false;
}

// Apple's Text Service Manager calls this function to determine the
// type of our input method
ACRT ATSCGetScriptLangSupport(Handle hndl, ScriptLanguageSupportHandle *outhndl)
{
    // #pragma unused (hndl)
    TSMSG(tsbRef && hndl, "ATSCGetScriptLangSupport");

    OSStatus result=noErr;
    ScriptLanguageRecord scriptrecord;
    
    if (!*outhndl)  // allocate space for handle if null
    {
        if (!(*outhndl=(ScriptLanguageSupportHandle)NewHandle(sizeof(SInt16))))
            return memFullErr;
    }

    SetHandleSize((Handle)*outhndl, sizeof(SInt16));
    if ((result=MemError()) != noErr) return result;
    
    (**outhndl)->fScriptLanguageCount=0;

    scriptrecord.fScript=kTextEncodingUnicodeDefault;
    scriptrecord.fLanguage=ATSCLANGUAGE;
    if (result=PtrAndHand(&scriptrecord, (Handle)*outhndl, 
        sizeof(ScriptLanguageRecord)))
    {
        if (*outhndl)
        {
            DisposeHandle((Handle)*outhndl);
            *outhndl=NULL;
        }
        return result;
    }
        
    (**outhndl)->fScriptLanguageCount++;
    
    return noErr; 
}

// --------------------------------------------------------------------------
// entry point of the text service component
// --------------------------------------------------------------------------

// UPP entry points
#define UPPBASE kPascalStackBased | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
#define UPPSTACK(n, t) STACK_ROUTINE_PARAMETER(n, SIZE_CODE(sizeof(t)))
enum {
    uppOpenComponent = UPPBASE | UPPSTACK(1, ComponentInstance),
    uppCloseComponent = UPPBASE |  UPPSTACK(1, Handle) | 
        UPPSTACK(2, ComponentInstance),
    uppCanDo = UPPBASE | UPPSTACK(1, short),
    uppGetVersion = UPPBASE,
    uppGetScriptLangSupport = UPPBASE | UPPSTACK(1, Handle) |
        UPPSTACK(2, ScriptLanguageSupportHandle*),
    uppInitiateTextService = UPPBASE | UPPSTACK(1, Handle),
    uppTerminateTextService = UPPBASE | UPPSTACK(1, Handle),
    uppActivateTextService = UPPBASE | UPPSTACK(1, Handle),
    uppDeactivateTextService = UPPBASE | UPPSTACK(1, Handle),
    uppTextServiceEvent = UPPBASE | UPPSTACK(1, Handle) |
        UPPSTACK(2, EventRef),
    uppGetTextServiceMenu = UPPBASE | UPPSTACK(1, Handle) |
        UPPSTACK(2, MenuHandle*),
    uppFixTextService = UPPBASE | UPPSTACK(1, Handle),
    uppHidePaletteWindows = UPPBASE | UPPSTACK(1, Handle)
};

// service functions
ComponentResult CallATSCFunc(ComponentParameters *param, ProcPtr proc, 
    SInt32 procinfo);
ComponentResult CallATSCFuncStorage(Handle hndl, ComponentParameters *param, 
    ProcPtr proc, SInt32 procinfo);

#define CALLCOMPONENT(x)    CallATSCFunc(param, (ProcPtr)ATSC##x, upp##x)
#define CALLCOMPONENTSTORAGE(x) CallATSCFuncStorage(hndl, param, (ProcPtr)ATSC##x, upp##x)

// the main entry point of a text service component is simply a dispatcher
ACRT ATSCDispatch(ComponentParameters *param, Handle hndl)
{
	switch (param->what)
	{
        // functions that we'll call dynamically loaded functions to do
		case kComponentOpenSelect:
			return CALLCOMPONENT(OpenComponent);
        case kComponentCloseSelect:
            return CALLCOMPONENTSTORAGE(CloseComponent);
        case kCMActivateTextService:
            return CALLCOMPONENTSTORAGE(ActivateTextService);
            break;
        case kCMDeactivateTextService:
			return CALLCOMPONENTSTORAGE(DeactivateTextService);
            break;
        case kCMTextServiceEvent:
            return CALLCOMPONENTSTORAGE(TextServiceEvent);
        case kCMFixTextService:
            return CALLCOMPONENTSTORAGE(FixTextService);

        // these we'll implement ourselves            
        case kComponentCanDoSelect:
            return CALLCOMPONENT(CanDo);
        case kComponentVersionSelect:
            return CALLCOMPONENT(GetVersion);
        case kCMGetScriptLangSupport:
            return CALLCOMPONENTSTORAGE(GetScriptLangSupport);
        case kCMGetTextServiceMenu:
            return CALLCOMPONENTSTORAGE(GetTextServiceMenu);
        case kCMInitiateTextService:
            MURMUR("init!");
            return noErr;
        case kCMTerminateTextService:
            MURMUR("terminate!");
            return noErr;
        case kCMHidePaletteWindows:
            return noErr;
        default:
            return badComponentSelector;
    }
}

// glue code that creates a universal procedure pointer (upp) for the
// internal dispatch routines and calls them
ComponentResult CallATSCFunc(ComponentParameters *param, ProcPtr proc,
    SInt32 procinfo)
{
    ComponentFunctionUPP upp=NewComponentFunctionUPP(proc, procinfo); 
    ComponentResult result=CallComponentFunction(param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}

ComponentResult CallATSCFuncStorage(Handle hndl,
    ComponentParameters *param, ProcPtr proc, SInt32 procinfo)
{
    ComponentFunctionUPP upp=NewComponentFunctionUPP(proc, procinfo);
    ComponentResult result=CallComponentFunctionWithStorage(hndl, param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}

