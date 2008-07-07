//
//  OVIMPOJHoloController.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVModuleController.h"

@interface OVIMPOJHoloController : OVModuleController {
	IBOutlet id u_keyboardLayout;
	IBOutlet id u_fullPOJOutput;
	IBOutlet id u_asciiOutput;
	IBOutlet id u_warningBeep;
}

- (IBAction)changePreference:(id)sender; 
@end
