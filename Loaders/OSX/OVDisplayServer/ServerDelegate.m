#import "ServerDelegate.h"
#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import <Carbon/Carbon.h>

#define CVIB_FADEWAIT       0.5     // fade after 0.5 sec
#define CVIB_FADEINTERVAL   0.05    // 0.05 sec/frame
#define CVIB_FADEVALUE      0.025   // alphaValue-=0.025/frame

enum {
    OVDPS_NOTIFY_DEFAULT=0,
    OVDPS_NOTIFY_SILENT=1
};

NSPoint CVPointToNSPoint(Point p, NSWindow *w);
Point CVFixWindowPosition(Point pp, int width, int height);

@implementation ServerDelegate
- (void)setConfig:(NSDictionary*)cfg {
   NSLog(@" (void)setConfig:(NSDictionary*)cfg ");
	[self applyConfig:cfg window:candi textField:canditext];
	[self applyConfig:cfg window:noti textField:notitext];
	notialpha=[noti alphaValue];

    notistyle=OVDPS_NOTIFY_DEFAULT;    
    NSString *ns=[cfg valueForKey:@"notificationStyle" default:@"default"];
    if ([ns isEqualToString:@"silent"]) notistyle=OVDPS_NOTIFY_SILENT;
}
- (void)candidateShow {
   NSLog(@" (void)candidateShow ");
	candion=YES;
	[self solveConflict];
	[candi orderFront:self];
        nop();
}
- (void)candidateHide {
   NSLog(@" (void)candidateHide ");
	candion=NO;
	[candi orderOut:self];
        nop();
}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {
   NSLog(@" (void)candidateUpdate:(bycopy NSString*)s position:(Point)p ");
	[self updateTextInWindow:candi textField:canditext text:s position:p];
}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {
   NSLog(@" (void)notifyMessage:(bycopy NSString*)s position:(Point)p ");
	[self updateTextInWindow:noti textField:notitext text:s position:p];
	[self solveConflict];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    [noti orderFront:self];
        nop();
}
- (void)notifyClose {
   NSLog(@" (void)notifyClose ");
	[noti orderOut:self];
        nop();
}
- (void)notifyFade {
   NSLog(@" (void)notifyFade ");
    if (fadetimer) [self stopTimer];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    fadetimer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEWAIT target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
}
- (void)aboutDialog {
   NSLog(@" (void)aboutDialog ");
    [aboutdialog setLevel:NSScreenSaverWindowLevel];
    [aboutdialog orderFront:self];
    nop();
//    NSLog(@"after aboutdialog orderFront");
}
- (void)dealloc {
   NSLog(@" (void)dealloc ");
    [defaultbackground release];
	[candi release];
	[super dealloc];
}
- (void)awakeFromNib {
   NSLog(@" (void)awakeFromNib ");
	NSLog(@"setting up OVDisplayServer");
	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:@"OVDisplayServer"]) NSLog(@"OVDisplayServer registered");
	else NSLog(@"OVDisplayServer registration failed");

	NSRect fr=NSMakeRect(0, 0, 100, 20);
	candion=NO;
	candi=[self createWindow:fr];
    defaultbackground=[[candi backgroundColor] retain];
    canditext=[self createTextField:fr];
    [[candi contentView] addSubview:canditext];
	noti=[self createWindow:fr];
	notitext=[self createTextField:fr];
    [[noti contentView] addSubview:notitext];
	
	fadetimer=nil;
}
- (void)stopTimer {
   NSLog(@" (void)stopTimer ");
    if (fadetimer) [fadetimer invalidate];
    fadetimer=nil;
	[noti setAlphaValue:notialpha];
}
- (void)fadeStart {
   NSLog(@" (void)fadeStart ");
    [fadetimer invalidate];
    fadetimer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEINTERVAL target:self selector:@selector(fadeWork) userInfo:nil repeats:YES];
    fadealpha=notialpha;
}
- (void)fadeWork {
   NSLog(@" (void)fadeWork ");
    fadealpha-=CVIB_FADEVALUE;
    if (fadealpha<=0) {
        [noti orderOut:self];
        [self stopTimer];
        nop();
        return;
    }
	[noti setAlphaValue:fadealpha];

}
- (void)solveConflict {
   NSLog(@" (void)solveConflict ");
	if (!candion) return;
	NSRect cfr, nfr;
	cfr=[candi frame];
	nfr=[noti frame];
	if (cfr.origin.y+cfr.size.height == nfr.origin.y+nfr.size.height) {
		nfr.origin.y = cfr.origin.y - (nfr.size.height + 5);
		[noti setFrameOrigin:nfr.origin];
	}
}
- (void)updateTextInWindow:(NSWindow*)w textField:(NSTextField*)t text:(NSString*)s position:(Point)p {
   NSLog(@" (void)updateTextInWindow:(NSWindow*)w textField:(NSTextField*)t text:(NSString*)s position:(Point)p ");
	[t setStringValue:s];
    [t sizeToFit];
    NSRect r=[t frame];
    [w setContentSize:r.size];
    
    Point realpos=CVFixWindowPosition(p, (int)r.size.width, (int)r.size.height);
    NSPoint nspos=CVPointToNSPoint(realpos, w);    
    [w setFrameTopLeftPoint:nspos];
}
- (NSWindow*)createWindow:(NSRect)fr {
   NSLog(@" (NSWindow*)createWindow:(NSRect)fr ");
	NSWindow *w=[[NSWindow alloc] initWithContentRect:fr styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];

//	NSWindow *w=[[NSPanel alloc] initWithContentRect:fr styleMask:NSUtilityWindowMask backing:NSBackingStoreBuffered defer:NO];
//	[(NSPanel*)w setBecomesKeyOnlyIfNeeded:YES]; 
    [w setLevel:NSScreenSaverWindowLevel];
	[w setMovableByWindowBackground:YES];
//	[w orderFront:self];
	[w setContentView:[[NSView alloc] initWithFrame:[w contentRectForFrameRect:fr]]];
	
	return w;
}
- (NSTextField*)createTextField:(NSRect)fr {
   NSLog(@" (NSTextField*)createTextField:(NSRect)fr ");
	NSTextField *t=[[NSTextField alloc] initWithFrame:fr];
    [t setEditable:NO];
    [t setSelectable:NO];
    [t setBordered:NO];
    [t setDrawsBackground:NO];
    [t setFrameOrigin:NSMakePoint(0,0)];
	return t;
}
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t {
   NSLog(@" (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t ");
	float alpha=[[d valueForKey:@"opacity" default:@"1.0"] floatValue];
	NSColor *fc=[[d valueForKey:@"foreground" default:@"1.0 1.0 1.0"] colorByString];
	NSString *img=[d valueForKey:@"backgroundImage" default:@""];
	NSString *font=[d valueForKey:@"font" default:@"Lucida Grande"];
	float s=[[d valueForKey:@"size" default:@"18"] floatValue];

	[w setAlphaValue:alpha];
    [w setHasShadow:YES];
    [w setOpaque:YES];
    if ([img length]) {
		NSImage *i=[[NSImage alloc] initByReferencingFile:img];
		if (i && [i isValid]) {
			[i autorelease];
			NSColor *c=[NSColor colorWithPatternImage:i];
			[w setBackgroundColor:c];
		}
	}
    else {
        // no image, use color
        NSString *bcstr=[d valueForKey:@"background" default:@"1.0 1.0 1.0"];
        if ([bcstr isEqualToString:@"transparent"]) {
            [w setHasShadow:NO];
            [w setOpaque:NO];
            [w setBackgroundColor:[NSColor clearColor]];
        }
        else if ([bcstr isEqualToString:@"none"]) {
            [w setBackgroundColor:defaultbackground];
        }
        else {
            NSColor *bc=[bcstr colorByString];
            [w setBackgroundColor:bc];
        }
    }
    
    [t setFont:[NSFont fontWithName:font size:s]];
	[t setTextColor:fc];
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
	if (p.h < avail.left) p.h=avail.left;
    
    return p;
}

void nop(){
    int i,j;
    for(i=0;i<10000;i++) j+=i;
    return;
}
