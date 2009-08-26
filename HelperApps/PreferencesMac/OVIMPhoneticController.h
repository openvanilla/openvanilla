//
//  OVIMPhoneticController.h
//  OVPreference

#import <Cocoa/Cocoa.h>
#import "OVViewController.h"

@class OVIMPhoneticController;

@interface PhoneticSelectionKeyTransformer: NSValueTransformer
{
	OVIMPhoneticController *_delegate;
}

- (void)setDelegate:(id)delegate;

@end

@interface OVIMPhoneticController : OVViewController 
{
	IBOutlet NSPopUpButton *_keyboardLayout;
	IBOutlet NSComboBox *_selectionKeys;
}

- (void)valueDidTransform:(id)value;

@end
