// [subversion header]

// CIMComponent.c:
//     Carbon Input Method, an input method framework written using
//     Apple's Carbon Application Framework

// Code based on Apple's BasicInputMethod. This is a rewritten version to
// provide C++ linkage and a more clear, minimalist input method framework.

// Copyright (c) 2004 by Lukhos D. Liu

// [BSD license]

#include <Carbon/Carbon.h>
#include "CIM.h"
#include "CIMconst.h"

// ACRT stands for "Apple component return type"
#define ACRT    extern "C" pascal ComponentResult

// main entry point
ACRT CIMComponentDispatch(ComponentParameters *param, Handle hndl);

// component entry points
ACRT CIMOpenComponent(ComponentInstance inst);
ACRT CIMCloseComponent(Handle hndl, ComponentInstance inst);
ACRT CIMCanDo(SInt16 selector);
ACRT CIMGetVersion();

// text service component entry points
ACRT CIMGetScriptLangSupport(Handle hndl, ScriptLanguageSupportHandle *outhndl);
ACRT CIMInitiateTextService(Handle hndl);
ACRT CIMTerminateTextService(Handle hndl);
ACRT CIMActivateTextService(Handle hndl);
ACRT CIMDeactivateTextService(Handle hndl);
ACRT CIMTextServiceEventRef(Handle hndl, EventRef evnt);
ACRT CIMGetTextServiceMenu(Handle hndl, MenuHandle *mnu);
ACRT CIMFixTextService(Handle hndl);
ACRT CIMHidePaletteWindows(Handle hndl);

// universal procedure pointers (UPP's) for the component entry points;
// this is used by the component's dispatch function

enum {
    uppOpenComponent = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(ComponentInstance))),
    uppCloseComponent = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle)))
        | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ComponentInstance))),
    uppCanDo = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(short))),
    uppGetVersion = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult))),
    uppGetScriptLangSupport = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle)))
        | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(ScriptLanguageSupportHandle *))),
    uppInitiateTextService = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle))),
    uppTerminateTextService = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle))),
    uppActivateTextService = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle))),
    uppDeactivateTextService = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle))),
    uppTextServiceEventRef = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle)))
        | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EventRef))),
    uppGetTextServiceMenu = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle)))
        | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(MenuHandle *))),
    uppFixTextService = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle))),
    uppHidePaletteWindows = kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Handle)))
};

// global variables
long cimInstanceRefcount=0;
MenuRef cimTextServiceMenu=nil;

// service functions
ComponentResult CallCIMFunc(ComponentParameters *param, ProcPtr proc, 
    SInt32 procinfo);
ComponentResult CallCIMFuncStorage(Handle hndl, ComponentParameters *param, 
    ProcPtr proc, SInt32 procinfo);
    
#define CALLCOMPONENT(x)    CallCIMFunc(param, (ProcPtr)CIM##x, upp##x)
#define CALLCOMPONENTSTORAGE(x) CallCIMFuncStorage(hndl, param, (ProcPtr)CIM##x, upp##x)

// the main entry point of a text service component is simply a dispatcher
ACRT CIMComponentDispatch(ComponentParameters *param, Handle hndl)
{
	ComponentResult result=noErr;

	switch (param->what)
	{
		// four basic component functions (called by Apple's Component Manager)
		case kComponentOpenSelect:
			result=CALLCOMPONENT(OpenComponent);
			break;
        case kComponentCloseSelect:
            result=CALLCOMPONENTSTORAGE(CloseComponent);
            break;
        case kComponentCanDoSelect:
            result=CALLCOMPONENT(CanDo);
            break;                        
        case kComponentVersionSelect:
            result=CALLCOMPONENT(GetVersion);
            break;
            
        // functions called by Apple's Text Service Manager (TSM)
        case kCMGetScriptLangSupport:
            result=CALLCOMPONENTSTORAGE(GetScriptLangSupport);
            break;
        case kCMInitiateTextService:
            result=CALLCOMPONENTSTORAGE(InitiateTextService);
            break;
        case kCMTerminateTextService:
            result=CALLCOMPONENTSTORAGE(TerminateTextService);
            break;
        case kCMActivateTextService:
            result=CALLCOMPONENTSTORAGE(ActivateTextService);
            break;
        case kCMDeactivateTextService:
			result=CALLCOMPONENTSTORAGE(DeactivateTextService);
            break;
        case kCMTextServiceEvent:
            result=CALLCOMPONENTSTORAGE(TextServiceEventRef);
            break;
        case kCMGetTextServiceMenu:
            result=CALLCOMPONENTSTORAGE(GetTextServiceMenu);
            break;
        case kCMFixTextService:
            result=CALLCOMPONENTSTORAGE(FixTextService);
            break;
        case kCMHidePaletteWindows:
            result=CALLCOMPONENTSTORAGE(HidePaletteWindows);
            break;

        default:
            result=badComponentSelector;
            break;
    }

    return result;
}

// creates a new component instance
ACRT CIMOpenComponent(ComponentInstance inst)
{
    ComponentResult result=noErr;
    Handle hndl=nil;

    // if this is the first stance of CIM, initialize the global state:
    //     (1) load text service menu ("the pencil menu"),
    //     (2) install menu event handler
    //     (3) initialize global (inter-session) variables
    if (!cimInstanceRefcount) result=CIMInitialize(inst, &cimTextServiceMenu);
    cimInstanceRefcount++;

    if (result==noErr)
    {
        // get the session handle to store per-session data
        hndl=GetComponentInstanceStorage(inst);
                
        // initialize the new session
        result=CIMSessionOpen(inst, (CIMSessionHandle*)&hndl);

        // save the handle into the instance
        if (result==noErr) SetComponentInstanceStorage(inst, hndl);        
    }

    return result;
}


