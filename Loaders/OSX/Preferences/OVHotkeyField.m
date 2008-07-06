//
//  LFHotkeyView.m
//  ShortCut
//
//  Created by Weizhong Yang on 12/26/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "OVHotkeyField.h"
#import "NDHotKeyEvent.h"

typedef int CGSConnection;

typedef enum {
    CGSGlobalHotKeyEnable = 0,
    CGSGlobalHotKeyDisable = 1,
} CGSGlobalHotKeyOperatingMode;

extern CGSConnection _CGSDefaultConnection(void);
extern CGError CGSGetGlobalHotKeyOperatingMode(CGSConnection connection, CGSGlobalHotKeyOperatingMode *mode);
extern CGError CGSSetGlobalHotKeyOperatingMode(CGSConnection connection, CGSGlobalHotKeyOperatingMode mode);


@implementation OVHotkeyField

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (id)initWithFrame:(NSRect)frame 
{
    self = [super initWithFrame:frame];
	
    if (self) {
		NSRect textRect = NSMakeRect(0, 0, frame.size.width - 36, frame.size.height);
		u_displayTextView = [[NSTextField alloc] initWithFrame:textRect];
		[u_displayTextView setEditable:NO];
		[u_displayTextView setAlignment:NSCenterTextAlignment];
		[u_displayTextView setStringValue:@""];
		[self addSubview:u_displayTextView];	
		
		NSRect buttonRect = NSMakeRect(frame.size.width -30, 0, 30, frame.size.height);	
		u_setButton = [[NSButton alloc] initWithFrame:buttonRect];
		[u_setButton	setTitle:@"Set"];
		[u_setButton setAction:@selector(set:)];
		[u_setButton setBezelStyle:NSTexturedSquareBezelStyle];
		[u_setButton setButtonType:NSToggleButton];
		[self addSubview:u_setButton];		
    }
    return self;
}

- (void)dealloc
{
	[u_setButton release];
	[u_displayTextView release];
	[m_shortcut release];
	[hotKey release];
	[super dealloc];
}

- (NSDictionary *)hotKeyDictForEvent:(NSEvent *)event{
	unsigned int modifiers = [event modifierFlags];
	unsigned short keyCode = [event keyCode];
	NSString *character=[event charactersIgnoringModifiers];
	if (keyCode == 48) {
		character= @"\t";
	}
	
	NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
		[NSNumber numberWithUnsignedInt:modifiers],@"modifiers",
		[NSNumber numberWithUnsignedShort:keyCode],@"keyCode",
		nil];
	return dict;
}

- (NSDictionary *)hotKey 
{
	return [[hotKey retain] autorelease]; 
}

- (void)setHotKey:(NSDictionary *)newHotKey
{
    if (hotKey != newHotKey) {
        [hotKey release];
        hotKey = [newHotKey retain];
		/*
		NSDictionary *binding=[self infoForBinding:@"hotKey"];
		if (binding)
			[[binding objectForKey:NSObservedObjectKey] setValue:hotKey forKeyPath:[binding objectForKey:NSObservedKeyPathKey]];
			*/
	
		[self updateStringForHotKey];
    }
} 

- (void)updateStringForHotKey
{
	if ([hotKey isKindOfClass:[NSDictionary class]]) {
		NSString *descrip;
		if (![hotKey objectForKey:@"keyCode"]) {
			descrip = @"";
		}
		else if (![hotKey objectForKey:@"modifiers"]) {
			descrip = @"";
		}
		else {
			descrip = [[NDHotKeyEvent getHotKeyForKeyCode:[[hotKey objectForKey:@"keyCode"]shortValue] character:[[hotKey objectForKey:@"character"]characterAtIndex:0] modifierFlags:[[hotKey objectForKey:@"modifiers"]unsignedIntValue]] stringValue];
		}
		[u_displayTextView setStringValue:descrip?descrip:@""]; 
	}
	else if (hotKey) {
		[u_displayTextView setStringValue:@"invalid"];
	}
	else {
		[u_displayTextView setStringValue:@""];
	}
}

- (IBAction)set:(id)sender
{
	[self absorbEvents];
}

- (void)timerFire:(NSTimer *)timer
{
	NSTimeInterval t = [[NSDate date]timeIntervalSinceReferenceDate];
	t = fmod(t,1.0);
	t = (sin(t * M_PI * 2) + 1) / 2;
	
	NSColor *newColor = [[NSColor textBackgroundColor] blendedColorWithFraction:t ofColor:[NSColor selectedTextBackgroundColor]];	
	[u_displayTextView setBackgroundColor:newColor];
}

- (void)absorbEvents
{
	[[self window] makeFirstResponder:self];
	NSTimer *timer = [[NSTimer alloc]initWithFireDate:[NSDate dateWithTimeIntervalSinceNow:0.1] interval:0.1 target:self selector:@selector(timerFire:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	
	[u_displayTextView setBackgroundColor:[NSColor selectedTextBackgroundColor]];
	[u_setButton setState:NSOnState];
	[u_displayTextView setStringValue:@"Set Keys"];
	[[self window] display];
	NSEvent *theEvent = nil;
	
	CGSConnection conn = _CGSDefaultConnection();
	CGSSetGlobalHotKeyOperatingMode(conn, CGSGlobalHotKeyDisable);
	BOOL collectEvents = YES;
	while (collectEvents) {
		theEvent=[NSApp nextEventMatchingMask:NSKeyDownMask|NSFlagsChangedMask|NSLeftMouseDownMask|NSAppKitDefinedMask|NSSystemDefinedMask untilDate:[NSDate dateWithTimeIntervalSinceNow:10.0] inMode:NSDefaultRunLoopMode dequeue:YES];
		switch ([theEvent type]) {
			case NSKeyDown:
				{
					unsigned short keyCode=[theEvent keyCode];
					NSString *characters=[theEvent charactersIgnoringModifiers];
					if (keyCode == 48) characters=@"\t";
					
					if ([theEvent modifierFlags] & (NSCommandKeyMask|NSFunctionKeyMask|NSControlKeyMask|NSAlternateKeyMask)){
						[self setHotKey:[self hotKeyDictForEvent:theEvent]];
						collectEvents = NO; 
					}
					else if ([theEvent keyCode] == 53) { //Escape
						collectEvents = NO; 
					}
					else if ([theEvent keyCode] == 48) { //Tab
						[[self window] makeFirstResponder:[self nextKeyView]];
						collectEvents = NO;
					}
					else if ([theEvent keyCode] == 51) { //Delete
						[self setHotKey:nil];
						collectEvents=NO; 
					}
					else {
						NSBeep();
					}
				}					
					break;
			case NSFlagsChanged:
			{
				NSString *newString = stringForModifiers([theEvent modifierFlags]);
				// NSLog(newString);
				[u_displayTextView setStringValue:[newString length]?newString:@""];	
				[u_displayTextView display];
				[u_setButton display];
				break;
			}
			case NSSystemDefinedMask:
			case NSAppKitDefinedMask:
			case NSLeftMouseDown:
				collectEvents = NO;
			default:
				break;
		}
	}
	[timer invalidate];
	[timer release];
	CGSSetGlobalHotKeyOperatingMode(conn, CGSGlobalHotKeyEnable);
	[self updateStringForHotKey];
	[u_displayTextView setBackgroundColor:[NSColor textBackgroundColor]];
	[u_setButton setState:NSOffState];

}

- (void)mouseDown:(NSEvent *)event
{
	
	[self absorbEvents];
}



@end
