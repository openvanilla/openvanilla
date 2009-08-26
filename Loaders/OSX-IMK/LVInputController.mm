//
// LVInputController.mm
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

#import "LVInputController.h"
#import "LVModuleManager.h"
#import "LVUIController.h"

@interface NSObject (LVInputControllerPrivateInterface)
- (bool)_handleExtraKeyAction:(OVKeyCode*)k;
@end

static LVInputController *LVICCurrentlyActiveController = nil;
static id LVICCurrentlyActiveSender = nil;

@implementation LVInputController
- (id)initWithServer:(IMKServer*)server delegate:(id)delegate client:(id)inputClient
{
	if (self = [super initWithServer:server delegate:delegate client:inputClient]) {
		_contextSandwich = [[LVModuleManager sharedManager] createContextSandwich];
		_candidateText = new LVCandidate;
		_composingBuffer = new LVBuffer;
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_handleModuleConfigChanged:) name:LVModuleConfigChangedNotification object:nil];
	}

	return self;
}
- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	delete _contextSandwich;
	delete _candidateText;
	delete _composingBuffer;
    [super dealloc];
}

+ (void)setActiveContext:(LVInputController *)context sender:(id)sender
{
    LVICCurrentlyActiveController = context;	
	id tmp = LVICCurrentlyActiveSender;	
    LVICCurrentlyActiveSender = [sender retain];
	[tmp release];
}

- (void)_resetUI
{
	_candidateText->clear();
	_candidateText->clearUpdateState();
	_candidateText->hide();
	_composingBuffer->clear();
	_composingBuffer->clearUpdateState();
	_composingBuffer->clearCommittedString();	
	[[LVModuleManager sharedManager] loaderService]->notify("");
	
	LVUIController *uiController = (LVUIController *)[NSApp delegate];
	[uiController hideCandidateWindow];
	[uiController hideTooltip];
}
- (void)sendComposedStringToClient:(NSString *)text sender:(id)sender
{	
	_composingBuffer->clear();
	_composingBuffer->clearCommittedString();
	_composingBuffer->append([text UTF8String]);
	_composingBuffer->send();
	
 	// force commit
	_contextSandwich->clear();
	_committedByOurselves = YES;
	[self commitComposition:sender];

	// set again, because this flag is reset by commitComposition:
	_committedByOurselves = YES;
	[self _resetUI];	
	_committedByOurselves = NO;
	
	[[LVModuleManager sharedManager] loaderService]->cleartNotifyMessage();
}

+ (void)sendComposedStringToCurrentlyActiveContext:(NSString *)text
{
    if (LVICCurrentlyActiveController && LVICCurrentlyActiveSender && [text length]) {
//		NSLog(@"Sending direct text: %@", text);
        [LVICCurrentlyActiveController sendComposedStringToClient:text sender:LVICCurrentlyActiveSender];
    }
    else {
    }
}

- (void)_recreateSandwich
{
	[self _resetUI];
	if (_contextSandwich) {
		delete _contextSandwich;
	}
	
	_contextSandwich = [[LVModuleManager sharedManager] createContextSandwich];
	_contextSandwich->start(_composingBuffer, _candidateText, [[LVModuleManager sharedManager] loaderService]);	
}

- (void)_handleModuleConfigChanged:(NSNotification *)notification
{
	[self _recreateSandwich];
}

