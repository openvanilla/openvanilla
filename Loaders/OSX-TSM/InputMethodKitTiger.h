//
// InputMethodKitTiger.h
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

#import <Cocoa/Cocoa.h>

@interface NSObject (IMKTigerClient)
- (void)setMarkedText:(NSAttributedString*)text selectionRange:(NSRange)selRange replacementRange:(NSRange)rplRange;
- (void)overrideKeyboardWithKeyboardNamed:(NSString*)keyboardName;
- (void)insertText:(NSString*)text replacementRange:(NSRange)rplRange;
- (NSDictionary*)attributesForCharacterIndex:(int)index lineHeightRectangle:(NSRect*)returnRect;
- (NSString*)bundleIdentifier;
@end


@protocol IMKTigerServerProtocol
- (NSString*)newContextWithSender:(id)sender;
- (void)destroyContext:(NSString*)contextID sender:(id)sender;
- (void)activateContextID:(NSString*)contextID sender:(id)sender;
- (void)deactivateContextID:(NSString*)contextID sender:(id)sender;
- (BOOL)handleEventForContext:(NSString*)contextID charString:(NSString*)charStr cocoaModifiers:(unsigned int)modifiers virtualKeyCode:(unsigned int)keyCode sender:(id)sender;
- (void)forceFixContext:(NSString*)contextID sender:(id)sender;
- (void)refreshMenuOfContext:(NSString*)contextID sender:(id)sender;
- (void)triggerMenuOfContext:(NSString*)contextID itemIndexWithoutBase:(unsigned int)index sender:(id)sender;
@end


@interface IMKServer : NSObject <IMKTigerServerProtocol>
{
    NSPort *_serverPort;
    NSConnection *_serverConnection;
    NSMutableDictionary *_contextDictionary;
    id _controllerClass;
}
- (id)initWithName:(NSString*)connectionName bundleIdentifier:(NSString*)bundleIdentifier;
@end


@interface IMKInputController : NSObject
{
}
- (id)initWithServer:(IMKServer*)server delegate:(id)aDelegate client:(id)aClient;
- (void)activateServer:(id)sender;
- (void)deactivateServer:(id)sender;
- (void)commitComposition:(id)sender;
- (BOOL)handleEvent:(NSEvent*)event client:(id)sender;
- (NSMenu*)menu;

// IMKTiger-specific
+ (NSArray*)translateMenuItemArray:(NSArray*)menuItemArray;
@end

//
// Free Tibet.
//
