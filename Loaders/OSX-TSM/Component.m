//
// Component.m
//
// Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
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

#import <Carbon/Carbon.h>
#import "Component.h"
#import "ComponentConfig.h"
#import "Context.h"
#import "InputMethodKitTiger.h"

bool _ComponentInitialized = false;
id _InputMethodServer = nil;
Handle _ActiveHandle = NULL;
bool _KeyEventSessionOn = false;
id _ComponentAutoreleasePool = nil;

// creates a new session context
AppleComponent TSMCOpenComponent(ComponentInstance instance)
{
	if (!_ComponentAutoreleasePool) {
		NSApplicationLoad();
		_ComponentAutoreleasePool = [NSAutoreleasePool new];
		
		// see if it's some unsupported app..
		
		NSString *bid = [[NSBundle mainBundle] bundleIdentifier];
		
		if ([bid isEqualToString:@"com.atomix.dojam"]) {
			NSRunAlertPanel(@"Unsupported App", @"This app is too old for the input method to support", @"Dismiss", nil, nil);
			_ComponentInitialized = true;
			return -1;
		}
	}
	
    ComponentResult result = noErr;
    
    if (!_ComponentInitialized) {
        _ComponentInitialized = true;
        
        // get the menu
        short res = OpenComponentResFile((Component)instance);
        if (res == -1)
            return resNotFound;

        MenuRef menu = GetMenu(TSMC_PENCIL_MENU_ID);
        CloseComponentResFile(res);
        
        if (!menu)
            return resNotFound;

        [TextServiceContext setSharedMenu:menu];

        // set up the menu handler
        EventTypeSpec s;
        s.eventClass = kEventClassCommand;
        s.eventKind = kEventProcessCommand;
        if ((result = InstallMenuEventHandler(menu, NewEventHandlerUPP(TSMCMenuHandler), 1, &s, nil, nil)) != noErr)
            return result;

        // get the server and create a context ID
        _InputMethodServer = [[NSConnection rootProxyForConnectionWithRegisteredName:TSMC_SERVER_CONNECTION_NAME host:nil] retain];


        if (!_InputMethodServer) {
        	NSString *sharedSupportPath = [[NSBundle bundleWithIdentifier:[NSString stringWithUTF8String:TSMC_BUNDLE_ID]] sharedSupportPath];
        	NSString *serverPath = [sharedSupportPath stringByAppendingPathComponent:TSMC_LOADER_APP_NAME];
			
        	if (![[NSWorkspace sharedWorkspace] openFile:serverPath withApplication:nil andDeactivate:NO]) {
        		[[NSWorkspace sharedWorkspace] launchAppWithBundleIdentifier:TSMC_LOADER_SERVER_BUNDLE_ID options:NSWorkspaceLaunchWithoutActivation additionalEventParamDescriptor:nil launchIdentifier:nil];
        	}

    		// a total timeout of 10 sec
    		int timeout = 200;
			int retry;
    		for (retry = 0; retry < timeout; retry++) {
    			NSLog([NSString stringWithFormat:@"Waiting for connection to Loader, tick %d", retry]);
    			usleep(50000);		// wait for the server to be brought up (0.05 sec)
    			_InputMethodServer = [[NSConnection rootProxyForConnectionWithRegisteredName:TSMC_SERVER_CONNECTION_NAME host:nil] retain];
    			if (_InputMethodServer) break;
    		}
    	}
        
        if (!_InputMethodServer) {
            NSLog(@"Cannot connect to server via name: %@", TSMC_SERVER_CONNECTION_NAME);
            return -1;
        }
		else {
            [_InputMethodServer setProtocolForProxy:@protocol(IMKTigerServerProtocol)];
		}
    }
    
    TextServiceContext *context = [[TextServiceContext alloc] initWithComponentInstance:instance];
    
	NSString *contextID = [_InputMethodServer newContextWithSender:context];
	[context setContextID:contextID];

    // save the allocated data
    SetComponentInstanceStorage(instance, (Handle)context);     
    return noErr;
}

// close the given instance. since we don't do reference counting, we won't
// know if that's the last closed instance (henceforth meaning the end
// of the text service component--but in fact we won't know anyway, as
// the text service seems to never terminate under OS X, so we leave the
// clear up thing to the operating system
AppleComponent TSMCCloseComponent(Handle handle, ComponentInstance instance)
{
    if (!instance)
        return paramErr;

    if (handle) {
		@try {
			[_InputMethodServer destroyContext:[(TextServiceContext*)handle contextID] sender:(TextServiceContext*)handle];
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}
		
        [(TextServiceContext*)handle release];
    }
        
    // remove instance storage pointer
    SetComponentInstanceStorage(instance, nil);     
    return noErr;
}

