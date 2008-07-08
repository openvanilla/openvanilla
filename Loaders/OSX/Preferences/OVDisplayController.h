//
//  OVDisplayController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVModuleController.h"
#import "CVPreviewView.h"

@interface OVDisplayController : OVModuleController {
	IBOutlet id u_fontManager;
	
	IBOutlet id u_foreground;
	IBOutlet id u_background;
	IBOutlet id u_fontLabel;
	IBOutlet id u_opacityScroll;
	IBOutlet id u_opacityLabel;
	IBOutlet id u_useWindowAnimation;
	IBOutlet id u_notificationStyle;
	IBOutlet id u_preview;
	
	IBOutlet id u_hotkeyField;
	
	NSString *beepSound;
}

- (IBAction)changeColor:(id)sender;
- (IBAction)changeOpacity:(id)sender;
- (IBAction)launchFontPanel:(id)sender;
- (IBAction)setDisplayServerSetting:(id)sender;
- (void)changeFont:(id)sender;
@end
