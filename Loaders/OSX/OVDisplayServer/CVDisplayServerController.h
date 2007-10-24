//
//  CVDisplayServerController.h
//  NewDisplayServer
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVDisplayServer.h"

@interface CVDisplayServerController : NSWindowController <OVDisplayServer> {
	IBOutlet id _aboutWindow;
	IBOutlet id _candidateWindow;
	IBOutlet id _notificationWindow;
	IBOutlet id _userText;
	
	float _fadeAlpha;
	NSTimer *_fadeTimer;
	
	int _nextRemoteID;
	int _currentFocusRemoteID;
}
- (IBAction)updateTextAction:(id)sender;
- (void)stopTimer;
- (void)fadeStart;
- (void)fadeWork;
@end
