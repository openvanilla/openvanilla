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

#import "NSDictionaryExtension.h"
#import "NSStringExtension.h"
#import <Carbon/Carbon.h>
#import <WebKit/WebKit.h>
#import "ServerDelegate.h"

#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]
#define SERVICE_NAME @"OpenVanilla Notify"
#define	IconData [NSData dataWithData:[[NSImage imageNamed:@"DisplayServerIcon"] TIFFRepresentation]]
 
enum {
    OVDPS_NOTIFY_DEFAULT=0,
    OVDPS_NOTIFY_SILENT=1
};

enum {
    OVDPS_CANDI_MAYHIDE=0,
    OVDPS_CANDI_ALWAYSSHOW=1
};

enum {
    OVDPS_CANDI_MAYMOVE=0,
    OVDPS_CANDI_FIXED=1
};

void CVFixWindowOrigin(NSWindow *w, Point p);

#define CVWS_DEFAULTFILE	@"~/Sites/ov-theme/ov.html"

@protocol OVKeyReceiver
- (void)sendKey:(char)c;
@end

@implementation NotifyDelegate
-(void) init {
	NSLog(@"register growl delegate");
	[GrowlApplicationBridge setGrowlDelegate: self];
}

- (NSDictionary *)registrationDictionaryForGrowl {
	NSLog(@"register growl service");
	NSArray * arrayOfNotifys = [NSArray arrayWithObjects:SERVICE_NAME, nil];
	return [NSDictionary dictionaryWithObjectsAndKeys:
		arrayOfNotifys, GROWL_NOTIFICATIONS_ALL,
		arrayOfNotifys, GROWL_NOTIFICATIONS_DEFAULT, nil];
}

- (NSString *)applicationNameForGrowl {
	return @"OpenVanilla";
}

- (void) notify: (NSString *)title {
	NSLog(@"send growl notify");
	[GrowlApplicationBridge
	notifyWithTitle:title
		description:title
   notificationName:SERVICE_NAME
		   iconData:IconData
		   priority:0
		   isSticky:YES
	   clickContext:nil];
	NSLog(@"send growl notify....end");	
}

@end

@implementation ServerDelegate
- (BOOL)ping {
	return YES;
}

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
	notifyDelegate = [NotifyDelegate alloc];
	[notifyDelegate init];
	NSConnection *c=[NSConnection defaultConnection];
	[c setRootObject:self];
    if ([c registerName:OVDSPSRVR_NAME]) ; // NSLog(@"OVDisplayServer registered");
	else {
		NSLog(@"WebkitServer registration failed");
		NSAlert *a=[NSAlert alertWithMessageText:MSG(@"WebKitServer registration failed!")
			defaultButton:nil
			alternateButton:nil
			otherButton:nil
			informativeTextWithFormat:MSG(@"Make sure you have killed the process of OVDisplayServer")];
		[a runModal];
		[[NSApplication sharedApplication] terminate:self];
	}
	candion=NO;

	NSLog([urlbase absoluteString]);
	
	NSString *toload=[CVWS_DEFAULTFILE stringByExpandingTildeInPath];
	NSLog(MSG(@"this default candidate html file will be used: %@"), toload);
	
	NSURL *loadurl=[NSURL fileURLWithPath:toload];
	NSLog([loadurl description]);
		
	[[candiWeb mainFrame] loadRequest:[NSURLRequest requestWithURL:loadurl]];	
	[candiWeb setUIDelegate:self];
    [candiWeb setFrameLoadDelegate:self];
    [candiWeb setDrawsBackground:NO];	
	[candiWeb setEditable:NO];

	if(candialwaysshow != OVDPS_CANDI_ALWAYSSHOW) {
		[candiWindow orderOut:self];
	}
}

- (void)setConfig:(NSDictionary*)cfg {

    notistyle=OVDPS_NOTIFY_DEFAULT; 
    NSString *ns=[cfg valueForKey:@"notificationStyle" default:@"default"];
    if ([ns isEqualToString:@"silent"]) notistyle=OVDPS_NOTIFY_SILENT;
	
	candialwaysshow = OVDPS_CANDI_MAYHIDE;
	NSString *ns2=[cfg valueForKey:@"candialwaysshow" default:@"false"];
    if ([ns2 isEqualToString:@"true"]) candialwaysshow=OVDPS_CANDI_ALWAYSSHOW;
	else [cfg setValue:@"false" forKey:@"candialwaysshow"];
	
	candifix = OVDPS_CANDI_MAYMOVE;
	NSString *ns3=[cfg valueForKey:@"candifix" default:@"false"];
    if ([ns3 isEqualToString:@"true"]) candifix= OVDPS_CANDI_FIXED;	
	else [cfg setValue:@"false" forKey:@"candifix"];
}

- (void)candidateShow {
	candion=YES;
	[candiWindow orderFront:self];
}

- (void)executeScript:(NSString*) script {
	WebScriptObject *scrobj = [candiWeb windowScriptObject];	
	NSLog(@"evaluating javascript: %@", script);
	id y=[scrobj evaluateWebScript:script];
	NSLog(@"result: %@", [y description]);
}

- (void)candidateHide {
	candion=NO;	
	[self executeScript:@"ov_clear()"];
	
	if(candialwaysshow != OVDPS_CANDI_ALWAYSSHOW) {
		[candiWindow orderOut:self];
	}
}

- (void)candidateUpdate:(bycopy NSString*)s position:(Point)p {	
	[self executeScript:[NSString stringWithFormat:@"ov_update('%@')", s]];

	if(candifix != OVDPS_CANDI_FIXED) {
		CVFixWindowOrigin(candiWindow, p);
	}
}

- (void)notifyMessage:(bycopy NSString*)s position:(Point)p {
	if([s isEqualToString:@""]) {
		[candiWindow orderOut:self];
		return;
	}
	[notifyDelegate notify:s];	
	[self executeScript:[NSString stringWithFormat:@"ov_notify('%@')", s]];	
	
	[candiWindow orderFront:self];
	if(candifix != OVDPS_CANDI_FIXED) {
		CVFixWindowOrigin(candiWindow, p);
	}

//	CVFixWindowOrigin(noti, p);
//	[self solveConflict];
    
//    if (notistyle!=OVDPS_NOTIFY_DEFAULT) return;
//    [noti orderFront:self];
}

- (void)notifyClose {
	[self executeScript:[NSString stringWithFormat:@"ov_notifyclose()"]];
	[candiWindow orderOut:self];
}


- (void)aboutDialog {
    [aboutWindow setLevel:NSScreenSaverWindowLevel];
    [aboutWindow orderFront:self];
	[aboutWindow center];
	// [[NSApplication sharedApplication] orderFrontStandardAboutPanel:self];
}

- (void)dealloc {
	[urlbase release];
	[super dealloc];
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