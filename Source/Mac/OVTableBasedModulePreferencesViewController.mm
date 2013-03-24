//
// OVTableBasedModulePreferencesViewController.m
//
// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

#import "OVTableBasedModulePreferencesViewController.h"
#import "OVConstants.h"
#import "OVModuleManager.h"
#import "OVNonModalAlertWindowController.h"

@interface OVTableBasedModulePreferencesViewController () <OVNonModalAlertWindowControllerDelegate>
@end

@implementation OVTableBasedModulePreferencesViewController

@synthesize fieldAlphaNumericKeyboardLayout = _fieldAlphaNumericKeyboardLayout;
@synthesize fieldMaximumRadicalLength = _fieldMaximumRadicalLength;
@synthesize fieldClearReadingBufferAtCompositionError = _fieldClearReadingBufferAtCompositionError;
@synthesize fieldComposeWhileTyping = _fieldComposeWhileTyping;
@synthesize fieldSendFirstCandidateWithSpaceWithOnePageList = _fieldSendFirstCandidateWithSpaceWithOnePageList;
@synthesize fieldShouldComposeAtMaximumRadicalLength = _fieldShouldComposeAtMaximumRadicalLength;
@synthesize fieldUseSpaceAsFirstCandidateSelectionKey = _fieldUseSpaceAsFirstCandidateSelectionKey;
@synthesize cusmtomTableBasedInputMethodInfo = _cusmtomTableBasedInputMethodInfo;
@synthesize removeInputMethodButton = _removeInputMethodButton;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
    }
    
    return self;
}

- (void)setStateForButton:(NSButton *)button forKey:(NSString *)key
{
    [button setState:([self boolValueForKey:key] ? NSOnState : NSOffState)];
}

- (void)loadPreferences
{
    [super loadPreferences];
    if ([[OVModuleManager defaultManager] isCustomTableBasedInputMethod:self.moduleIdentifier]) {
        [self.cusmtomTableBasedInputMethodInfo setStringValue:NSLocalizedString(@"This is a customized input method.", nil)];
        [self.removeInputMethodButton setHidden:NO];
    }
    else {
        [self.cusmtomTableBasedInputMethodInfo setStringValue:NSLocalizedString(@"This is a built-in input method.", nil)];
        [self.removeInputMethodButton setHidden:YES];
    }

    [self setStateForButton:self.fieldClearReadingBufferAtCompositionError forKey:@"ClearReadingBufferAtCompositionError"];
    [self setStateForButton:self.fieldComposeWhileTyping forKey:@"ComposeWhileTyping"];
    [self setStateForButton:self.fieldShouldComposeAtMaximumRadicalLength forKey:@"ShouldComposeAtMaximumRadicalLength"];

    if ([self boolValueForKey:@"UseSpaceAsFirstCandidateSelectionKey"]) {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOnState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
    }
    else if ([self boolValueForKey:@"SendFirstCandidateWithSpaceWithOnePageList"]) {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOnState];
    }
    else {
        [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
    }

    NSUInteger length = [self unsignedIntegerValueForKey:@"MaximumRadicalLength"];
    if (!length || length > 100) {
        length = 5;
    }

    [self.fieldMaximumRadicalLength selectItemWithTitle:@"5"];

    for (NSMenuItem *item in [[self.fieldMaximumRadicalLength menu] itemArray]) {
        if ([[item title] integerValue] == length) {
            [self.fieldMaximumRadicalLength selectItem:item];
            break;
        }
    }

    [self configureKeyboardLayoutList:self.fieldAlphaNumericKeyboardLayout];
}

- (IBAction)updateField:(id)sender
{
    if (sender == self.fieldUseSpaceAsFirstCandidateSelectionKey) {
        if ([self.fieldUseSpaceAsFirstCandidateSelectionKey state] == NSOnState) {
            [self.fieldSendFirstCandidateWithSpaceWithOnePageList setState:NSOffState];
        }
    }
    else if (sender == self.fieldSendFirstCandidateWithSpaceWithOnePageList) {
        if ([self.fieldSendFirstCandidateWithSpaceWithOnePageList state] == NSOnState) {
            [self.fieldUseSpaceAsFirstCandidateSelectionKey setState:NSOffState];
        }
    }
    else if (sender == self.fieldAlphaNumericKeyboardLayout) {
        [self setStringValue:[[self.fieldAlphaNumericKeyboardLayout selectedItem] representedObject] forKey:OVAlphanumericKeyboardLayoutKey];
    }

    [self setBoolValue:([self.fieldClearReadingBufferAtCompositionError state] == NSOnState) forKey:@"ClearReadingBufferAtCompositionError"];
    [self setBoolValue:([self.fieldComposeWhileTyping state] == NSOnState) forKey:@"ComposeWhileTyping"];
    [self setBoolValue:([self.fieldShouldComposeAtMaximumRadicalLength state] == NSOnState) forKey:@"ShouldComposeAtMaximumRadicalLength"];
    [self setBoolValue:([self.fieldUseSpaceAsFirstCandidateSelectionKey state] == NSOnState) forKey:@"UseSpaceAsFirstCandidateSelectionKey"];
    [self setBoolValue:([self.fieldSendFirstCandidateWithSpaceWithOnePageList state] == NSOnState) forKey:@"SendFirstCandidateWithSpaceWithOnePageList"];

    NSMenuItem *selectedItem = [self.fieldMaximumRadicalLength selectedItem];
    if (selectedItem) {
        NSInteger length = [[selectedItem title] integerValue];
        [self setUnsignedIntegerValue:(NSUInteger)length forKey:@"MaximumRadicalLength"];
    }
}

- (IBAction)removeInputMethodAction:(id)sender
{
    [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Confirm Removal", nil) content:[NSString stringWithFormat:NSLocalizedString(@"Are you sure you want to remove the input method\"%@\"?", nil), [[OVModuleManager defaultManager] localizedInputMethodName:self.moduleIdentifier]] confirmButtonTitle:NSLocalizedString(@"Remove", nil) cancelButtonTitle:NSLocalizedString(@"Cancel", nil) cancelAsDefault:YES delegate:self];
}

- (void)setModuleIdentifier:(NSString *)moduleIdentifier
{
    [super setModuleIdentifier:moduleIdentifier];

    OVNonModalAlertWindowController *controller = [OVNonModalAlertWindowController sharedInstance];
    if ([[controller window] isVisible] && controller.delegate == self) {
        [controller cancelOperation:self];
    }
}

- (void)nonModalAlertWindowControllerDidConfirm:(OVNonModalAlertWindowController *)controller
{
    NSError *error = nil;
    BOOL success = [[OVModuleManager defaultManager] removeCustomTableBasedInputMethod:self.moduleIdentifier error:&error];
    if (!success) {
        [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Removal Failed", nil) content:[NSString stringWithFormat:NSLocalizedString(@"Failed to remove the input method\"%@\".\n\nError: %@", nil), [[OVModuleManager defaultManager] localizedInputMethodName:self.moduleIdentifier], [error localizedDescription]] confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
    }
}

- (void)nonModalAlertWindowControllerDidCancel:(OVNonModalAlertWindowController *)controller
{
}
@end
