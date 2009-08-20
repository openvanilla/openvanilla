//
//  OVIMGenericController.h
//  OpenVanilla

#import <Cocoa/Cocoa.h>
#import "OVViewController.h"

@interface OVIMGenericController : OVViewController
{
	IBOutlet NSStepper *_stepper;
	IBOutlet NSButton *_shiftSelectionKeyCheckBox;
	IBOutlet NSButton *_autoComposeCheckBox;
	IBOutlet NSButton *_hitMaxAndComposeCheckBox;
	IBOutlet NSButton *_warningBeepCheckBox;
	IBOutlet NSTextField *_maxKeySequenceLengthTextField;
	IBOutlet NSTextField *_matchOneCharTextField;
	IBOutlet NSTextField *_matchZeroOrMoreCharTextField;
}

- (IBAction)changeMaxKeySequenceLengthAction:(id)sender;

@end