AppleComponent TSMCActivateTextService(Handle handle)
{
	_ActiveHandle = handle;
	
	if (handle) {
		@try {
			[_InputMethodServer activateContextID:[(TextServiceContext*)handle contextID] sender:(TextServiceContext*)handle];
			[_InputMethodServer refreshMenuOfContext:[(TextServiceContext*)handle contextID] sender:(TextServiceContext*)handle];		
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}
	}
		
    return noErr;
}

AppleComponent TSMCDeactivateTextService(Handle handle)
{
    if (_ActiveHandle == handle)
        _ActiveHandle = NULL;
    
    if (handle) {
		@try {
			[_InputMethodServer deactivateContextID:[(TextServiceContext*)handle contextID] sender:(TextServiceContext*)handle];
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}		
	}
    return noErr;
}

AppleComponent TSMCTextServiceEvent(Handle handle, EventRef event)
{
    if (handle) {
        // we only take kEventRawKeyDown
        if (GetEventClass(event) != kEventClassKeyboard || (GetEventKind(event) != kEventRawKeyDown && GetEventKind(event) != kEventRawKeyRepeat))
            return FALSE;

        // extract charcode (ASCII) and modifiers
        UInt32 modifiers = 0;    
        UInt32 virtualKeyCode = 0;
        UniChar uchars[16];
        bzero(&uchars, sizeof(uchars));      
        unsigned long ucharLength = 0;

        GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, nil, sizeof(modifiers), nil, &modifiers);
        GetEventParameter(event, kEventParamKeyCode, typeUInt32, nil, sizeof(virtualKeyCode), nil, &virtualKeyCode);
        GetEventParameter(event, kEventParamKeyUnicodes, typeUnicodeText, nil, sizeof(uchars), &ucharLength, &uchars);
        NSString *charStr = [NSString stringWithCharacters:uchars length:ucharLength/sizeof(UniChar)];
		
		UInt32 cocoaModifiers = 0;
		if (modifiers & alphaLock)  cocoaModifiers |= NSAlphaShiftKeyMask;
		if (modifiers & shiftKey)   cocoaModifiers |= NSShiftKeyMask;
		if (modifiers & controlKey) cocoaModifiers |= NSControlKeyMask;
		if (modifiers & optionKey)  cocoaModifiers |= NSAlternateKeyMask;
		if (modifiers & cmdKey)     cocoaModifiers |= NSCommandKeyMask;

        _KeyEventSessionOn = true;
        BOOL result = false;
		
		@try {
			result = [_InputMethodServer handleEventForContext:[(TextServiceContext*)handle contextID]
				charString:(NSString*)charStr
				cocoaModifiers:cocoaModifiers
				virtualKeyCode:virtualKeyCode
				sender:(TextServiceContext*)handle];
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}
		
        _KeyEventSessionOn = false;
            
        return result;
    }
	
    return noErr;
}

AppleComponent TSMCFixTextService(Handle handle)
{
    if (handle && !_KeyEventSessionOn) {
		@try {
			[_InputMethodServer forceFixContext:[(TextServiceContext*)handle contextID] sender:(TextServiceContext*)handle];
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}		
	}
    return noErr;
}

pascal OSStatus TSMCMenuHandler(EventHandlerCallRef callRef, EventRef event, void *userdata)
{
    HICommand command;
 	if (GetEventParameter(event, kEventParamDirectObject, typeHICommand, nil, sizeof(command), nil, &command) != noErr)
 	    return eventNotHandledErr;

    if (_ActiveHandle) {
		@try {
			[_InputMethodServer triggerMenuOfContext:[(TextServiceContext*)_ActiveHandle contextID] 
				itemIndexWithoutBase:command.commandID - TSC_MENU_ITEM_INDEX_BASE
				sender:(TextServiceContext*)_ActiveHandle];
		}
		@catch (NSException *exception) {
			_InputMethodServer = nil;
		}		
	}
    
    return noErr;
}

AppleComponent TSMCGetTextServiceMenu(Handle handle, MenuHandle *mnu)
{
    *mnu = [TextServiceContext sharedMenu];
    return noErr;
}

AppleComponent TSMCGetVersion()
{
    return TSMC_VERSION;
}

