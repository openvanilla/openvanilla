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
#import <WebKit/WebKit.h>

#define CVIB_FADEWAIT       0.5     // fade after 0.5 sec
#define CVIB_FADEINTERVAL   0.05    // 0.05 sec/frame
#define CVIB_FADEVALUE      0.025   // alphaValue-=0.025/frame

enum {
    OVDPS_NOTIFY_DEFAULT=0,
    OVDPS_NOTIFY_SILENT=1
};

void CVFixWindowOrigin(NSWindow *w, Point p);

#define CVWS_EXAMPLE		@"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html xmlns=\"http://www.w3.org/1999/xhtml\"><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /><head><title></title><link rel=\"stylesheet\" href=\"%@\" type=\"text/css\" media=\"screen\" /></head><body><p>%@</p></body></html>"
#define CVWS_URLBASE		@"~/Sites/WebKitServer/style.css"
#define CVWS_DEFAULTFILE	@"~/Sites/DefaultCandidate.html"


@protocol OVKeyReceiver
- (void)sendKey:(char)c;
@end


@implementation ServerDelegate
- (IBAction)sendKeyTest:(id)sender {
	NSLog(@"getting remote object");
	id keyrcvr=[[NSConnection rootProxyForConnectionWithRegisteredName:@"OVKeyReceiverTest" host:nil] retain];
	if (keyrcvr) {
		NSLog(@"setting protocol");
		[keyrcvr setProtocolForProxy:@protocol(OVKeyReceiver)];
		
		NSLog(@"sending keys");
		[keyrcvr sendKey:'e'];
		[keyrcvr sendKey:' '];
		[keyrcvr sendKey:'2'];
		NSLog(@"releasing remote object");
		[keyrcvr release];
		NSLog(@"session ended");
	}
	else {
		NSLog(@"cannot connect to remote object");
	}
}

- (void)awakeFromNib {
	NSLog(@"WebkitServer started");
	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:OVDSPSRVR_NAME]) ; // NSLog(@"OVDisplayServer registered");
	else {
		NSLog(@"WebkitServer registration failed");
		NSAlert *a=[NSAlert alertWithMessageText:@"WebKitServer registration failed" 
			defaultButton:nil
			alternateButton:nil
			otherButton:nil
			informativeTextWithFormat:@"Make sure you have killed the process of OVDisplayServer"];
		[a runModal];
		[[NSApplication sharedApplication] terminate:self];
	}

	candion=NO;
    defaultbackground=[[candi backgroundColor] retain];
	fadetimer=nil;

	NSLog([CVWS_URLBASE stringByExpandingTildeInPath]);
	urlbase=[[NSURL alloc] initFileURLWithPath:[CVWS_URLBASE stringByExpandingTildeInPath]];
	NSLog([urlbase absoluteString]);
	
	[candi setLevel:NSScreenSaverWindowLevel];
	[noti setLevel:NSScreenSaverWindowLevel];
	
	NSString *toload=[CVWS_DEFAULTFILE stringByExpandingTildeInPath];
	NSLog(@"this default candidate html file will be used: %@", toload);
	
	NSURL *loadurl=[NSURL fileURLWithPath:toload];
	NSLog([loadurl description]);
		
	[[candiweb mainFrame] loadRequest:[NSURLRequest requestWithURL:loadurl]];
	
	WebScriptObject *scrobj = [candiweb windowScriptObject];
	id y;
	
	// NSString *href = [scrobj evaluateWebScript:@"location.href"];
	// NSLog(@"href=%@", href);
	
	
//	y=[scrobj callWebScriptMethod:@"ov_init()" withArguments:[NSArray array]];
  

/*
	y=[scrobj evaluateWebScript:@"window.document.body"];
	if (y) {
	   NSLog(@"window.document.body = %@", [y description]);
	}
	*/
/*
	y=[scrobj evaluateWebScript:@"1+1"];
	if (y) {
	   NSLog(@"1+1 = %@", [y description]);
	}
*/
/*	
	
	y=[scrobj evaluateWebScript:@"ov_init();"];
	if (y) {
	   NSLog(@"after ov_init = %@", [y description]);
	}
	
	y=[scrobj evaluateWebScript:@"(typeof test == \"undefined\")"];
	if (y) {
	   NSLog(@"after test = %@", [y description]);
	}
	
	
	y=[scrobj evaluateWebScript:@"ov_update(\"hello, world! alert!!\")"];
	if (y) {
	   NSLog([y description]);
	}
	y=[scrobj evaluateWebScript:@"window.alert('hello, world!')"];
	
	if (y) {
	   NSLog([y description]);
	}
*/
}
- (void)setConfig:(NSDictionary*)cfg {
	[self applyConfig:cfg window:candi];
	[self applyConfig:cfg window:noti];
	notialpha=[noti alphaValue];

    notistyle=OVDPS_NOTIFY_DEFAULT; 
    NSString *ns=[cfg valueForKey:@"notificationStyle" default:@"default"];
    if ([ns isEqualToString:@"silent"]) notistyle=OVDPS_NOTIFY_SILENT;
}
- (void)candidateShow {
	candion=YES;
	[self solveConflict];
	[candi orderFront:self];
    // nop();
}
- (void)candidateHide {
	candion=NO;
	[candi orderOut:self];
}
- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {
	NSLog(@"candiupdate");

	WebScriptObject *scrobj = [candiweb windowScriptObject];
	
	NSString *script=[NSString stringWithFormat:@"ov_update('%@')", s];
	NSLog(@"evaluating javascript: %@", script);
	id y=[scrobj evaluateWebScript:script];
	if (y) {
	   NSLog(@"update text!!!!", [y description]);
	}


	// NSString *ws=[NSString stringWithFormat:CVWS_EXAMPLE, [urlbase absoluteString], s];
	// [[candiweb mainFrame] loadHTMLString:ws baseURL:[NSURL URLWithString:@"http://localhost"]];

	CVFixWindowOrigin(candi, p);
}
- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {
	NSLog(@"notiupdate");

	NSString *ws=[NSString stringWithFormat:CVWS_EXAMPLE, [urlbase absoluteString], s];
	[[notiweb mainFrame] loadHTMLString:ws baseURL:[NSURL URLWithString:@"http://localhost"]];

	CVFixWindowOrigin(noti, p);
	[self solveConflict];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    [noti orderFront:self];
}
- (void)notifyClose {
	[noti orderOut:self];
}
- (void)notifyFade {
    if (fadetimer) [self stopTimer];
    
    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
    fadetimer=[NSTimer scheduledTimerWithTimeInterval:CVIB_FADEWAIT target:self selector:@selector(fadeStart) userInfo:nil repeats:NO];
}
- (void)aboutDialog {
    // [aboutdialog setLevel:NSScreenSaverWindowLevel];
    // [aboutdialog orderFront:self];
	[[NSApplication sharedApplication] orderFrontStandardAboutPanel:self];
}
- (void)dealloc {
    [defaultbackground release];
	[urlbase release];
	[super dealloc];
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
        // nop();
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
- (void)applyConfig:(NSDictionary*)d window:(NSWindow*)w {
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
    
    // [t setFont:[NSFont fontWithName:font size:s]];
	// [t setTextColor:fc];
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
