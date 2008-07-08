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
#define MSG(x)      [[NSBundle mainBundle] localizedStringForKey:x value:nil table:nil]

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
	
	IBOutlet id u_testSoundButton;
	IBOutlet id u_soundMenu;
	IBOutlet id u_hotkeyField;
	
	NSString *_beepSound;
	NSSound *_sound;
}
- (NSString *)beepSound;
- (void)setSound;
- (void)changeFont:(id)sender;

- (IBAction)changeColor:(id)sender;
- (IBAction)changeOpacity:(id)sender;
- (IBAction)changeSound:(id)sender;
- (IBAction)testSound:(id)sender;
- (IBAction)launchFontPanel:(id)sender;
- (IBAction)setDisplayServerSetting:(id)sender;
@end
