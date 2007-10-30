//  LVInputMethodController.m

#import "LVInputMethodController.h"

@implementation LVInputMethodController
- (id)initWithServer:(IMKServer*)server delegate:(id)delegate client:(id)inputClient
{
	if (!_sharedLoader)
		return nil;

	if (self = [super initWithServer:server delegate:delegate client:inputClient]) {
		_bufferContent = [NSMutableString new];
		_composingBuffer = new LVComposingBuffer(self);
		_context = _sharedLoader->newContext();
		_currentClient = nil;
	}
	
	return self;
}
- (void)dealloc
{
	delete _composingBuffer;
	delete _context;
	[_bufferContent release];
	[super dealloc];
}
- (void)finalize
{
	[super finalize];
}
- (void)updateCursorPosition
{
	NSRect lineHeightRect;
	[_currentClient attributesForCharacterIndex:0 lineHeightRectangle:&lineHeightRect];
	_cursorPosition = lineHeightRect.origin;
}
- (void)activateServer:(id)sender
{
 	_sharedLoader->setActiveContext(_context);
	
	_currentClient = sender;
	_context->activate(_composingBuffer);
}
- (void)deactivateServer:(id)sender
{
	_context->clearAll();
	_context->deactivate();
	_sharedLoader->setActiveContext(0);
	_currentClient = nil;
}
- (void)commitComposition:(id)sender 
{
	if ([_bufferContent length] > 0) {
		[sender insertText:_bufferContent replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
	}
}
- (BOOL)handleEvent:(NSEvent*)event client:(id)sender
{
	_currentClient = sender;
	
	if ([event type] == NSKeyDown) {
		[self updateCursorPosition];

		NSString *chars = [event characters];
		NSUInteger cocoaModifiers = [event modifierFlags];
		// unsigned short keyCode = [event keyCode];
		
		UInt32 carbonModifiers = 0;
		if (cocoaModifiers & NSAlphaShiftKeyMask) carbonModifiers = carbonModifiers | alphaLock;
		if (cocoaModifiers & NSShiftKeyMask) carbonModifiers = carbonModifiers | shiftKey;
		if (cocoaModifiers & NSControlKeyMask) carbonModifiers = carbonModifiers | controlKey;
		if (cocoaModifiers & NSAlternateKeyMask) carbonModifiers = carbonModifiers | optionKey;
		if (cocoaModifiers & NSCommandKeyMask) carbonModifiers = carbonModifiers | cmdKey;
		
		UniChar unicharCode = 0;
		if ([chars length] > 0) {
			unicharCode = [chars characterAtIndex:0];
		}

		return _context->event(unicharCode, carbonModifiers);
	}

	return NO;
}
- (void)handleMenuEvent:(id)sender
{
	_sharedLoader->menuHandler([[sender objectForKey:@"IMKCommandMenuItem"] tag]);
}
- (NSMenu*)menu
{	
	NSMenu *menu = [[[NSMenu alloc] init] autorelease];
	
	UInt16 carbonMenuItemCount = CountMenuItems(_sharedCarbonMenu);
	UInt16 menuIndex;
	for (menuIndex = 1; menuIndex <= carbonMenuItemCount; menuIndex++) {
		MenuItemAttributes itemAttributes;
		GetMenuItemAttributes(_sharedCarbonMenu, menuIndex, &itemAttributes);
		
		MenuCommand commandID;
		GetMenuItemCommandID(_sharedCarbonMenu, menuIndex, &commandID);
		
		UInt16 keyCode;
		GetMenuItemCommandKey(_sharedCarbonMenu, menuIndex, FALSE, &keyCode);
		
		SInt16 keyGlyph;
		GetMenuItemKeyGlyph(_sharedCarbonMenu, menuIndex, &keyGlyph);

		UInt8 modifiers;
		GetMenuItemModifiers(_sharedCarbonMenu, menuIndex, &modifiers);
		   
		CFStringRef menuText;
		CopyMenuItemTextAsCFString(_sharedCarbonMenu, menuIndex, &menuText);

		CharParameter charParameter;
		GetItemMark(_sharedCarbonMenu, menuIndex, &charParameter);

		NSMenuItem *menuItem;
		if (itemAttributes & kMenuItemAttrSeparator) {
			menuItem = [NSMenuItem separatorItem];
		}
		else {
			menuItem = [[[NSMenuItem alloc] init] autorelease];
			
			if (itemAttributes & kMenuItemAttrDisabled) {
				[menuItem setEnabled:NO];
			}
			else {
				[menuItem setTag:commandID];
				[menuItem setAction:@selector(handleMenuEvent:)];
				[menuItem setTarget:self];
			}
			
			if (charParameter) {
				[menuItem setState:NSOnState];
			}
			
			if (keyGlyph) {
				switch(keyGlyph) {
					case kMenuSpaceGlyph:
						keyCode = ovkSpace;
						break;
					case kMenuDeleteRightGlyph:
						keyCode = ovkDelete;
						break;
					case kMenuDeleteLeftGlyph:
						keyCode = ovkBackspace;
						break;
					case kMenuUpArrowGlyph:
						keyCode = ovkUp;
						break;
					case kMenuDownArrowGlyph:					
						keyCode = ovkDown;
						break;
					case kMenuLeftArrowGlyph:
						keyCode = ovkLeft;
						break;
					case kMenuRightArrowGlyph:
						keyCode = ovkRight;
						break;
					case kMenuPageUpGlyph:
						keyCode = ovkPageUp;
						break;
					case kMenuPageDownGlyph:
						keyCode = ovkPageDown;
						break;
					case kMenuTabRightGlyph:
						keyCode = ovkTab;
						break;
				}
			}
			
			NSUInteger cocoaModifiers = 0;
			if (keyCode) {
				if (modifiers & kMenuShiftModifier) cocoaModifiers = cocoaModifiers | NSShiftKeyMask;
				if (modifiers & kMenuOptionModifier) cocoaModifiers = cocoaModifiers | NSAlternateKeyMask;
				if (modifiers & kMenuControlModifier) cocoaModifiers = cocoaModifiers | NSControlKeyMask;
				if (!(modifiers & kMenuNoCommandModifier)) cocoaModifiers = cocoaModifiers | NSCommandKeyMask;
			}
			
			[menuItem setTitle:(NSString*)menuText];
			[menuItem setKeyEquivalent:[NSString stringWithFormat:@"%c", keyCode]];
			[menuItem setKeyEquivalentModifierMask:cocoaModifiers];
			
		}
		
		[menu addItem:menuItem];		
		CFRelease(menuText);
	}

	return menu;
}
- (void)composingBufferClear
{
	[_bufferContent setString:@""];
}
- (void)composingBufferSend
{
	[self commitComposition:_currentClient];
	[self composingBufferClear];
}
- (int)bufferContentRealPosition:(int)codepointPosition
{
    int rp=0;
    unsigned int i;
    
    for (i=0; i<[_bufferContent length]; i++)
    {
        if (rp == codepointPosition) break;

        // see if encounters a surrogate pair
        UniChar c=[_bufferContent characterAtIndex: i];
        if (c >= 0xd800 && c < 0xdc00)
        {
            c=[_bufferContent characterAtIndex: ++i];
            if (i<[_bufferContent length] && c >= 0xdc00 && c < 0xe000) rp++;
        }
        else rp++;
    }
    
    return i;    

}
- (void)composingBufferUpdate:(BOOL)shouldSend cursorPosition:(int)position highlightFrom:(int)from to:(int)to
{
	if (shouldSend) {
		[self commitComposition:_currentClient];
		[self composingBufferClear];		
		return;
	}

	NSMutableAttributedString *attrString = [[[NSMutableAttributedString alloc] initWithString:_bufferContent attributes:[NSDictionary dictionary]] autorelease];
	NSRange selectionRange;
	
	if (position == -1)
		selectionRange = NSMakeRange([_bufferContent length], 0);
	else
		selectionRange = NSMakeRange([self bufferContentRealPosition:position], 0);
	
	if (from >= 0 && to >= 0 && from != to) {
		int realFrom = [self bufferContentRealPosition:from];
		int realTo = [self bufferContentRealPosition:to];
		
		int section = 0;
		NSRange markedRange;
		if (realFrom > 0) {
			markedRange = NSMakeRange(0, realFrom);
			[attrString setAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
				[NSNumber numberWithInt:NSUnderlineStyleSingle],
					NSUnderlineStyleAttributeName,
					[NSNumber numberWithInt:section],
					NSMarkedClauseSegmentAttributeName,
					nil]
				range:markedRange];
			section++;
		}
		
		if (realTo - realFrom > 0) {
			markedRange = NSMakeRange(realFrom, realTo - realFrom);
			[attrString setAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
				[NSNumber numberWithInt:NSUnderlineStyleThick],
					NSUnderlineStyleAttributeName,
					[NSNumber numberWithInt:section],
					NSMarkedClauseSegmentAttributeName,
					nil]
				range:markedRange];
			section++;
		}
		
		if (realTo < [_bufferContent length]) {
			markedRange = NSMakeRange(realTo, [_bufferContent length] - realTo);
			[attrString setAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
				[NSNumber numberWithInt:NSUnderlineStyleSingle],
					NSUnderlineStyleAttributeName,
					[NSNumber numberWithInt:section],
					NSMarkedClauseSegmentAttributeName,
					nil]
				range:markedRange];
			section++;
		}
		
	}
	
	[_currentClient setMarkedText:attrString selectionRange:selectionRange replacementRange:NSMakeRange(NSNotFound, NSNotFound)];
}
- (void)composingBufferAppend:(NSString*)string
{
	[_bufferContent appendString:string];
}
- (BOOL)isComposingBufferEmpty
{
	return [_bufferContent length] == 0;
}
- (Point)getCursorPosition
{
	// translates Cocoa coordinate to Carbon coordinate
	Point point;
	point.h = _cursorPosition.x;
	
	// let's find where the point falls in first
	NSRect frame = [[NSScreen mainScreen] frame];
	BOOL hasFocus = NO;
	
	NSArray *screens = [NSScreen screens];
	int i, c = [screens count];
	for (i = 0; i < c; i++) {
		NSScreen *screen = [screens objectAtIndex:i];
		NSRect screenFrame = [screen frame];

		if (!hasFocus && _cursorPosition.x >= NSMinX(screenFrame) && _cursorPosition.x <= NSMaxX(screenFrame)) {
			frame = screenFrame;
			hasFocus = YES;
		}
	}
	
	if (hasFocus) {
		point.v = (int)([[NSScreen mainScreen] frame].size.height - _cursorPosition.y);
	}
	else {
		point.h = (int)(frame.size.width / 2.0);
		point.v = (int)(frame.size.height / 2.0);
	}
	
	return point;
}
- (NSMutableString*)composingBufferContent
{
	return [NSMutableString stringWithString:_bufferContent];
}
@end