- (void)_updateComposingBuffer:(id)client cursorAtIndex:(NSUInteger)cursorIndex highlightRange:(NSRange)range
{
	// we first need to map the cursor and highlight range (which are codepoint-based) to UniChar-based indexes
	vector<string> codepoints = OVUTF8Helper::SplitStringByCodePoint(_composingBuffer->composingBuffer());	
	vector<size_t> unicharIndex;	
	size_t accuIndex = 0;
	
	unicharIndex.push_back(0);
	for (vector<string>::const_iterator citer = codepoints.begin() ; citer != codepoints.end() ; ++citer) {
		accuIndex += OVUTF16::FromUTF8(*citer).length();
		unicharIndex.push_back(accuIndex);
	}
	
	if (cursorIndex <= unicharIndex.size()) {
		cursorIndex = unicharIndex[cursorIndex];
	}
	
	if (range.location != NSNotFound) {
		if (range.length != NSNotFound) { 
			NSInteger to = range.location + range.length;
			if (to <= unicharIndex.size()) {
				range.length = unicharIndex[to] - unicharIndex[range.location];
			}
		}
		
		if (range.location <= unicharIndex.size()) {
			range.location = unicharIndex[range.location];
		}
	}

	
	NSString *composingBuffer = [NSString stringWithUTF8String:_composingBuffer->composingBuffer().c_str()];
	
	NSMutableAttributedString *attrString = [[[NSMutableAttributedString alloc] initWithString:composingBuffer attributes:[NSDictionary dictionary]] autorelease];    

    NSRange markerRange[3];
    NSInteger markerStyle[3];
    NSInteger markerCount = 0;

	if (range.location != NSNotFound) {
	    if (range.location) {
            markerRange[markerCount] = NSMakeRange(0, range.location);
            markerStyle[markerCount] = NSUnderlineStyleSingle;
            markerCount++;
	    }
	    
	    if (range.length) {
            markerRange[markerCount] = range;
            markerStyle[markerCount] = NSUnderlineStyleThick;
            markerCount++;
	    }
	    
        vector<string> codepoints = OVUTF8Helper::SplitStringByCodePoint(_composingBuffer->composingBuffer());
		
	    if (codepoints.size() > range.location + range.length) {
            markerRange[markerCount] = NSMakeRange(range.location + range.length, [composingBuffer length] - range.location + range.length);
            markerStyle[markerCount] = NSUnderlineStyleSingle;
            markerCount++;
	    }
	}
	else {
        markerRange[markerCount] = NSMakeRange(0, [composingBuffer length]);
        markerStyle[markerCount] = NSUnderlineStyleSingle;
        markerCount++;
	}
	
    for (NSInteger i = 0 ; i < markerCount ; i++) {
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
        NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInt:markerStyle[i]], NSUnderlineStyleAttributeName,
								  [NSNumber numberWithInt:i], NSMarkedClauseSegmentAttributeName, nil];
#else
        NSDictionary *attrDict = [NSDictionary dictionaryWithObjectsAndKeys:
                                  [NSNumber numberWithInt:markerStyle[i]], @"UnderlineStyleAttribute",
								  [NSNumber numberWithInt:i], @"MarkedClauseSegmentAttribute", nil];