// returns true if what indicates in the selector is what we can do
AppleComponent TSMCCanDo(SInt16 selector)
{
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
AppleComponent TSMCGetScriptLangSupport(Handle handle, ScriptLanguageSupportHandle *output)
{
    OSStatus result = noErr;
    ScriptLanguageRecord scriptrecord;
    
    // allocate space for handle if null
    if (!*output)
    {
        if (!(*output=(ScriptLanguageSupportHandle)NewHandle(sizeof(SInt16))))
            return memFullErr;
    }

    SetHandleSize((Handle)*output, sizeof(SInt16));
    if ((result = MemError()) != noErr)
        return result;
    
    (**output)->fScriptLanguageCount=0;
    
    scriptrecord.fScript = kTextEncodingUnicodeDefault;
    scriptrecord.fLanguage = TSMC_LANGUAGE;
    if (result = PtrAndHand(&scriptrecord, (Handle)*output, sizeof(ScriptLanguageRecord)))
    {
        if (*output)
        {
            DisposeHandle((Handle)*output);
            *output = NULL;
        }
        return result;
    }
        
    (**output)->fScriptLanguageCount++;    
    return noErr; 
}

// --------------------------------------------------------------------------
// entry point of the text service component
// --------------------------------------------------------------------------

// UPP entry points
#define UPPBASE kPascalStackBased | RESULT_SIZE(SIZE_CODE(sizeof(ComponentResult)))
#define UPPSTACK(n, t) STACK_ROUTINE_PARAMETER(n, SIZE_CODE(sizeof(t)))
enum {
    uppOpenComponent            = UPPBASE | UPPSTACK(1, ComponentInstance),
    uppCloseComponent           = UPPBASE | UPPSTACK(1, Handle) | UPPSTACK(2, ComponentInstance),
    uppCanDo                    = UPPBASE | UPPSTACK(1, short),
    uppGetVersion               = UPPBASE,
    uppGetScriptLangSupport     = UPPBASE | UPPSTACK(1, Handle) | UPPSTACK(2, ScriptLanguageSupportHandle*),
    uppInitiateTextService      = UPPBASE | UPPSTACK(1, Handle),
    uppTerminateTextService     = UPPBASE | UPPSTACK(1, Handle),
    uppActivateTextService      = UPPBASE | UPPSTACK(1, Handle),
    uppDeactivateTextService    = UPPBASE | UPPSTACK(1, Handle),
    uppTextServiceEvent         = UPPBASE | UPPSTACK(1, Handle) | UPPSTACK(2, EventRef),
    uppGetTextServiceMenu       = UPPBASE | UPPSTACK(1, Handle) | UPPSTACK(2, MenuHandle*),
    uppFixTextService           = UPPBASE | UPPSTACK(1, Handle),
    uppHidePaletteWindows       = UPPBASE | UPPSTACK(1, Handle)
};

// service functions
ComponentResult CallTSMCFunc(ComponentParameters *param, ProcPtr proc, SInt32 procinfo);
ComponentResult CallTSMCFuncStorage(Handle handle, ComponentParameters *param, ProcPtr proc, SInt32 procinfo);

#define CALLCOMPONENT(x)            CallTSMCFunc(param, (ProcPtr)TSMC##x, upp##x)
#define CALLCOMPONENTSTORAGE(x)     CallTSMCFuncStorage(handle, param, (ProcPtr)TSMC##x, upp##x)

// the main entry point of a text service component is simply a dispatcher
AppleComponent TSMCDispatch(ComponentParameters *param, Handle handle)
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
        case kComponentCanDoSelect:
            return CALLCOMPONENT(CanDo);
        case kComponentVersionSelect:
            return CALLCOMPONENT(GetVersion);
        case kCMGetScriptLangSupport:
            return CALLCOMPONENTSTORAGE(GetScriptLangSupport);
        case kCMGetTextServiceMenu:
            return CALLCOMPONENTSTORAGE(GetTextServiceMenu);
        case kCMInitiateTextService:
            return noErr;
        case kCMTerminateTextService:
            return noErr;
        case kCMHidePaletteWindows:
            return noErr;
        default:
            return badComponentSelector;
    }
}

// glue code that creates a universal procedure pointer (upp) for the
// internal dispatch routines and calls them
ComponentResult CallTSMCFunc(ComponentParameters *param, ProcPtr proc, SInt32 procinfo)
{
    ComponentFunctionUPP upp = NewComponentFunctionUPP(proc, procinfo); 
    ComponentResult result = CallComponentFunction(param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}

ComponentResult CallTSMCFuncStorage(Handle handle, ComponentParameters *param, ProcPtr proc, SInt32 procinfo)
{
    ComponentFunctionUPP upp = NewComponentFunctionUPP(proc, procinfo);
    ComponentResult result = CallComponentFunctionWithStorage(handle, param, upp);
    DisposeComponentFunctionUPP(upp);
    return result;
}

//
// Free Tibet
//
