//
// OVAddTableBasedInputMethodViewController.m
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

#import "OVAddTableBasedInputMethodViewController.h"
#import "OVConstants.h"
#import "OVModuleManager.h"
#import "OVNonModalAlertWindowController.h"
#import "OVPreferencesWindowController.h"

@interface OVAddTableBasedInputMethodViewController () <OVNonModalAlertWindowControllerDelegate>
- (BOOL)install:(NSString *)path;

@property (strong) NSString *tablePathToBeInstalled;
@property (strong) NSString *moduleIdentifierIfInstalled;
@end

@implementation OVAddTableBasedInputMethodViewController

- (void)loadPreferences
{
    // add link to the more info text field
    // cf. http://developer.apple.com/library/mac/#qa/qa1487/_index.html

    NSMutableAttributedString *attrString = [self.moreInfoTextField.attributedStringValue mutableCopy];
    NSRange linkRange = [attrString.string rangeOfString:OVMainSiteURLString];
    if (linkRange.location == NSNotFound) {
        return;
    }

    [attrString beginEditing];
    NSURL *url = [NSURL URLWithString:OVMainSiteURLString];
    [attrString addAttribute:NSLinkAttributeName value:url range:linkRange];
    [attrString addAttribute:NSForegroundColorAttributeName value:[NSColor blueColor] range:linkRange];
    [attrString addAttribute:NSUnderlineStyleAttributeName value:@(NSUnderlineStyleSingle) range:linkRange];
    [attrString endEditing];

    self.moreInfoTextField.attributedStringValue = attrString;
    self.moreInfoTextField.allowsEditingTextAttributes = YES;
    self.moreInfoTextField.selectable = YES;
}

- (IBAction)importNewTableAction:(id)sender
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setTitle:NSLocalizedString(@"Pick the .cin Table to Import", nil)];
    [panel setLevel:CGShieldingWindowLevel() + 1];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:@[@"cin"]];

    [panel beginWithCompletionHandler:^(NSInteger result) {
        if (result != NSFileHandlingPanelOKButton) {
            return;
        }

        NSArray *files = [panel URLs];
        if (![files count]) {
            return;
        }

        NSString *cinPath = [files[0] path];
        NSError *error = nil;
        BOOL override = NO;
        NSString *identifier = nil;
        NSString *localizedName = nil;
        BOOL canInstall = [[OVModuleManager defaultManager] canInstallCustomTableBasedInputMethodWithTablePath:cinPath willOverrideBuiltInTable:&override identifier:&identifier localizedName:&localizedName error:&error];
        if (!canInstall) {
            [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Cannot Import Input Method", nil) content:[NSString stringWithFormat:NSLocalizedString(@"\"%@\" is not a valid cin file.", nil), [cinPath lastPathComponent]] confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
            return;
        }

        if (override) {
            self.tablePathToBeInstalled = cinPath;
            self.moduleIdentifierIfInstalled = identifier;

            [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Overwriting Existing Input Method", nil) content:[NSString stringWithFormat:NSLocalizedString(@"\"%@\" will replace an existing input method that you have. Do you want to continue?", nil), [cinPath lastPathComponent]] confirmButtonTitle:NSLocalizedString(@"Overwrite", nil) cancelButtonTitle:NSLocalizedString(@"Cancel", nil) cancelAsDefault:YES delegate:self];
            return;
        }

        [self install:cinPath];
    }];
}

- (BOOL)install:(NSString *)path
{
    NSString *cinPath = path;
    NSError *error = nil;
    BOOL override = NO;
    NSString *identifier = nil;
    NSString *localizedName = nil;
    BOOL canInstall = [[OVModuleManager defaultManager] canInstallCustomTableBasedInputMethodWithTablePath:cinPath willOverrideBuiltInTable:&override identifier:&identifier localizedName:&localizedName error:&error];
    if (!canInstall) {
        [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Cannot Import Input Method", nil) content:[NSString stringWithFormat:NSLocalizedString(@"\"%@\" is not a valid cin file.", nil), [cinPath lastPathComponent]] confirmButtonTitle:NSLocalizedString(@"Dismiss", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];
        return NO;
    }

    [[OVModuleManager defaultManager] installCustomTableBasedInputMethodWithTablePath:path];

    [[OVNonModalAlertWindowController sharedInstance] showWithTitle:NSLocalizedString(@"Input Method Imported", nil) content:[NSString stringWithFormat:NSLocalizedString(@"Your new input method \"%@\" is ready to use", nil), localizedName] confirmButtonTitle:NSLocalizedString(@"OK", nil) cancelButtonTitle:nil cancelAsDefault:NO delegate:nil];

    [self.preferencesWindowController selectInputMethodIdentifier:identifier];
    return YES;
}

- (void)nonModalAlertWindowControllerDidConfirm:(OVNonModalAlertWindowController *)controller
{
    if (controller.delegate == self) {
        [self install:self.tablePathToBeInstalled];
    }
}

- (void)nonModalAlertWindowControllerDidCancel:(OVNonModalAlertWindowController *)controller
{
    self.tablePathToBeInstalled = nil;
    self.moduleIdentifierIfInstalled = nil;
}

@end