#endif		
        [attrString setAttributes:attrDict range:markerRange[i]];  
    }

    // selectionRange means "cursor position"
	NSRange selectionRange = NSMakeRange(cursorIndex, 0);	
	[client setMarkedText:attrString selectionRange:selectionRange replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
}
// To fix the curson position in some applications such as WOW
- (NSPoint)_fixCaretPosition:(NSPoint)caretPosition
{
	NSRect frame = [[NSScreen mainScreen] frame];
	BOOL hasFocus = NO;
	
	NSArray *screens = [NSScreen screens];
	NSEnumerator *enumerator = [screens objectEnumerator];
	NSScreen *screen;
	while (screen = [enumerator nextObject]) {
		NSRect screenFrame = [screen frame];
		
		if (!hasFocus && caretPosition.x >= NSMinX(screenFrame) && caretPosition.x <= NSMaxX(screenFrame)) {
			frame = [screen visibleFrame];
			hasFocus = YES;
			break;
		}
	}

	if (hasFocus) {
		// TO DO: Fix caret position according to the current window dimension
		return caretPosition;
	}
	
	NSPoint point;		
	point.x = (int)(frame.size.width / 2.0);
	point.y = (int)(frame.size.height / 2.0);
	return point;
}
- (void)_handleCandidateWindow:(id)sender
{
    // update cursor position
    NSPoint caretPosition;
    NSRect lineHeightRect;
	[sender attributesForCharacterIndex:0 lineHeightRectangle:&lineHeightRect];
	
	LVUIController *uiController = (LVUIController *)[NSApp delegate];
	
	if (_candidateText->shouldUpdate()) {
		NSString *text = [NSString stringWithUTF8String:_candidateText->candidateText().c_str()];		
		[uiController updateCandidateText:text];
		
		// TO DO: set font metrics
		caretPosition = [self _fixCaretPosition:lineHeightRect.origin];			
		[uiController setCandidateWindowOrigin:caretPosition];
		_candidateText->clearUpdateState();
	}
	
    if (_candidateText->onScreen()) 
		[uiController showCandidateWindow];
	else
		[uiController hideCandidateWindow];
}
- (void)_handleNotifyMessage:(id)sender
{
	LVService* service = [[LVModuleManager sharedManager] loaderService];
	NSString* notifyMsg = [NSString stringWithUTF8String:service->notifyMessage().c_str()];
	
	LVUIController *uiController = (LVUIController *)[NSApp delegate];
	
	if ([notifyMsg length]) {
		// show notify message at current caret position, adjust position if there's already candidate window
		NSPoint location;
		NSRect lineHeightRect;
		[sender attributesForCharacterIndex:0 lineHeightRectangle:&lineHeightRect];
		location = lineHeightRect.origin;
		
		// TO DO: Handle tooltip better
		[uiController showTooltipWithText:notifyMsg];		
		service->cleartNotifyMessage();
	}
	else {
		[uiController hideTooltip];
		// hide notify message tooltip if there's already one, otherwise do nothing
	}
}
- (unsigned int)recognizedEvents:(id)sender
{
    return NSKeyDownMask | NSKeyUpMask | NSFlagsChangedMask | NSMouseEnteredMask | NSLeftMouseDownMask | NSLeftMouseDragged;
}
- (NSString *)currentKeyboardLayout
{
	return @"com.apple.keylayout.US";
}
- (void)activateServer:(id)sender
{
	[self _resetUI];
	[[LVModuleManager sharedManager] syncConfiguration];
	_contextSandwich->start(_composingBuffer, _candidateText, [[LVModuleManager sharedManager] loaderService]);
	[[self class] setActiveContext:self sender:sender];	
}
- (void)deactivateServer:(id)sender
{
	if (LVICCurrentlyActiveController == self && LVICCurrentlyActiveSender == sender) {
		[[self class] setActiveContext:nil sender:nil];
	}
		
    _committedByOurselves = NO;
	string committedString = _composingBuffer->committedString();
	NSLog(@"%s, str: %s, commited by ourselves? %d", __PRETTY_FUNCTION__, committedString.c_str(), _committedByOurselves);
    
	[self commitComposition:sender];
	_composingBuffer->clear();

	_contextSandwich->end();
	[self _resetUI];	
}
- (void)commitComposition:(id)sender 
{    	
	string committedString;
	
	if (_committedByOurselves) {
	    _committedByOurselves = NO;
		committedString = _composingBuffer->committedString();
    }
    else {
		_composingBuffer->send();
		committedString = _composingBuffer->committedString();
		
        _contextSandwich->clear();
		_composingBuffer->clear();
        [self _resetUI];
    }
	
	committedString = [[LVModuleManager sharedManager] processStringWithOutputFilters:committedString];
	
	if (committedString.length()) {
		// NSLog(@"updating buffer");
		NSString *committedText = [NSString stringWithUTF8String:committedString.c_str()];

		// we can't use the following line because this will create an NSAttributeString with underline attributes;
		// to workaround the bug in PowerPoint 2008, we need to give a "no-attr" NSAttributeString
		// [self _updateComposingBuffer:sender cursorAtIndex:_composingBuffer->cursorIndex() highlightRange:NSMakeRange(NSNotFound, NSNotFound)];
		NSMutableAttributedString *attrString = [[[NSMutableAttributedString alloc] initWithString:committedText attributes:[NSDictionary dictionary]] autorelease];    
		[sender setMarkedText:attrString selectionRange:NSMakeRange([committedText length], 0) replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
		
		
		[sender insertText:committedText replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
		_composingBuffer->clearCommittedString();	
	}
}
- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{
    BOOL handled = NO;
    
    if ([event type] == NSFlagsChanged) {
        // handles caps lock and shift here
    }
    else if ([event type] == NSKeyDown) {
		NSString *chars = [event characters];
		unsigned int cocoaModifiers = [event modifierFlags];
		unsigned short virtualKeyCode = [event keyCode];		
		
		LVKeyCode keyCode;		
        if (cocoaModifiers & NSAlphaShiftKeyMask) keyCode.capsLock = true;
        if (cocoaModifiers & NSControlKeyMask) keyCode.ctrl = true;        
        if (cocoaModifiers & NSAlternateKeyMask) keyCode.opt = true;
        if (cocoaModifiers & NSCommandKeyMask) keyCode.command = true;
		if (cocoaModifiers & NSShiftKeyMask) keyCode.shift = true;

		UInt32 numKeys[16] = {    
			0x52, 0x53, 0x54, 0x55, 0x56, 0x57, // 0,1,2,3,4,5
			0x58, 0x59, 0x5b, 0x5c, 0x41, 0x45, 0x4e, 0x43, 0x4b, 0x51 // 6,7,8,9,.,+,-,*,/,=
		};
		
		for (size_t i = 0; i < 16; i++) {
			if (virtualKeyCode == numKeys[i]) {
                keyCode.num = true;
				break;
			}
		}
		
		UniChar unicharCode = 0;
		if ([chars length] > 0) {
			unicharCode = [chars characterAtIndex:0];
            
			// translates CTRL-[A-Z] to the correct PVKeyImpl
			if (cocoaModifiers & NSControlKeyMask) {
				if (unicharCode < 27) {
					unicharCode += ('a' - 1);
				}
				else {
					switch (unicharCode) {
						case 27:
							unicharCode = (cocoaModifiers & NSShiftKeyMask) ? '{' : '[';
							break;
						case 28:
							unicharCode = (cocoaModifiers & NSShiftKeyMask) ? '|' : '\\';
							break;
						case 29:
							unicharCode = (cocoaModifiers & NSShiftKeyMask) ? '}': ']';
							break;
						case 31:
							unicharCode = (cocoaModifiers & NSShiftKeyMask) ? '_' : '-';
							break;							
					}
				}
			}
			
            // remap; fix 10.6 "bug"
            switch(unicharCode) {
                case NSUpArrowFunctionKey:      unicharCode = ovkUp; break;
                case NSDownArrowFunctionKey:    unicharCode = ovkDown; break;
                case NSLeftArrowFunctionKey:    unicharCode = ovkLeft; break;
                case NSRightArrowFunctionKey:   unicharCode = ovkRight; break;
                case NSDeleteFunctionKey:       unicharCode = ovkDelete; break;
                case NSHomeFunctionKey:         unicharCode = ovkHome; break;
                case NSEndFunctionKey:          unicharCode = ovkEnd; break;
                case NSPageUpFunctionKey:       unicharCode = ovkPageUp; break;
                case NSPageDownFunctionKey:     unicharCode = ovkPageDown; break;           
                case NSF1FunctionKey:           unicharCode = 0x11001; break;   // Carbon F1-F10
                case NSF2FunctionKey:           unicharCode = 0x11002; break;
                case NSF3FunctionKey:           unicharCode = 0x11003; break;
                case NSF4FunctionKey:           unicharCode = 0x11004; break;
                case NSF5FunctionKey:           unicharCode = 0x11005; break;
                case NSF6FunctionKey:           unicharCode = 0x11006; break;
                case NSF7FunctionKey:           unicharCode = 0x11007; break;
                case NSF8FunctionKey:           unicharCode = 0x11008; break;
                case NSF9FunctionKey:           unicharCode = 0x11009; break;
                case NSF10FunctionKey:          unicharCode = 0x11010; break;
					
            }
            
			keyCode.keyCode = unicharCode;
			
            if (unicharCode < 128) {
				if ([self respondsToSelector:@selector(_handleExtraKeyAction:)]) {
					handled = [self _handleExtraKeyAction:&keyCode];
				}
				if (!handled) {
					handled = _contextSandwich->keyEvent(&keyCode, _composingBuffer, _candidateText, [[LVModuleManager sharedManager] loaderService]);
				}
            }
            else {
            }
        }
        else {
        }
    }
	
	if (_composingBuffer->shouldCommit()) {
        _committedByOurselves = YES;
		[self commitComposition:sender];
        
		_composingBuffer->clearCommittedString();
	}
	
	if (_composingBuffer->shouldUpdate()) {
		NSRange highlightRange = NSMakeRange(NSNotFound, NSNotFound);
		int hf = _composingBuffer->highlightFrom(), ht = _composingBuffer->highlightTo();
		if (hf >= 0 && ht >= 0 && (ht - hf) >= 0) {
			highlightRange = NSMakeRange(hf, ht - hf);
		}
		
		[self _updateComposingBuffer:sender cursorAtIndex:_composingBuffer->cursorIndex() highlightRange:highlightRange];
	}

	[self _handleCandidateWindow:sender];
	[self _handleNotifyMessage:sender];
	
    return handled;
}
- (void)_switchInputMethodAction:(id)sender
{
    NSMenuItem *menuItem = [sender objectForKey:@"IMKCommandMenuItem"];
	[[LVModuleManager sharedManager] setPrimaryInputMethodModuleID:[menuItem representedObject]];
	[self _recreateSandwich];
}

- (void)_toggleOutputFilterAction:(id)sender
{
    NSMenuItem *menuItem = [sender objectForKey:@"IMKCommandMenuItem"];
	[[LVModuleManager sharedManager] toggleOutputFilterModuleID:[menuItem representedObject]];
	[self _recreateSandwich];	
}

- (void)_openPreferencesAction:(id)sender
{
	NSString *prefAppPath = [[[NSBundle bundleForClass:[self class]] sharedSupportPath] stringByAppendingPathComponent:@"Preferences.app"];
	NSLog(@"launching: %@", prefAppPath);
	[[NSWorkspace sharedWorkspace] openFile:prefAppPath];
}

- (NSMenuItem *)_createInputMethodMenuItemWithIndentifer:(NSString *)identifier localizedName:(NSString *)localizedName
{
	NSMenuItem *menuItem = [[[NSMenuItem alloc] init] autorelease];
	if (![[[LVModuleManager sharedManager] moduleForIdentifier:identifier] isUsable]) {
		[menuItem setEnabled:NO];
	}
	else {        
		if ([[[LVModuleManager sharedManager] primaryInputMethodModuleID] isEqualTo:identifier]) {
			[menuItem setState:NSOnState];
		}
		
		[menuItem setTarget:self];
		[menuItem setAction:@selector(_switchInputMethodAction:)];
		[menuItem setRepresentedObject:identifier];
	}
	[menuItem setTitle:localizedName];
	return menuItem;
}

- (NSMenuItem *)_createOutputFilterMenuItemWithIndentifer:(NSString *)identifier localizedName:(NSString *)localizedName
{
	NSMenuItem *menuItem = [[[NSMenuItem alloc] init] autorelease];
	if (![[[LVModuleManager sharedManager] moduleForIdentifier:identifier] isUsable]) {
		[menuItem setEnabled:NO];
	}
	else {        
		if ([[LVModuleManager sharedManager] isOutputFilterActivated:identifier]) {
			[menuItem setState:NSOnState];
		}
		
		[menuItem setTarget:self];
		[menuItem setAction:@selector(_toggleOutputFilterAction:)];
		[menuItem setRepresentedObject:identifier];
	}
	[menuItem setTitle:localizedName];
	return menuItem;
}

- (NSMenu *)menu
{
    NSMenu *menu = [[[NSMenu alloc] init] autorelease];
	
	NSArray *namesAndIDs = [[LVModuleManager sharedManager] inputMethodTitlesAndModuleIDs];
	NSEnumerator *arrayEnum = [namesAndIDs objectEnumerator];
	NSArray *nameIDPair;
	while (nameIDPair = [arrayEnum nextObject]) {
		[menu addItem:[self _createInputMethodMenuItemWithIndentifer:[nameIDPair objectAtIndex:1] localizedName:[nameIDPair objectAtIndex:0]]];
	}

	[menu addItem:[NSMenuItem separatorItem]];

	namesAndIDs = [[LVModuleManager sharedManager] outputFilterTitlesAndModuleIDs];
	arrayEnum = [namesAndIDs objectEnumerator];
	while (nameIDPair = [arrayEnum nextObject]) {
		[menu addItem:[self _createOutputFilterMenuItemWithIndentifer:[nameIDPair objectAtIndex:1] localizedName:[nameIDPair objectAtIndex:0]]];
	}	
	
	[menu addItem:[NSMenuItem separatorItem]];
	
	NSMenuItem *menuItem = nil;
	menuItem = [[[NSMenuItem alloc] init] autorelease];
	[menuItem setEnabled:YES];
	[menuItem setTarget:self];
	[menuItem setAction:@selector(_openPreferencesAction:)];
	[menuItem setTitle:@"Preferences…"];
	[menu addItem:menuItem];
	
	
    return menu;
}
@end
