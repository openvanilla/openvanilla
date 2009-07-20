//
// Context.m
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

#import <Carbon/Carbon.h>
#import <Quartz/Quartz.h>
#import "ComponentConfig.h"
#import "Context.h"

static MenuRef TSCSharedMenu = NULL;

@implementation TextServiceContext
+ (void)setSharedMenu:(MenuRef)menu
{
    TSCSharedMenu = menu;
}
+ (MenuRef)sharedMenu
{
    return TSCSharedMenu;
}
- (void)dealloc
{
    [_contextID release];
    [super dealloc];
}
- (id)initWithComponentInstance:(ComponentInstance)instance
{
    if (self = [super init]) {
        _componentInstance = instance;
		_lastUpdateLength = 0;
        _contextID = nil;
    }
    return self;
}
- (NSString*)contextID
{
    return [NSString stringWithString:_contextID];
}
- (void)setContextID:(NSString*)contextID
{
    NSString *tmp = _contextID;
    _contextID = [contextID copy];
    [tmp release];
}
- (void)setMarkedText:(NSAttributedString*)text selectionRange:(NSRange)selRange replacementRange:(NSRange)rplRange
{
    NSRange maxRange = NSMakeRange(0, [text length]);
    NSRange currentRange = NSMakeRange(0, 0);

    NSMutableArray *highlightSegments = [NSMutableArray array];

    if ([text length]) {
        do {
			NSDictionary *attributes = [text attributesAtIndex:currentRange.location longestEffectiveRange:&currentRange inRange:maxRange];       

//			NSLog(@"section range: %@, attr: %@", NSStringFromRange(currentRange), attributes);
			id usa = [attributes objectForKey:@"UnderlineStyleAttribute"];
			if (!usa) {
				usa = [NSNumber numberWithInt:NSUnderlineStyleSingle];
			}
			[highlightSegments addObject:[NSArray arrayWithObjects:[NSValue valueWithRange:currentRange], usa, nil]];       
			currentRange.location += currentRange.length;              
        } while (currentRange.location < maxRange.length);
    }
    
//	NSLog(@"now highlight seg count: %d, text %@, selRange: %@", [highlightSegments count], [text string], NSStringFromRange(selRange));
	
//	if (![highlightSegments count]) {
//		NSLog(@"using no segments");
//		[highlightSegments addObject:[NSArray arrayWithObjects:[NSValue valueWithRange:maxRange], [NSNumber numberWithInt:NSUnderlineStyleSingle], nil]];       
//	}
	
	
	
    OSStatus status;
    long fixLength = 0;
    long updateLength = [text length] * sizeof(UniChar);
    EventRef event;
    ScriptLanguageRecord slRecord;    
    slRecord.fScript = TSMC_SCRIPT;
    slRecord.fLanguage = TSMC_LANGUAGE;

    UniChar *uniCharBuffer = 0;
    uniCharBuffer = (UniChar*)calloc(1, updateLength);
    [[text string] getCharacters:uniCharBuffer];

    TextRangeArrayPtr updateArray = NULL;
    TextRangeArrayPtr highlightArray = NULL;

    status = CreateEvent(NULL, kEventClassTextInput, kEventTextInputUpdateActiveInputArea, GetCurrentEventTime(), kEventAttributeUserEvent, &event);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendComponentInstance, typeComponentInstance, sizeof(ComponentInstance), &_componentInstance);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendSLRec, typeIntlWritingCode, sizeof(ScriptLanguageRecord), &slRecord);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendText, typeUnicodeText, updateLength, uniCharBuffer);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendFixLen, typeLongInteger, sizeof(long), &fixLength);
    if (status == noErr) {
        long arraySize = sizeof(short) + sizeof(TextRange) * 2;
        updateArray = (TextRangeArrayPtr)calloc(1, arraySize);
        if (updateArray) {
            updateArray->fNumOfRanges=2;
            updateArray->fRange[0].fStart = 0;
            updateArray->fRange[0].fEnd = _lastUpdateLength;
            updateArray->fRange[0].fHiliteStyle = 0;
            updateArray->fRange[1].fStart = 0;
            updateArray->fRange[1].fEnd = updateLength;
            updateArray->fRange[1].fHiliteStyle = 0;
			
			_lastUpdateLength = updateLength;
        }
        
		// cf. CarbonEvents.h for the explanation for the two update regions
        status = SetEventParameter(event, kEventParamTextInputSendUpdateRng, typeTextRangeArray, arraySize, updateArray);
    }
    if (status == noErr) {
        long arraySize = sizeof(short) + sizeof(TextRange) * (1 + [highlightSegments count]);
        highlightArray = (TextRangeArrayPtr)calloc(1, arraySize);
        
        if (highlightArray) {
            highlightArray->fNumOfRanges = 1 + [highlightSegments count];
            
            highlightArray->fRange[0].fStart = selRange.location * sizeof(UniChar);
            highlightArray->fRange[0].fEnd = selRange.location * sizeof(UniChar);
            highlightArray->fRange[0].fHiliteStyle = kTSMHiliteCaretPosition;
            
            int i;
            for (i = 0 ; i < [highlightSegments count] ; i++) {
                NSRange range = [[[highlightSegments objectAtIndex:i] objectAtIndex:0] rangeValue];
                int style = [[[highlightSegments objectAtIndex:i] objectAtIndex:1] intValue];
                
                highlightArray->fRange[1 + i].fStart = range.location * sizeof(UniChar);
                highlightArray->fRange[1 + i].fEnd = (range.location + range.length) * sizeof(UniChar);
                
                if (style == NSUnderlineStyleThick)
                    highlightArray->fRange[1 + i].fHiliteStyle = kTSMHiliteSelectedConvertedText;
                else
                    highlightArray->fRange[1 + i].fHiliteStyle = kTSMHiliteConvertedText;
            }

            SetEventParameter(event, kEventParamTextInputSendHiliteRng, typeTextRangeArray, arraySize, highlightArray);
        }
    }

    if (status == noErr)
        SendTextInputEvent(event);
    
    if (highlightArray)
        free(highlightArray);
    if (updateArray)
        free(updateArray);

    free(uniCharBuffer);
}
- (void)overrideKeyboardWithKeyboardNamed:(NSString*)keyboardName
{
    // to be implemented
}
- (void)insertText:(NSString*)text replacementRange:(NSRange)rplRange
{
    if (![text length])
        return;
    
    OSStatus status;
    long fixLength = [text length] * sizeof(UniChar);
    EventRef event;
    ScriptLanguageRecord slRecord;    
    slRecord.fScript = TSMC_SCRIPT;
    slRecord.fLanguage = TSMC_LANGUAGE;

    UniChar *uniCharBuffer = 0;
    uniCharBuffer = (UniChar*)calloc(1, fixLength);
    [text getCharacters:uniCharBuffer];

    status = CreateEvent(NULL, kEventClassTextInput, kEventTextInputUpdateActiveInputArea, GetCurrentEventTime(), kEventAttributeUserEvent, &event);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendComponentInstance, typeComponentInstance, sizeof(ComponentInstance), &_componentInstance);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendSLRec, typeIntlWritingCode, sizeof(ScriptLanguageRecord), &slRecord);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendText, typeUnicodeText, fixLength, uniCharBuffer);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendFixLen, typeLongInteger, sizeof(long), &fixLength);
    SendTextInputEvent(event);
    free(uniCharBuffer);
	
	_lastUpdateLength = 0;
}
- (NSDictionary*)attributesForCharacterIndex:(int)index lineHeightRectangle:(NSRect*)returnRect
{
    EventRef event = NULL;
    OSStatus status;
    
    status = CreateEvent(NULL, kEventClassTextInput, kEventTextInputOffsetToPos, GetCurrentEventTime(),kEventAttributeUserEvent, &event);

    // the reply data
    Point replyPoint;
    long refCount = 0;
	// I do not know the exact meaning of text offset, however, the position looks good when we double the value.
    long textOffset = (long)index * 2;
    // long textOffset = (long)index;
    Boolean leadingEdge = true;
    ScriptLanguageRecord slRecord;
    float fontSize = 0.0;
    
    bzero(&slRecord, sizeof(slRecord));
    replyPoint.h = replyPoint.v = 0;
    
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendComponentInstance, typeComponentInstance, sizeof(ComponentInstance), &_componentInstance);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendRefCon, typeLongInteger, sizeof(long), &refCount);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendTextOffset, typeLongInteger, sizeof(long), &textOffset);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendSLRec, typeIntlWritingCode, sizeof(ScriptLanguageRecord), &slRecord);
    if (status == noErr)
        status = SetEventParameter(event, kEventParamTextInputSendLeadingEdge, typeBoolean, sizeof(Boolean), &leadingEdge);        
    if (status == noErr)
        status = SendTextInputEvent(event);
        
    if (status == noErr)
        status = GetEventParameter(event, kEventParamTextInputReplyPoint, typeQDPoint, NULL, sizeof(Point), NULL, &replyPoint);

    if (status == noErr) {
        Fixed fixedFontSize;
        status = GetEventParameter(event, kEventParamTextInputReplyPointSize, typeFixed, NULL, sizeof(fixedFontSize), NULL, &fixedFontSize);
        
        if (status != noErr) {
            // in 10.5

            float floatFontSize;
            status = GetEventParameter(event, kEventParamTextInputReplyPointSize, 'cgfl', NULL, sizeof(floatFontSize), NULL, &floatFontSize);
            if (status == noErr) {
                fontSize = floatFontSize;
            }
            else {
                // if we're in 10.4, we'll really be here... very strange
                
                
                fontSize = 10.0; // just a temporary value
            }
        }
        else {
            // NSLog(@"in 10.4, size = %f, fixedSize = 0x%08x", fontSize, fixedFontSize);
            fontSize = FixedToFloat(fixedFontSize);
        }
    }
			
    if (event)
        ReleaseEvent(event);

    // translate the point back
    NSPoint point = NSMakePoint((float)replyPoint.h, (float)replyPoint.v);
    
    // let's find where the point falls in first
    NSRect frame = [[NSScreen mainScreen] frame];
    BOOL hasFocus = NO;
    
    NSArray *screens = [NSScreen screens];
	NSScreen *screen;
	NSEnumerator *enumerator = [screens objectEnumerator];
	while (screen = [enumerator nextObject]) {
        NSRect screenFrame = [screen frame];

        if (!hasFocus && point.x >= NSMinX(screenFrame) && point.x <= NSMaxX(screenFrame)) {
            frame = screenFrame;
            hasFocus = YES;
			break;
        }
    }
    
    if (hasFocus) {
        point.y = [[NSScreen mainScreen] frame].size.height - point.y;

        // we don't want the window to be out of screen, so...
        if (point.x > NSMaxX(frame)) point.x = NSMaxX(frame);
        if (point.y > NSMaxY(frame)) point.y = NSMaxY(frame);
        if (point.x < NSMinX(frame)) point.x = NSMinX(frame);
        if (point.y < NSMinY(frame)) point.y = NSMinY(frame);
    }
    else {
        // can't find a proper focus (ie. a screen where the point falls in), so we set it to center
        point.x = frame.origin.x + frame.size.width / 2.0;
        point.y = frame.origin.y + frame.size.height / 2.0;
    }
    
    if (fontSize != 0.0) {
        *returnRect = NSMakeRect(point.x, point.y, 1.0, fontSize);
        return [NSDictionary dictionaryWithObjectsAndKeys:[NSFont fontWithName:@"Helvetica" size:fontSize], NSFontAttributeName, nil];
    }
    
    *returnRect = NSMakeRect(point.x, point.y, 1.0, 10.0);
    return [NSDictionary dictionary];
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

