// ServerDelegate.mm: OVDisplayServer.app delegated object
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
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

// nop function to prevent Objective-C remote object failure
// in some architectures--DO NOT REMOVE THIS WITHOUT EXTENSIVE TESTING!

void nop() {
    volatile a=10;
}


void CVFixWindowOrigin(NSWindow *w, Point p);

@implementation ServerDelegate
- (void)setConfig:(NSDictionary*)cfg {
	[self applyConfig:cfg window:candi textField:canditext];
	[self applyConfig:cfg window:noti textField:notitext];
	notialpha=[noti alphaValue];

    notistyle=OVDPS_NOTIFY_DEFAULT;    
    NSString *ns=[cfg valueForKey:@"notificationStyle" default:@"default"];
    if ([ns isEqualToString:@"silent"]) notistyle=OVDPS_NOTIFY_SILENT;
}
- (void)candidateShow {
	candion=YES;
	[self solveConflict];
	[candi orderFront:self];
    nop();
}
- (void)candidateHide {
	candion=NO;
	[candi orderOut:self];
    nop();
}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {
	[self updateTextInWindow:candi textField:canditext text:s position:p];
}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {
	[self updateTextInWindow:noti textField:notitext text:s position:p];
	[self solveConflict];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    [noti orderFront:self];
    nop();
}
- (void)notifyClose {
	[noti orderOut:self];
    nop();
}
- (void)notifyFade {
    if (fadetimer) [self stopTimer];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    fadetimer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEWAIT target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
}
- (void)aboutDialog {
    [aboutdialog setLevel:NSScreenSaverWindowLevel];
    [aboutdialog orderFront:self];
    nop();
}
- (void)dealloc {
    [defaultbackground release];
	[candi release];
	[super dealloc];
}
- (void)awakeFromNib {
	// NSLog(@"OVDisplayServer started");
	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:OVDSPSRVR_NAME]) ; // NSLog(@"OVDisplayServer registered");
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
    if (fadetimer) [fadetimer invalidate];
    fadetimer=nil;
	[noti setAlphaValue:notialpha];
}
- (void)fadeStart {
    [fadetimer invalidate];
    fadetimer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEINTERVAL target:self selector:@selector(fadeWork) userInfo:nil repeats:YES];
    fadealpha=notialpha;
}
- (void)fadeWork {
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
	[t setStringValue:s];
    [t sizeToFit];
    NSRect r=[t frame];
    [w setContentSize:r.size];
    
	CVFixWindowOrigin(w, p);
}
- (NSWindow*)createWindow:(NSRect)fr {
	NSWindow *w=[[NSWindow alloc] initWithContentRect:fr styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:NO];
    [w setLevel:NSScreenSaverWindowLevel];
	[w setMovableByWindowBackground:YES];
	[w setContentView:[[NSView alloc] initWithFrame:[w contentRectForFrameRect:fr]]];	
	return w;
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
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w textField:(NSTextField*)t {
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

void CVFixWindowOrigin(NSWindow *w, Point p) {
	// first of all, we do a y-coordiate transformation (mirroring)
	NSPoint np=NSMakePoint((float)p.h, (float)p.v);
	
	float maxh=480;		// a silly default, but anyway the below loop always runs
	NSArray *sa=[NSScreen screens];
	int i, c=[sa count];
	for (i=0; i<c; i++) {
		float h=[[sa objectAtIndex:i] frame].size.height;
		if (h > maxh) maxh=h;
	}
	
	np.y=maxh-np.y;

	NSSize s=[w frame].size;
	
	// now we determine the window frame
	NSRect f=[[NSScreen mainScreen] frame];
	BOOL found=NO;

	for (i=0; i<c; i++) {
		NSRect sf=[[sa objectAtIndex:i] frame];
		if (NSPointInRect(np, sf)) {
			f=sf;
			found=YES;
		}
	}

	// if the origin is out of bound, we place the origin in the center of the main screen
	if (found) {
		np.y-=s.height;		// must offset the height of the window
		if (np.y > f.origin.y+f.size.height-s.height) np.y=f.origin.y+f.size.height-s.height;
		if (np.x > f.origin.x+f.size.width-s.width) np.x=f.origin.x+f.size.width-s.width;
		if (np.y < f.origin.y) np.y=f.origin.y;
		if (np.x < f.origin.x) np.x=f.origin.x;
	}
	else {
		np.x = f.origin.x + (f.size.width-s.width)/2;
		np.y = f.origin.y + (f.size.height-s.height)/2;
	}
	
	[w setFrameOrigin:np];
}