// close the instance; if it is the last instnace, terminate the global state
ACRT CIMCloseComponent(Handle hndl, ComponentInstance inst)
{
    ComponentResult result=noErr;

    if (inst==nil) result=paramErr;
    else
    {
        CIMSessionClose((CIMSessionHandle)hndl);
        SetComponentInstanceStorage(inst, nil);     // delete instance storage
                
        cimInstanceRefcount--;
        if (!cimInstanceRefcount) CIMTerminate(inst);
    }
    
    return result;
}

// returns true if what indicates in the selector is what we can do
ACRT CIMCanDo(SInt16 selector)
{
    Boolean result;

    switch(selector)
    {
        case kComponentOpenSelect:
        case kComponentCloseSelect:
        case kComponentCanDoSelect:
        case kComponentVersionSelect:
            result=true;
            break;

        // no need to support kCMTextServiceMenuSelect or kCMSetTextServiceCursor
        // (handled for us by OS X)
        case kCMGetScriptLangSupport:
        case kCMInitiateTextService:
        case kCMTerminateTextService:
        case kCMActivateTextService:
        case kCMDeactivateTextService:
        case kCMTextServiceEvent:
        case kCMGetTextServiceMenu:
        case kCMFixTextService:
        case kCMHidePaletteWindows:
            result=true;
            break;

        default:
            result=false;
            break;
    }
    
    return result;
}

ACRT CIMGetVersion()
{
    return cimVersion;      // see CIM.h for definition
}

// Apple's Text Service Manager calls this function to determine the
// type of our input method
ACRT CIMGetScriptLangSupport(Handle hndl, ScriptLanguageSupportHandle *outhndl)
{
    // #pragma unused (hndl)

    OSStatus result=noErr;
    ScriptLanguageRecord scriptrecord;
    
    if (*outhndl==NULL)     // if the value of handle is NULL, allocate space
    {
        *outhndl=(ScriptLanguageSupportHandle)NewHandle(sizeof(SInt16));
        if (*outhndl==NULL) return memFullErr;
    }

    if (result==noErr)
    {
        SetHandleSize((Handle)*outhndl, sizeof(SInt16));
        result=MemError();
        if (result==noErr) (**outhndl)->fScriptLanguageCount=0;
    }    

    // The comment below is cut-and-pasted from Apple's BIMComponent.c:
    //  Add a script/language record to tell TSM that we are a type 3 input method (we
    //  work with Unicode but only send back Unicode characters that are part of some
    //  Macintosh script encoding). We put our Macintosh script into our 'thng' resource
    //  component flags (as always), and return kTextEncodingUnicodeDefault from this
    //  routine.
    //
    //	Input   Component       ScriptLanguage-         xCHR Resource
    //	Method  Description     Support               ('KCHR' or 'uchr')
    //	Type    componentFlags  fScript              (chosen from KB Menu)
    //	------  --------------  -------------------  ---------------------
    //	type 1    Mac Script    Mac Script (or err)           KCHR
    //	type 2    Mac Script            0x7E             KCHR and uchr
    //	type 3    Mac Script           0x100                  uchr
    //	type 4       0x7E              0x100           uchr (negative ID)
    //	------  --------------  -------------------  ---------------------
    //

    if (result==noErr)
    {
        scriptrecord.fScript=kTextEncodingUnicodeDefault;
        scriptrecord.fLanguage=cimLanguage;
        result=PtrAndHand(&scriptrecord, (Handle)*outhndl,
            sizeof(ScriptLanguageRecord));
        if (result==noErr) (**outhndl)->fScriptLanguageCount++;
    }

    if (result)
    {
        if (*outhndl)
        {
            DisposeHandle((Handle)*outhndl);
            *outhndl=NULL;
        }
    }
    
    return result;        
}

ACRT CIMInitiateTextService(Handle hndl)
{
    // #pragma unused (hndl)
    return noErr;
}

ACRT CIMTerminateTextService(Handle hndl)
{
    // #pragma unused (hndl)
    return noErr;
}

ACRT CIMActivateTextService(Handle hndl)
{
    return CIMSessionActivate((CIMSessionHandle)hndl);
}

ACRT CIMDeactivateTextService(Handle hndl)
{
    return CIMSessionDeactivate((CIMSessionHandle)hndl);
}

// session event handler
ACRT CIMTextServiceEventRef(Handle hndl, EventRef evnt)
{
    return CIMSessionEvent((CIMSessionHandle)hndl, evnt);
}

// returns our "pencil" menu
ACRT CIMGetTextServiceMenu(Handle hndl, MenuHandle *mnu)
{
    *mnu=cimTextServiceMenu;
    return noErr;
}

// this is called when the input buffer must be "fixed"
ACRT CIMFixTextService(Handle hndl)
{
    return CIMSessionFix((CIMSessionHandle)hndl);
}

// called when all palettes must be hidden
ACRT CIMHidePaletteWindows(Handle hndl)
{
    return CIMSessionHidePalettes((CIMSessionHandle)hndl);
}


// glue code that creates a universal procedure pointer (upp) for the
// internal dispatch routines and calls them
ComponentResult CallCIMFunc(ComponentParameters *param, ProcPtr proc,
    SInt32 procinfo)
{
    ComponentResult result=noErr;
    ComponentFunctionUPP upp;

    upp=NewComponentFunctionUPP(proc, procinfo);
    
    result=CallComponentFunction(param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}

ComponentResult CallCIMFuncStorage(Handle hndl,
    ComponentParameters *param, ProcPtr proc, SInt32 procinfo)
{
    ComponentResult result=noErr;
    ComponentFunctionUPP upp;

    upp=NewComponentFunctionUPP(proc, procinfo);
    
    result=CallComponentFunctionWithStorage(hndl, param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}
