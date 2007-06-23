#import "CandiWindow.h"
#import <WebKit/WebKit.h>

@implementation CandiWindow

//In Interface Builder we set CustomWindow to be the class for our window, so our own initializer is called here.
- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag {
	
    NSWindow* result = [super initWithContentRect:contentRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    [result setBackgroundColor: [NSColor clearColor]];
    [result setLevel: NSScreenSaverWindowLevel];
    [result setAlphaValue:1.0];
    [result setOpaque:NO];
    [result setHasShadow: YES];
    return result;
}

// Custom windows that use the NSBorderlessWindowMask can't become key by default.  Therefore, controls in such windows
// won't ever be enabled by default.  Thus, we override this method to change that.
- (BOOL) canBecomeKeyWindow
{
    return NO;
}

//Once the user starts dragging the mouse, we move the window with it. We do this because the window has no title
//bar for the user to drag (so we have to implement dragging ourselves)
- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint currentLocation;
	NSPoint newOrigin;
	NSRect  screenFrame = [[NSScreen mainScreen] frame];
	NSRect  windowFrame = [self frame];
	
	NSLog(@"Start dragging");
	
	WebScriptObject *scrobj = [candiweb windowScriptObject];
	
	NSString *script=[NSString stringWithFormat:@"ov_startdrag()"];
	NSLog(@"evaluating javascript: %@", script);
	id y=[scrobj evaluateWebScript:script];
	if (y) {
		NSLog(@"Script of Start dragging: %@", [y description]);
	}	
	
	
	//grab the current global mouse location; we could just as easily get the mouse location 
	//in the same way as we do in -mouseDown:
    currentLocation = [self convertBaseToScreen:[self mouseLocationOutsideOfEventStream]];
    newOrigin.x = currentLocation.x - initialLocation.x;
    newOrigin.y = currentLocation.y - initialLocation.y;
    
    // Don't let window get dragged up under the menu bar
    if( (newOrigin.y+windowFrame.size.height) > (screenFrame.origin.y+screenFrame.size.height) ){
		newOrigin.y=screenFrame.origin.y + (screenFrame.size.height-windowFrame.size.height);
    }
    
    //go ahead and move the window to the new location
    [self setFrameOrigin:newOrigin];
}

//We start tracking the a drag operation here when the user first clicks the mouse,
//to establish the initial location.
- (void)mouseDown:(NSEvent *)theEvent
{    
    NSRect  windowFrame = [self frame];
	
    //grab the mouse location in global coordinates
	initialLocation = [self convertBaseToScreen:[theEvent locationInWindow]];
	initialLocation.x -= windowFrame.origin.x;
	initialLocation.y -= windowFrame.origin.y;

	NSLog(@"Stop dragging");
	
	WebScriptObject *scrobj = [candiweb windowScriptObject];
	
	NSString *script=[NSString stringWithFormat:@"ov_stopdrag()"];
	NSLog(@"evaluating javascript: %@", script);
	id y=[scrobj evaluateWebScript:script];
	if (y) {
		NSLog(@"Script of Stop dragging: %@", [y description]);
	}		
}

@end
