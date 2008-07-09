// TSBunble.mm: A dynamically-loadable text service bundle
//
// This implementation loads CocoaVanilla (OVLoader.bundle)
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

#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>
#include "TSBundle.h"
#include "IMInterface.h"

// #define DEBUG

// If we follow the ATSC protocol well, we can be 100% sure that 
// (1) our component will only be initialized once
// (2) when initialization fails, no other functions will ever be called
// (3) if TSBContextOpen fails, no other functions will ever be called

#ifdef DEBUG
    #define MURMUR(x)   fprintf(stderr, x); fprintf(stderr, "\n");
#else
    #define MURMUR(x)
#endif

// global data (active context and menu reference)
static TSBDataPtr tsbActiveContext=NULL;

@interface TSThreadFoo : NSObject
- (void)run;
@end;

@implementation TSThreadFoo
- (void)run {
    // NSLog(@"OpenVanilla gets Cocoa into multithreaded mode");
    [NSThread exit];
    // NSLog(@"thread ended");
}
@end;


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
	[[NSAutoreleasePool alloc] init];

    id ttf=[[[TSThreadFoo alloc] init] autorelease];
    [NSThread detachNewThreadSelector:@selector(run) toTarget:ttf withObject:nil];
    
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

    // we only take kEventRawKeyDown and kEventRawKeyRepat
    if (GetEventClass(evnt)!=kEventClassKeyboard) return FALSE;

    UInt32 eventkind=GetEventKind(evnt);
    if (eventkind!=kEventRawKeyDown && eventkind!=kEventRawKeyRepeat)
        return FALSE;

    // extract charcode (ASCII) and modifiers
    char charcode=0;
    UInt32 modifiers=0;    
    UInt32 kcode=0;

    GetEventParameter(evnt, kEventParamKeyCode, typeUInt32, nil,
        sizeof(kcode), nil, &kcode);
    GetEventParameter(evnt, kEventParamKeyMacCharCodes, typeChar, nil,
        sizeof(charcode), nil, &charcode);
    GetEventParameter(evnt, kEventParamKeyModifiers, typeUInt32, nil,
        sizeof(modifiers), nil, &modifiers);
    
    UInt32 numkeys[16]={    // keycode (not charcode) for numkeys
        // 0,1,2,3,4,5
        // 6,7,8,9,.,+,-,*,/,=
        0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5b, 0x5c, 0x41, 0x45, 0x4e, 0x43, 0x4b, 0x51
    };
    
    // there is some mysterious OS X numlock key issue, we need
    // this workaround to tackle it
    
    modifiers &= 0xfffeffff;    // first, we remove the numlock mask
    for (int i=0; i<16; i++) {
        if (kcode==numkeys[i]) {
            modifiers |= 0x10000;   // only if it's numback we put mask back
            break;
        }
    }

    // fprintf(stderr, "charcode=%x, modifiers=%x, keycode=%x\n", charcode, modifiers, kcode);

    IMContext *c=(IMContext*)ptr;
    c->sessionlock=1;
    int r=IMContextEvent((IMContext*)ptr, charcode, modifiers);
    c->sessionlock=0;    

    return r;
 
    // old code snippet from VanillaInput
    /*
        Boolean handled;
        UInt32 eventClass;
        UInt32 eventKind;
    
        handled = FALSE;
        eventClass = GetEventClass(inEventRef);
        eventKind = GetEventKind(inEventRef);
        if(eventClass==kEventClassKeyboard && 
           (eventKind == kEventRawKeyDown || eventKind == kEventRawKeyRepeat )) {
            UInt32 keyCode;
            unsigned char charCode;
            UInt32 modifiers;
            GetEventParameter( inEventRef, kEventParamKeyCode, typeUInt32, nil, sizeof (keyCode), nil, &keyCode );
            GetEventParameter( inEventRef, kEventParamKeyMacCharCodes, typeChar, nil, sizeof( charCode ), nil, &charCode );
            GetEventParameter( inEventRef, kEventParamKeyModifiers, typeUInt32, nil,
    sizeof( modifiers ), nil, &modifiers );
    
            fprintf(stderr, "charcode=%d, modifiers=%d, keyCode=%x\n", charCode, modifiers, keyCode);
    
            IMContext *c=(IMContext*)ptr;
            c->sessionlock=1;
            handled=IMContextEvent((IMContext*)ptr, charCode, modifiers);
            c->sessionlock=0;    
                               
        }
        return handled;
    */
   
}

extern "C" ComponentResult TSBMenuHandler(UInt32 cmd)
{
    MURMUR("TSBMenuHandler")
    if (!tsbActiveContext) return eventNotHandledErr;
    IMMenuHandler((IMContext*)tsbActiveContext, cmd);
    return noErr;
}
