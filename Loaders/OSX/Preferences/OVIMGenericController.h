//
//  OVIMGenericController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/5.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVModuleController.h"

@interface OVIMGenericController : OVModuleController {
	IBOutlet id u_maxLength;
	IBOutlet id u_maxLengthString;	
	IBOutlet id u_shiftSelectionKey;
	IBOutlet id u_autoCompose;
	IBOutlet id u_hitMaxAndCompose;
	IBOutlet id u_warningBeep;
	IBOutlet id u_matchOneChar;
	IBOutlet id u_matchZeroOrMoreChar;
}

- (IBAction)changeMaxLength:(id)sender;
- (IBAction)changePreference:(id)sender; 
- (IBAction)changeMatchOneChar:(id)sender;
- (IBAction)changeMatchZeroOrMoreChar:(id)sender;
@end
