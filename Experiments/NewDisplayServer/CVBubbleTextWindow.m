//
//  CVBubbleTextWindow.m
//  NewDisplayServer
//

#import "CVBubbleTextWindow.h"
#import "CVBubbleTextView.h"

@implementation CVBubbleTextWindow
- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{	
    NSWindow *window = [super initWithContentRect:contentRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    [window setBackgroundColor:[NSColor clearColor]];
    [window setLevel:NSScreenSaverWindowLevel];
    [window setAlphaValue:1.0];
    [window setOpaque:NO];
    [window setHasShadow:YES];	
    return window;
}
- (BOOL)canBecomeKeyWindow
{
    return YES;
}
- (BOOL)canBecomeMainWindow
{
    return NO;
}
- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint currentLocation;
	NSPoint newOrigin;
	NSRect screenFrame = [[NSScreen mainScreen] frame];
	NSRect windowFrame = [self frame];
	
    currentLocation = [self convertBaseToScreen:[self mouseLocationOutsideOfEventStream]];
    newOrigin.x = currentLocation.x - _initialLocation.x;
    newOrigin.y = currentLocation.y - _initialLocation.y;
    
    if ((newOrigin.y + windowFrame.size.height) > (screenFrame.origin.y + screenFrame.size.height))
		newOrigin.y=screenFrame.origin.y + (screenFrame.size.height-windowFrame.size.height);
    
    [self setFrameOrigin:newOrigin];
}
- (void)mouseDown:(NSEvent *)theEvent
{
    NSRect windowFrame = [self frame];
	_initialLocation = [self convertBaseToScreen:[theEvent locationInWindow]];
	_initialLocation.x -= windowFrame.origin.x;
	_initialLocation.y -= windowFrame.origin.y;
}
- (void)setText:(NSString *)newText
{
	[(CVBubbleTextView*)_textView setText:newText];
}
- (void)setBubbleBackgroundColor:(NSColor *)color
{
	[(CVBubbleTextView*)_textView setBackgroundColor:color];
}
- (void)setTextColor:(NSColor *)color
{
	[(CVBubbleTextView*)_textView setTextColor:color];
}
- (void)setFont:(NSFont *)font
{
	[(CVBubbleTextView*)_textView setFont:font];
}
- (void)updateWindowSizeWithAnimation
{
	NSRect frame = [self frame];	
	frame.size = [(CVBubbleTextView*)_textView boundingRect].size;
	[self setFrame:frame display:YES animate:YES];
}
@end