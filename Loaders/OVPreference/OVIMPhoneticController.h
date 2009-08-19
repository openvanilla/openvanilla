//
//  OVIMPhoneticController.h
//  OVPreference

#import <Cocoa/Cocoa.h>
#import "OVViewController.h"

@interface OVIMPhoneticController : OVViewController 
{
	IBOutlet NSPopUpButton *_keyboardLayout;
	IBOutlet NSComboBox *_selectionKeys;
}

@end
