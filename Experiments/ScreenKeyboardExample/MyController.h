//
//  MyController.h
//  ScreenKeyboardExample
//
//  Created by Lukhnos D. Liu on 9/19/2007.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#define OVDSPSRVR_NAME @"OVDisplayServer-0.8.0"

@interface MyController : NSWindowController {
	id _displayServer;
}
- (IBAction)characterAction:(id)sender;
- (IBAction)stringAction:(id)sender;
@end
