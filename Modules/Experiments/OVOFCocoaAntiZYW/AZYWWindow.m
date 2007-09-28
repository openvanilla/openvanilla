#import "AZYWWindow.h"

@implementation AZYWWindow

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
    return NO;
}
- (BOOL)canBecomeMainWindow
{
    return NO;	
}

@end
