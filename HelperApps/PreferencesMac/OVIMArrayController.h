//
//  OVIMArrayController.h
//  OVPreference

#import <Cocoa/Cocoa.h>
#import "OVViewController.h"

@interface OVIMArrayController : OVViewController
{
	IBOutlet NSButton *_quickModeCheckBox;
	IBOutlet NSButton *_specialCodeCheckBox;
}

- (IBAction)setQuickModeAction:(id)sender;
- (IBAction)setSpecialCodeAction:(id)sender;

@end
