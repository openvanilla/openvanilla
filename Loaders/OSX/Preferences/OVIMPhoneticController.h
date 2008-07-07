//
//  OVIMPhonetic.h
//  OpenVanilla
//
//  Created by zonble on 2008/7/8.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OVModuleController.h"

@interface OVIMPhoneticController : OVModuleController {
	IBOutlet id u_keyboardLayout;
	IBOutlet id u_selectKey;
}

- (IBAction)changeSelectKey:(id)sender;
- (IBAction)changePreference:(id)sender; 
@end
