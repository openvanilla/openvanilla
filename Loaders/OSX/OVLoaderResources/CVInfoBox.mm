// CVInfoBox.mm

#include <Carbon/Carbon.h>
#import "CVInfoBox.h"

#define CVIB_FADEWAIT       0.3     // fade after 0.3 sec
#define CVIB_FADEINTERVAL   0.05    // 0.05 sec/frame
#define CVIB_FADEVALUE      0.025   // alphaValue-=0.025/frame

CVInfoBoxState::CVInfoBoxState(NSString *s, Point p, BOOL o) {
    str=[[NSString alloc] initWithString:s];
    pos=p;
    onscreen=o;
}

CVInfoBoxState::~CVInfoBoxState() {
    [str release];
}

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


@implementation CVInfoBox
- (void)awakeFromNib {
    onscreen=NO;
    pos=(Point){0, 0};
    str=[NSMutableString new];
    timer=nil;
    alpha=1;
	name=[NSString new];
    
    // trial code
    [text setTextColor:[NSColor whiteColor]];
    [[self window] setBackgroundColor:[NSColor blueColor]];
    // [text setBackgroundColor:blue];
    // [text setDrawsBackground:TRUE];
    [text setFont:[NSFont fontWithName:@"Lucida Grande" size:18.0]];
    
    [self clear];
    [self append:@"OpenVanilla"];
    [self update];
}
- (void)dealloc {
	[name release];
    [str release];
    [super dealloc];
}
- (void)setName:(NSString*)n {
	if (name) [name release];
	name=[[NSString alloc] initWithString:n];
}
- (void)clear {
    NSRange r=(NSRange){0, [str length]};
    [str deleteCharactersInRange:r];
}
- (void)update {
    if (timer) [self stopTimer];
    [text setStringValue:str];
    [text setFrameOrigin:(NSPoint){0, 0}];
    [text sizeToFit];
    NSRect r=[text frame];
    // r.size.width-=1.0;       // must do this to avoid background problem
    [[self window] setContentSize:r.size];
    
    Point realpos=CVFixWindowPosition(pos, (int)r.size.width, (int)r.size.height);
    NSPoint nspos=CVPointToNSPoint(realpos, [self window]);
    
    [[self window] setFrameTopLeftPoint:nspos];

	NSString *s=[NSString stringWithFormat:@"%@ update %d %d \"%@\"", name, (int)nspos.x, (int)nspos.y, str];
	FILE *f=fopen("/tmp/ovib-hack", "a");
	fprintf(f,"%s\n",[s UTF8String]);
	fclose(f);
}

- (void)append:(NSString*)s {
    [str appendString:s];
}
- (void)setPosition:(Point)p {
    pos=p;
}
- (void)show {
    if (timer) [self stopTimer];
    if (!onscreen) [[self window] orderFront:self];
    onscreen=TRUE;
	NSString *s=[NSString stringWithFormat:@"%@ %s", name, "show"];
	FILE *f=fopen("/tmp/ovib-hack", "a");
	fprintf(f,"%s\n",[s UTF8String]);
	fclose(f);
}
- (void)hide {
    // stop the timer
    if (timer) [self stopTimer];
    
    if (onscreen) [[self window] orderOut:self];
    onscreen=FALSE;
	NSString *s=[NSString stringWithFormat:@"%@ %s", name, "hide"];
	FILE *f=fopen("/tmp/ovib-hack", "a");
	fprintf(f,"%s\n",[s UTF8String]);
	fclose(f);
}
- (BOOL)onScreen {
    return onscreen;
}
- (int)height {
    NSRect r=[[self window] frame];
    return (int)r.size.height;
}
- (CVInfoBoxState*)saveState {
    return new CVInfoBoxState(str, pos, onscreen);
}
- (void)restoreState:(CVInfoBoxState*)s {
    [self clear];
    [self append:s->str];
    [self setPosition:s->pos];
    [self update];
    if (s->onscreen) [self show]; else [self hide];
}
- (void)fade {
    if (timer) [self stopTimer];
    timer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEWAIT target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
}
- (void)fadeStart {
    [timer invalidate];
    timer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEINTERVAL target:self selector:@selector(fadeWork) userInfo:nil repeats:YES];
    alpha=1;
}
- (void)fadeWork {
    alpha-=CVIB_FADEVALUE;
    if (alpha<=0) {
        [[self window] orderOut:self];
        onscreen=FALSE;
        [self stopTimer];
        return;
    }
    [[self window] setAlphaValue:alpha];
}
- (void)stopTimer {
    if (timer) [timer invalidate];
    timer=nil;
    [[self window] setAlphaValue:1];
    alpha=1;
}
@end