- (void)refreshMenuWithNSMenu:(NSMenu *)menu
{
    [self refreshMenu:[[self class] translateMenuItemArray:[menu itemArray]]];
}

- (void)refreshMenu:(NSArray*)menuItems
{
	int count = CountMenuItems(TSCSharedMenu);
	
	if (count > 0)
	    DeleteMenuItems(TSCSharedMenu, 1, count);
    
    NSEnumerator *e = [menuItems objectEnumerator];
    NSDictionary *item;
    
    count = 0;
    
    while (item = [e nextObject]) {
        NSString *title = [item objectForKey:@"Title"];
        int state = [[item objectForKey:@"State"] intValue];
        BOOL enabled = [[item objectForKey:@"IsEnabled"] boolValue];
        
		NSString *keyEquivalent = [item objectForKey:@"KeyEquivalent"];
		unsigned int keyEquivalentModifierMask = [[item objectForKey:@"KeyEquivalentModifierMask"] intValue];
		
        if ([title isEqualToString:@"-"]) {
            InsertMenuItemTextWithCFString(TSCSharedMenu, (CFStringRef)title, count, kMenuItemAttrSeparator, 0);
            count++;
        }
        else {
            InsertMenuItemTextWithCFString(TSCSharedMenu, (CFStringRef)title, count, 0, TSC_MENU_ITEM_INDEX_BASE + count);
            count++;

			if (keyEquivalent) {
				Boolean isVirtualKey = TRUE;
				UniChar uc = [keyEquivalent characterAtIndex:0];
				UInt16 inKey = uc;
				
				// this is copied from the old CocoaVanilla's CVSmartMenu
				#define VKC(k, v) case k: inKey = v; break;
				switch (uc) {
					VKC(32, 0x31); // space
					VKC(27, 0x35); // esc
					VKC(13, 0x24); // return
					VKC(127, 0x75); //delete
					VKC(8, 0x33); // backspace
					VKC(30, 0x7e); // up
					VKC(31, 0x7d); // down
					VKC(28, 0x7b); // left
					VKC(29, 0x7c); // right
					VKC(1, 0x73); // home
					VKC(4, 0x77); // end
					VKC(11, 0x74); // page up
					VKC(12, 0x79); // page down
					VKC(9, 0x30); // tab	
					default:
						isVirtualKey = FALSE;
				}
			#undef VKC
				
				SetMenuItemCommandKey(TSCSharedMenu, count, isVirtualKey, inKey);
				
				UInt8 modifiers = 0;
				if (!(keyEquivalentModifierMask & NSCommandKeyMask)) modifiers |= kMenuNoCommandModifier;
				if (keyEquivalentModifierMask & NSShiftKeyMask) modifiers |= kMenuShiftModifier;
				if (keyEquivalentModifierMask & NSAlternateKeyMask) modifiers |= kMenuOptionModifier;
				if (keyEquivalentModifierMask & NSControlKeyMask) modifiers |= kMenuControlModifier;
				
				SetMenuItemModifiers(TSCSharedMenu, count, modifiers);
			}
			
            if (state == NSOnState)
                CheckMenuItem(TSCSharedMenu, count, TRUE);
            
            if (!enabled)
                DisableMenuItem(TSCSharedMenu, count);        
        }
    }
}
- (NSString*)bundleIdentifier
{
    return [[NSBundle mainBundle] bundleIdentifier];
}
@end

//
// Free Tibet.
//
