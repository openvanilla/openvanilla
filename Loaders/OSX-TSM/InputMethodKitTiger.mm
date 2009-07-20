//
// InputMethodKitTiger.mm
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
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

#import "InputMethodKitTiger.h"
#import "Context.h"
#import <objc/objc-runtime.h>
#import "LVConfig.h"

@implementation IMKServer
+ (NSString*)generateUUID
{
    CFUUIDRef uuid = CFUUIDCreate(NULL);
    CFStringRef uuidStr = CFUUIDCreateString(NULL, uuid);
    CFRelease(uuid);
    [(NSString *)uuidStr autorelease];
    return (NSString *)uuidStr;
}    
- (void)dealloc
{
    [_serverConnection release];
    [_serverPort release];
    [_contextDictionary release];
    [super dealloc];
}
- (id)initWithName:(NSString*)connectionName bundleIdentifier:(NSString*)bundleIdentifier
{
    if (self = [super init]) {
        NSBundle *bundle = [NSBundle bundleWithIdentifier:bundleIdentifier];        
        NSString *controllerClassName = nil;
        
        if (bundle) {
            NSDictionary *infoDict = [bundle infoDictionary];            
            controllerClassName = [infoDict objectForKey:@"InputMethodServerControllerClass"];
        }
        else {
            NSLog(@"cannot find bundle");
            [self autorelease];
            return nil;
        }
        
		if (controllerClassName) {
		}
		else {
            NSLog(@"cannot find class name");
            [self autorelease];
            return nil;			
		}
		
        if (_controllerClass = objc_getClass([controllerClassName UTF8String])) {
        }
        else {
            NSLog(@"cannot find class named: %@", controllerClassName);
            [self autorelease];
            return nil;
        }
                
        _serverPort = [[NSPort port] retain];
        _serverConnection  = [[NSConnection connectionWithReceivePort:_serverPort sendPort:_serverPort] retain];
        _contextDictionary = [[NSMutableDictionary dictionary] retain];

        [_serverConnection setRootObject:self];

        if ([_serverConnection registerName:OPENVANILLA_CONNECTION_NAME]) {
        }
        else {
            NSLog(@"IMK-Tiger connection failed: %@", OPENVANILLA_CONNECTION_NAME);
            [self autorelease];
            return nil;
        }        
    }
    
    return self;
}
- (NSString*)newContextWithSender:(id)sender
{
    IMKInputController *context = [[[_controllerClass alloc] initWithServer:self delegate:nil client:sender] autorelease];    
    NSString *contextID = [IMKServer generateUUID];
    [_contextDictionary setObject:context forKey:contextID];
    return contextID;    
}
- (void)destroyContext:(NSString*)contextID sender:(id)sender;
{
    [_contextDictionary removeObjectForKey:contextID];
}
- (void)activateContextID:(NSString*)contextID sender:(id)sender
{
    [[_contextDictionary objectForKey:contextID] activateServer:sender];
    [self refreshMenuOfContext:contextID sender:sender];
}
- (void)deactivateContextID:(NSString*)contextID sender:(id)sender
{
    [[_contextDictionary objectForKey:contextID] deactivateServer:sender];
}
- (BOOL)handleEventForContext:(NSString*)contextID charString:(NSString*)charStr cocoaModifiers:(unsigned int)modifiers virtualKeyCode:(unsigned int)keyCode sender:(id)sender
{	
    NSEvent *fakeEvent = [NSEvent keyEventWithType:NSKeyDown
        location:NSMakePoint(0.0, 0.0)
        modifierFlags:modifiers
        timestamp:0.0
        windowNumber:0
        context:nil
        characters:charStr
        charactersIgnoringModifiers:nil
        isARepeat:NO
        keyCode:keyCode];
    return [[_contextDictionary objectForKey:contextID] handleEvent:fakeEvent client:sender];
}
- (void)forceFixContext:(NSString*)contextID sender:(id)sender
{
    [[_contextDictionary objectForKey:contextID] commitComposition:sender];
}
- (void)refreshMenuOfContext:(NSString*)contextID sender:(id)sender
{
    NSMenu *menu = [[_contextDictionary objectForKey:contextID] menu];
    [sender refreshMenu:[IMKInputController translateMenuItemArray:[menu itemArray]]];
}
- (void)triggerMenuOfContext:(NSString*)contextID itemIndexWithoutBase:(unsigned int)index sender:(id)sender
{
    NSMenu *menu = [[_contextDictionary objectForKey:contextID] menu];
    NSArray *itemArray = [menu itemArray];
    NSMenuItem *menuItem = [itemArray objectAtIndex:index];
    if (menuItem) {
        id target = [menuItem target];
        if (target) {
            SEL selector = [menuItem action];
            [target performSelector:selector withObject:[NSDictionary dictionaryWithObjectsAndKeys:menuItem, @"IMKCommandMenuItem", nil]];
        }
    }
    
    [self refreshMenuOfContext:contextID sender:sender];
}
@end


@implementation IMKInputController
- (id)initWithServer:(IMKServer*)server delegate:(id)aDelegate client:(id)aClient
{
    self = [super init];
    return self;
}
- (void)activateServer:(id)sender
{
}
- (void)deactivateServer:(id)sender
{
}
- (void)commitComposition:(id)sender
{
}
- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{
    return NO;
}
- (NSMenu*)menu
{
    return nil;
}
+ (NSArray*)translateMenuItemArray:(NSArray*)menuItemArray
{
    NSMutableArray *translatedArray = [NSMutableArray array];
    NSEnumerator *e = [menuItemArray objectEnumerator];
    NSMenuItem *item;
    
    while (item = [e nextObject]) {
		NSMutableDictionary *newItem = [NSMutableDictionary dictionaryWithDictionary:[NSDictionary dictionaryWithObjectsAndKeys:
										([item isSeparatorItem] ? @"-" : [item title]), @"Title",
										[NSNumber numberWithBool:[item isEnabled]], @"IsEnabled",
										[NSNumber numberWithInt:[item state]], @"State",
										nil]];
		
		if ([[item keyEquivalent] length]) {
			[newItem setObject:[item keyEquivalent] forKey:@"KeyEquivalent"];
			[newItem setObject:[NSNumber numberWithInt:[item keyEquivalentModifierMask]] forKey:@"KeyEquivalentModifierMask"];
		}
		
        [translatedArray addObject:newItem];
    }
    
    return translatedArray;
}
@end

//
// Free Tibet.
//
