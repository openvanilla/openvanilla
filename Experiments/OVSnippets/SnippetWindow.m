#import "SnippetWindow.h"

NSPoint setWindowPosition(NSRect windowRect)
{	
	NSPoint point;
	NSRect frame = [[NSScreen mainScreen] frame];
	point.y = windowRect.size.height + 50;
	point.x = frame.size.width - windowRect.size.width - 20;		
	return point;
}

NSSize setWindowSize(NSRect windowRect)
{
	NSRect frame = [[NSScreen mainScreen] frame];	
	float width = windowRect.size.width;
	float height = frame.size.height;
	NSSize size = NSMakeSize(width,height);
	return size;
}

@implementation SnippetWindow

- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag
{	
    NSWindow *window = [super initWithContentRect:contentRect styleMask: (NSBorderlessWindowMask | NSUtilityWindowMask | NSNonactivatingPanelMask | NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask) backing:NSBackingStoreBuffered defer:NO]; 
    [window setLevel:NSScreenSaverWindowLevel];
    [window setAlphaValue:1.0];
    [window setOpaque:NO];
    [window setHasShadow:YES];	
	[window setMovableByWindowBackground:YES];
    [window setFrameTopLeftPoint:setWindowPosition([window frame])];	
	[window setMaxSize:setWindowSize([window frame])];
	_isZoomed = NO;
	_soundEffet = [NSSound soundNamed:@"keydown"];	
    return window;
}

- (NSTimeInterval)animationResizeTime:(NSRect)newWindowFrame {
	NSTimeInterval timeInterval = 0.3;
	return timeInterval;
}

- (void)saveSize {
	_currentWidth = [super frame].size.width;
	_currentHeight = [super frame].size.height;	
	_currentY = [super frame].origin.y;
	_currentX = [super frame].origin.x;
}

- (void)zoom:(id)sender {
	NSPoint point;
	point.y = [super frame].origin.y;
	point.x = [super frame].origin.x;	
	if(_isZoomed == NO) {	
		[self saveSize];		
		NSRect rect = NSMakeRect(point.x,point.y + [super frame].size.height - 36 ,[super frame].size.width,36);
		[super setFrame:rect display:YES animate:YES];
		[_soundEffet play];
		_isZoomed = YES;
	} else {
		NSRect rect = NSMakeRect(_currentX,_currentY,_currentWidth,_currentHeight);
		[super setFrame:rect display:YES animate:YES];
		[_soundEffet play];		
		_isZoomed = NO;		
	}
}

-(void) isZoomed {
	return _isZoomed;
}

- (BOOL)canBecomeKeyWindow
{
    return YES;
}
- (BOOL)canBecomeMainWindow
{
    return NO;
}

@end
