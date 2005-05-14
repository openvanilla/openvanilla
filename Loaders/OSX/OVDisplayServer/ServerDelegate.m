#import "ServerDelegate.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import <Carbon/Carbon.h>

NSPoint CVPointToNSPoint(Point p, NSWindow *w);
Point CVFixWindowPosition(Point pp, int width, int height);

@implementation ServerDelegate
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t {
	float alpha=[[d valueForKey:@"opacity" default:@"1.0"] floatValue];
	NSColor *fc=[[d valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSColor *bc=[[d valueForKey:@"background" default:@"1.0 1.0 1.0"] colorByString];
	NSString *img=[d valueForKey:@"backgroundImage" default:@""];
	NSString *font=[d valueForKey:@"font" default:@"Lucida Grande"];
	float s=[[d valueForKey:@"size" default:@"18"] floatValue];

	[w setAlphaValue:alpha];
	[w setBackgroundColor:bc];
	if ([img length]) {
		NSImage *i=[[NSImage alloc] initByReferencingFile:img];
		if (i && [i isValid]) {
			[i autorelease];
			NSColor *c=[NSColor colorWithPatternImage:i];
			[w setBackgroundColor:c];
		}
	}

    [t setFont:[NSFont fontWithName:font size:s]];
	[t setTextColor:fc];
}
- (void)setConfig:(NSDictionary*)cfg {
	NSLog([cfg description]);
	[self applyConfig:cfg window:candi textField:canditext];
}
- (void)candidateShow {
	[candi orderFront:self];
}
- (void)candidateHide {
	[candi orderOut:self];
}
- (void)candidateUpdate:(NSString*)s position:(Point)p {
	[canditext setStringValue:s];
    [canditext sizeToFit];
    NSRect r=[canditext frame];
    [candi setContentSize:r.size];
    
    Point realpos=CVFixWindowPosition(p, (int)r.size.width, (int)r.size.height);
    NSPoint nspos=CVPointToNSPoint(realpos, candi);    
    [candi setFrameTopLeftPoint:nspos];
}
- (NSTextField*)createTextField:(NSRect)fr {
	NSTextField *t=[[NSTextField alloc] initWithFrame:fr];
    [t setEditable:NO];
    [t setSelectable:NO];
    [t setBordered:NO];
    [t setDrawsBackground:NO];
    [t setFrameOrigin:NSMakePoint(0,0)];
	return t;
}
- (NSWindow*)createWindow:(NSRect)fr {
	NSWindow *w=[[NSWindow alloc] initWithContentRect:fr styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    [w setLevel:NSScreenSaverWindowLevel];
	[w setMovableByWindowBackground:YES];
	[w orderOut:self];
	[w setContentView:[[NSView alloc] initWithFrame:[w contentRectForFrameRect:fr]]];
	return w;
}
- (void)dealloc {
	[candi release];
	[super dealloc];
}
- (void)awakeFromNib {
	NSLog(@"setting up OVDisplayServer");
	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:@"OVDisplayServer"]) NSLog(@"OVDisplayServer registered");
	else NSLog(@"OVDisplayServer registration failed");

	NSRect fr=NSMakeRect(0, 0, 100, 20);
	candi=[self createWindow:fr];
    canditext=[self createTextField:fr];
    [[candi contentView] addSubview:canditext];
}
@end

NSPoint CVPointToNSPoint(Point p, NSWindow *w) {
    NSPoint pos;
    pos.x=p.h;

    // get the screen the window is on and calculate the device coordination
    // if no screen, it's offscreen anyway, position is not important...
    NSScreen *screen=[w screen];
    if (screen) {
        NSRect screenrect=[screen frame];
        pos.y=NSMaxY(screenrect)-p.v;
    }
    else pos.y=p.v;
    return pos;
}

// this is copied from VXTextBar.cpp of OVLoader 0.6.3 :)
Point CVFixWindowPosition(Point pp, int width, int height) {
    Point p=pp;
	Rect bound, avail;
	GDHandle nextgd=GetDeviceList();
	
	do {
		if(!TestDeviceAttribute(nextgd, screenDevice)) continue;
		if(!TestDeviceAttribute(nextgd, screenActive)) continue;
		bound=(*nextgd)->gdRect;
		GetAvailableWindowPositioningBounds(nextgd, &avail);
		if (PtInRect(p, &bound)) break;
	} while((nextgd = GetNextDevice(nextgd)) != nil);
	
	if (!PtInRect(p, &bound)) {
		bound=(*GetMainDevice())->gdRect;
		GetAvailableWindowPositioningBounds(nextgd, &avail);
	}
	
	if (p.v > avail.bottom - height) p.v = avail.bottom-height;
	if (p.h > avail.right - width) p.h = avail.right-width;
	if (p.v < avail.top+GetMBarHeight()) p.v=avail.top+GetMBarHeight();
	if (p.h < avail.left) p.h=avail.left+width;
    return p;
}
