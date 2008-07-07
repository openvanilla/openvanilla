//
//  OVIMTibetan.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVModuleController.h"

@interface OVIMTibetanController : OVModuleController {
	IBOutlet id u_keyboardLayout;
}

- (IBAction)changePreference:(id)sender; 
@end
